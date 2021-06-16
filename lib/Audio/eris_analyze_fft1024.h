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

#ifndef erisanalyze_fft1024_h_
#define erisanalyze_fft1024_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "arm_math.h"
#include "analyze_fft1024.h"
#include <stdlib.h>
#include "data_windows_f32.h"

#define ENABLE_F32_FFT

enum subsample_range{SS_LOWFREQ, SS_HIGHFREQ};


typedef struct FFTReadRangeStruct{
	uint16_t cqtBin;		// Provided by the caller
	float startFrequency;		// Provided by the caller
	float stopFrequency;		// Provided by the caller
	
	uint16_t startBin;		
	uint16_t stopBin;		
	uint16_t peakBin;		
	//uint32_t peakFFTResult;		//real and imag packed data can be used to calc phase 
	float estimatedFrequency;
	float peakFrequency;
	float peakValue;
	float avgValueFast;				//used to calc moving average convergence / divergence (MACD) 
	float avgValueSlow; 			//by comparing a short and long moving average; slow transient detection
	float macdValue;
	float transientValue;			//difference between the peak and fast peak values
} FFTReadRange;


class erisAudioAnalyzeFFT1024 : public AudioStream
{
public:
	erisAudioAnalyzeFFT1024() : AudioStream(1, inputQueueArray),
	  sample_block(0), outputflag(false) {
		#ifdef ENABLE_F32_FFT
		arm_cfft_radix4_init_f32(&fft_inst,1024, 0, 1);
		window_f32 = AudioWindowHamming1024_f32;//AudioWindowKaiser14_1024_f32;//AudioWindowBlackman1024_f32;AudioWindowHamming1024_f32;//
		window = NULL;
		#else
		arm_cfft_radix4_init_q15(&fft_inst, 1024, 0, 1);
		window_f32 = NULL;
		window = AudioWindowHanning1024;
		#endif
		shortName="fft1024";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
		enabled = false;
		sample_block=0;
		SAMPLING_INDEX=0;
		MEM_STEP = 0x010;
		subsample_by = 4;
		BLOCKS_PER_FFT = (1024 / AUDIO_BLOCK_SAMPLES) * subsample_by;
		BLOCK_REFRESH_SIZE = BLOCKS_PER_FFT/2;
		subsample_lowfreqrange = 32;//ratio should be 2:1; bw is fc of the low range
		subsample_highfreqrange = 16;//
		ssr = SS_HIGHFREQ;	
		//memset(&output_packed,0,sizeof(uint32_t)*512);
		memset(&buffer,0,sizeof(int16_t)*2048);
		#ifdef ENABLE_F32_FFT
		//memset(&output,0,sizeof(float32_t)*1024);
		arm_fill_f32(0,output,1024);
		//memset(&tmp_buffer,0,sizeof(float32_t)*2048);
		arm_fill_f32(0,tmp_buffer,2048);
		#else
		memset(&output,0,sizeof(uint16_t)*512);
		memset(&tmp_buffer,0,sizeof(int16_t)*2048);
		#endif
	}
	//FAT Audio
	void reset(){
		sample_block=0;
		SAMPLING_INDEX=0;
	};

	void enableFFT(bool enable_state){
		reset();
		enabled = enable_state;
	}

	void configSubsample(uint16_t subsample,subsample_range range){
		//subsampling provides more resolution in the lower frequency range
		//...in exchange for lower bandwidth
		//values between 16 (high range) & 48 (low range) are suitable for guitar
		if (range == SS_LOWFREQ) subsample_lowfreqrange = subsample;
		else if (range == SS_HIGHFREQ) subsample_highfreqrange = subsample;
		outputflag = false;
		reset();
	}

	void setActiveRange(subsample_range range){
		ssr = range;
		outputflag = false;
		reset();
	}

	void toggleActiveRange(){
		if (ssr == SS_LOWFREQ) ssr = SS_HIGHFREQ;
		else if (ssr == SS_HIGHFREQ) ssr = SS_LOWFREQ;
		outputflag = false;
		reset();
	}

