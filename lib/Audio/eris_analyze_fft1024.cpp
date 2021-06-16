/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include "eris_analyze_fft1024.h"
#include "sqrt_integer.h"
#include "utility/dspinst.h"
#include "arm_const_structs.h" //WILL BE NEEDED FOR IMPLEMENTING f32 FFT


//Fat Audio - to add trailing zeros to fft buffer
/*
static void zero_to_fft_buffer(void *destination)
{
	uint32_t *dst = (uint32_t *)destination;
	for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
		*dst++ = 0;
	}
}
*/
// 140312 - PAH - slightly faster copy
void erisAudioAnalyzeFFT1024::copy_to_fft_buffer(void *destination, const void *source,int subsample)
{
	const uint16_t *src = (const uint16_t *)source;
	uint32_t *dst = (uint32_t *)destination;
	//Fat Audio - dumb downsample
	for (; SAMPLING_INDEX < AUDIO_BLOCK_SAMPLES; SAMPLING_INDEX+=subsample) {
		if (SAMPLING_INDEX < AUDIO_BLOCK_SAMPLES) *dst++ = src[SAMPLING_INDEX];
	}
	SAMPLING_INDEX -= AUDIO_BLOCK_SAMPLES;
}


static void apply_window_to_fft_buffer(void *buffer, const void *window)
{
	int16_t *buf = (int16_t *)buffer;
	const int16_t *win = (int16_t *)window;;

	for (int i=0; i < 1024; i++) {
		int32_t val = *buf * *win++;
		//*buf = signed_saturate_rshift(val, 16, 15);
		*buf = val >> 15;
		buf += 2;
	}

}

static void apply_window_to_fft_buffer_f32(float32_t *buffer, const float32_t *window)
{
	float32_t *buf = (float32_t *)buffer;
	const float32_t *win = (float32_t *)window;;

	for (int16_t i=0; i < 1024; i++) {
		*buf++ *= *win++;
	}
	
}

void erisAudioAnalyzeFFT1024::update(void)
{
	audio_block_t *block;

	block = receiveReadOnly();
	if (!block) return;


	//FAT Audio - add the ability to turn FFT on or OFF to save CPU
	if (!enabled){
		release(block);
		return;
	};

	uint16_t ofs;
	//calcuate active subsampling step and offset
	if (ssr == SS_LOWFREQ){
		MEM_STEP = subsample_lowfreqrange;
		subsample_by = (int)subsample_lowfreqrange;
	}
	else if (ssr == SS_HIGHFREQ){
		MEM_STEP = subsample_highfreqrange;
		subsample_by = (int)subsample_highfreqrange;
	}
	BLOCKS_PER_FFT = (1024 / AUDIO_BLOCK_SAMPLES) * subsample_by;
	BLOCK_REFRESH_SIZE =  BLOCKS_PER_FFT/2;//64 / (AUDIO_BLOCK_SAMPLES /subsample_by);
	if (ssr == SS_LOWFREQ) BLOCK_REFRESH_SIZE = BLOCKS_PER_FFT/2;//32 / (AUDIO_BLOCK_SAMPLES /subsample_by);


	ofs = (AUDIO_BLOCK_SAMPLES/subsample_by) * sample_block;

	if (sample_block < BLOCKS_PER_FFT - 1){
		copy_to_fft_buffer(buffer+ofs, block->data,subsample_by);
		release(block);
		sample_block++;
	} else{
		copy_to_fft_buffer(buffer+ofs, block->data,subsample_by);
		release(block);
		#ifdef ENABLE_F32_FFT
		//copy buffer while casting to float and scale to the range -1 to 1
		for (int16_t i=0; i < 2048; i++){
			tmp_buffer[i] = (float32_t)buffer[i] / 32768.0;
		}
		apply_window_to_fft_buffer_f32(tmp_buffer, window_f32);
		arm_cfft_radix4_f32(&fft_inst, tmp_buffer);
		/* Process the data through the Complex Magnitude Module for calculating the magnitude at each bin */ 
		arm_cmplx_mag_f32(tmp_buffer, output, 1024);  
		/* Calculates maxValue and returns corresponding BIN value */ 
		//arm_max_f32(testOutput, fftSize, &maxValue, &testIndex); 
		#else
		memcpy(tmp_buffer,buffer,2048 * sizeof(int16_t));
		apply_window_to_fft_buffer(tmp_buffer, window);
		arm_cfft_radix4_q15(&fft_inst, tmp_buffer);
		outputflag = false; //output flag is false while updating the fft results
		
		for (int i=0; i < 512; i++) {
			uint32_t tmp = *((uint32_t *)tmp_buffer + i); // real & imag
			uint32_t magsq = multiply_16tx16t_add_16bx16b(tmp, tmp);
			output[i] = sqrt_uint32_approx(magsq);
			output_packed[i] = tmp;//normalized_atan2((float)real,(float)imag)*180/PI;//atan2(imag,real)*180/PI;
		}
		#endif

		if (sample_block!= 0){
			sample_block = BLOCKS_PER_FFT - BLOCK_REFRESH_SIZE;
			//fft overlap - restore tmp cpy of last half to first half
			//memmove(buffer,&buffer+((AUDIO_BLOCK_SAMPLES/subsample_by) * BLOCK_REFRESH_SIZE *  sizeof(int16_t)), (AUDIO_BLOCK_SAMPLES/subsample_by) * (BLOCKS_PER_FFT - BLOCK_REFRESH_SIZE) * sizeof(int16_t));
			memmove(buffer,&buffer[(AUDIO_BLOCK_SAMPLES/subsample_by) * BLOCK_REFRESH_SIZE], (AUDIO_BLOCK_SAMPLES/subsample_by) * (BLOCKS_PER_FFT - BLOCK_REFRESH_SIZE)* sizeof(int16_t));
			//memmove(buffer,&buffer[1024], 1024 * sizeof(int16_t));
			
			outputflag = true;
		}

	}
}