	bool available() {
		if (outputflag == true) {
			outputflag = false;
			return true;
		}
		return false;
	}
	float read(unsigned int binNumber) {
		if (binNumber > 511) return 0.0;
		#ifdef ENABLE_F32_FFT
		return output[binNumber];
		#else
		return (float)(output[binNumber]) * (1.0 / 16384.0);
		#endif
	}
	float read(unsigned int binFirst, unsigned int binLast,FFTReadRange *fftRR = NULL) {
		float32_t maxf = 0;
		float32_t powerf = 0;
		uint32_t peak_index;
		uint32_t span;

		span = binLast - binFirst;
		if(span<2) return 0.0;

		if (binFirst > binLast) {
			unsigned int tmp = binLast;
			binLast = binFirst;
			binFirst = tmp;
		}
		if(fftRR){
			fftRR->peakBin = 0;
			fftRR->peakFrequency = 0;
			fftRR->peakValue = 0;
		}
		if (binFirst > 510) return 0.0;
		if (binLast > 510) binLast = 510;
		arm_power_f32(&output[binFirst], span, &powerf);
		arm_max_f32 (&output[binFirst], span, &maxf, &peak_index);
		maxf = 0;		
		if(fftRR){
			fftRR->peakValue = powerf / (span);
			fftRR->peakBin = peak_index + binFirst;
		} 
		return powerf/(1024);
	}
	float read(FFTReadRange *fftRR){
		return read(fftRR->startFrequency, fftRR->stopFrequency, fftRR);
	}

	float read(float freq_from, float freq_to, FFTReadRange *fftRR = NULL) {
		//convert f to bin
		float bw;
		float bin_size;
		unsigned int start_bin;
		unsigned int stop_bin;

		bw = (AUDIO_SAMPLE_RATE_EXACT / (float)subsample_by)/2.0;
		bin_size = bw/512.0f;
		start_bin = (unsigned int)(freq_from / bin_size);
		stop_bin = (unsigned int)(freq_to / bin_size);
		if (start_bin > 511 || stop_bin > 511){
			start_bin = 0;
			stop_bin = 0;
		}
		/*
		Serial.print(F("fft read: "));
		Serial.print(subsample_by);Serial.print(F(","));
		Serial.print(bw);Serial.print(F(","));
		Serial.print(freq_from);Serial.print(F(","));
		Serial.print(freq_to);Serial.print(F(","));
		Serial.print(bin_size);Serial.print(F(","));
		Serial.print(start_bin);Serial.print(F(","));
		Serial.println(stop_bin); 
		*/
		if(fftRR){
			fftRR->startBin = start_bin;
			fftRR->stopBin = stop_bin;
			fftRR->startFrequency = freq_from;
			fftRR->stopFrequency = freq_to;
		}
		
		if (freq_to > bw){
			if(fftRR){
				int16_t cqtBin;
				cqtBin = fftRR->cqtBin;
				//memset(&fftRR,0,sizeof(FFTReadRange));
				fftRR->cqtBin = cqtBin;
				fftRR->peakBin=0;		
				fftRR->estimatedFrequency=0;
				fftRR->peakFrequency=0;
				fftRR->peakValue=0;
				fftRR->avgValueFast=0;				//used to calc moving average convergence / divergence (MACD) 
				fftRR->avgValueSlow=0; 			//by comparing a short and long moving average; slow transient detection
				fftRR->macdValue=0;
				fftRR->transientValue=0;			//difference between the peak and fast peak values
			}
			return 0;
		}


		float rval = read(start_bin,stop_bin,fftRR);
		if(fftRR){
			//fftRR->peakValue *= 80.0 / (fftRR->peakBin);//bin_size;
			fftRR->peakValue *= 100.0/(subsample_by);
			fftRR->peakValue = sqrt(fftRR->peakValue * bin_size);
			
			//from the peak bin calc the freq
			fftRR->peakFrequency = (fftRR->peakBin * bin_size) -  (bin_size/2.0); //center of the bin
			if (fftRR->peakFrequency < 0) fftRR->peakFrequency = 0.01;
			if ((fftRR->peakBin > 1) && (fftRR->peakBin < 510) && (output[fftRR->peakBin] > 0)){
				//from the balance of the side lobes, estimate the actual frequency
				float ratio = 1;
				float lobeFrequency = 1;
				if ((output[fftRR->peakBin+1]-output[fftRR->peakBin-1]) > 0.1){
					//pos lobe
					ratio = (output[fftRR->peakBin+1] - output[fftRR->peakBin-1] + output[fftRR->peakBin+1]) / (1.0 * output[fftRR->peakBin]);
					lobeFrequency = ((fftRR->peakBin+1) * bin_size) - bin_size/2;
				 } else if (output[fftRR->peakBin-1] > 0.1){ 
					 //neg lobe
					ratio = (output[fftRR->peakBin-1]-output[fftRR->peakBin+1] + output[fftRR->peakBin-1]) / (1.0 * output[fftRR->peakBin]);
				 	lobeFrequency = ((fftRR->peakBin-1) * bin_size)  - bin_size/2;
				 }
				 fftRR->estimatedFrequency = (fftRR->peakFrequency * (1-ratio)) + (lobeFrequency * ratio); 
				 //clamp estimate
				 if (fftRR->estimatedFrequency > fftRR->stopFrequency)fftRR->estimatedFrequency = fftRR->stopFrequency;
				 if (fftRR->estimatedFrequency < fftRR->startFrequency)fftRR->estimatedFrequency = fftRR->startFrequency;

				fftRR->avgValueFast = (fftRR->avgValueFast * 0.75) + (fftRR->peakValue * 0.25);	//used to calc moving average convergence / divergence (MACD) 
				fftRR->avgValueSlow = (fftRR->avgValueSlow * 0.10) + (fftRR->avgValueFast * 0.90); 	//by comparing a short and long moving average; slow transient detection
				//if(fftRR->peakValue > fftRR->avgValueFast) fftRR->avgValueFast = fftRR->peakValue;
				//if(fftRR->peakValue > fftRR->avgValueSlow) fftRR->avgValueSlow = (fftRR->avgValueSlow * 0.9) + (fftRR->peakValue * 0.1);
				
				fftRR->macdValue = fftRR->avgValueFast - fftRR->avgValueSlow;
				fftRR->transientValue = fftRR->peakValue - fftRR->avgValueSlow;
			}
		}
			
		return rval; 
	}

	//comparison function used for qsort of FFTReadRange arrays (used for finding cqt peaks)
	static int compare_fftrr_value(const void *p, const void *q) {
		if (((const FFTReadRange *)p)->avgValueFast > ((const FFTReadRange *)q)->avgValueFast) return -1;
		return 1;
	}

	static int compare_fftrr_cqt_bin(const void *p, const void *q) {
		if (((const FFTReadRange *)p)->cqtBin <= ((const FFTReadRange *)q)->cqtBin) return 1;
		return -1;
	}

	static void sort_fftrr_by_value(FFTReadRange *a, size_t n) {
		//helper function used to sort FFTReadRange arrays (used for CQT)
		qsort(a, n, sizeof(*a), compare_fftrr_value);
	}

	static void sort_fftrr_by_cqt_bin(FFTReadRange *a, size_t n) {
		//helper function used to sort FFTReadRange arrays (used for CQT)
		qsort(a, n, sizeof(*a), compare_fftrr_cqt_bin);
	}

	void averageTogether(uint8_t n) {
		// not implemented yet (may never be, 86 Hz output rate is ok)
	}
	void windowFunction(const int16_t *w) {
		window = w;
	}
	virtual void update(void);
	//uint32_t output_packed[512] __attribute__ ((aligned (4))); //16bit real and imag packed data 
private:
	void init(void);
	void copy_to_fft_buffer(void *destination, const void *source,int subsample);
	const int16_t *window;
	const float32_t *window_f32;
public: //tmp for debug
	uint16_t sample_block;
	bool enabled; //FAT Audio
	uint16_t MEM_STEP;
	int subsample_by;
	int SAMPLING_INDEX;
	uint16_t BLOCKS_PER_FFT;
	uint16_t BLOCK_REFRESH_SIZE;
	uint16_t subsample_lowfreqrange;
	uint16_t subsample_highfreqrange;
	subsample_range ssr;
private:	
	volatile bool outputflag;
	audio_block_t *inputQueueArray[1];
public: //tmp for debug
	#ifdef ENABLE_F32_FFT
	arm_cfft_radix4_instance_f32 fft_inst;
	float32_t tmp_buffer[2048] __attribute__ ((aligned (4)));
	float32_t output[1024] __attribute__ ((aligned (4)));
	#else
	arm_cfft_radix4_instance_q15 fft_inst;
	int16_t tmp_buffer[2048] __attribute__ ((aligned (4)));
	uint16_t output[512] __attribute__ ((aligned (4)));
	#endif
	int16_t buffer[2048] __attribute__ ((aligned (4)));
};

#endif
