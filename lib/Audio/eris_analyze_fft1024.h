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


enum subsample_range{SS_LOWFREQ, SS_HIGHFREQ};


typedef struct FFTReadRangeStruct{
	uint16_t cqtBin;		// Provided by the caller
	uint16_t startBin;		
	uint16_t stopBin;		
	uint16_t peakBin;		
	float startFrequency;		// Provided by the caller
	float stopFrequency;		// Provided by the caller
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
	  window(AudioWindowNuttall1024), sample_block(0), outputflag(false) {
		arm_cfft_radix4_init_q15(&fft_inst, 1024, 0, 1);
		shortName="fft1024";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
		enabled = false;
		sample_block=0;
		SAMPLING_INDEX=0;
		MEM_STEP = 0x010;
		subsample_by = 8;
		BLOCKS_PER_FFT = 128;
		BLOCK_REFRESH_SIZE = 4; 
		subsample_lowfreqrange = 32;//689hz
		subsample_highfreqrange = 4;//~2637 (24th fret)
		ssr = SS_HIGHFREQ;
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
		return (float)(output[binNumber]) * (1.0 / 16384.0);
	}
	float read(unsigned int binFirst, unsigned int binLast,FFTReadRange *fftRR = NULL) {
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
		if (binFirst > 511) return 0.0;
		if (binLast > 511) binLast = 511;
		float maxf = 0;
		float comp;
		do {
			comp = (float)output[binFirst++];
			maxf = max(comp,maxf);
			if(fftRR) {
				if (maxf > fftRR->peakValue) {
					fftRR->peakBin = binFirst-1;
					fftRR->peakValue = maxf;
				}
			}
		} while (binFirst <= binLast);
		if(fftRR) fftRR->peakValue = maxf * (1.0 / 16384.0);
		return maxf * (1.0 / 16384.0);
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

		bw = (AUDIO_SAMPLE_RATE_EXACT * 0.5) / (float)subsample_by;
		bin_size = bw/512;
		start_bin = (unsigned int)(freq_from / bin_size);
		stop_bin = (unsigned int)(freq_to / bin_size);
		
		if(fftRR){
			fftRR->startBin = start_bin;
			fftRR->stopBin = stop_bin;
			fftRR->startFrequency = freq_from;
			fftRR->stopFrequency = freq_to;
		}
		
		/*
		Serial.print(F("fft read: "));
		Serial.print(subsample_by);Serial.print(F(","));
		Serial.print(bw);Serial.print(F(","));
		Serial.print(bin_size);Serial.print(F(","));
		Serial.print(start_bin);Serial.print(F(","));
		Serial.println(stop_bin);
		*/
		float rval = read(start_bin,stop_bin,fftRR);
		if(fftRR){
			//from the peak bin calc the freq
			fftRR->peakFrequency = (fftRR->peakBin * bin_size) -  (bin_size/2.0); //center of the bin
			if ((fftRR->peakBin > 1) && (fftRR->peakBin < 510) && (output[fftRR->peakBin] > 0)){
				//from the balance of the side lobes, estimate the actual frequency
				float ratio = 1;
				float lobeFrequency = 1;
				if ((output[fftRR->peakBin+1]-output[fftRR->peakBin-1]) > 0.1){
					//pos lobe
					ratio = (output[fftRR->peakBin+1] - output[fftRR->peakBin-1]) / (1.0 * output[fftRR->peakBin]);
					lobeFrequency = ((fftRR->peakBin+1) * bin_size) - bin_size/2;
				 } else if (output[fftRR->peakBin-1] > 0.1){ 
					 //neg lobe
					ratio = (output[fftRR->peakBin-1]-output[fftRR->peakBin+1]) / (1.0 * output[fftRR->peakBin]);
				 	lobeFrequency = ((fftRR->peakBin-1) * bin_size)  - bin_size/2;
				 }
				 fftRR->estimatedFrequency = (fftRR->peakFrequency * (1-ratio)) + (lobeFrequency * ratio); 
				 //clamp estimate
				 if (fftRR->estimatedFrequency > fftRR->stopFrequency)fftRR->estimatedFrequency = fftRR->stopFrequency;
				 if (fftRR->estimatedFrequency < fftRR->startFrequency)fftRR->estimatedFrequency = fftRR->startFrequency;

				fftRR->avgValueFast = (fftRR->avgValueFast * 0.40) + (fftRR->peakValue * 0.60);	//used to calc moving average convergence / divergence (MACD) 
				fftRR->avgValueSlow = (fftRR->avgValueFast * 0.90) + (fftRR->peakValue * 0.10); 	//by comparing a short and long moving average; slow transient detection
				fftRR->macdValue = fftRR->avgValueFast - fftRR->avgValueSlow;
				fftRR->transientValue = fftRR->peakValue - fftRR->avgValueSlow;
			}
		}
		return rval; 
	}

	//comparison function used for qsort of FFTReadRange arrays (used for finding cqt peaks)
	static int compare_fftrr_value(const void *p, const void *q) {
		if (((const FFTReadRange *)p)->peakValue > ((const FFTReadRange *)q)->peakValue) return -1;
		return 1;
	}

	static int compare_fftrr_cqt_bin(const void *p, const void *q) {
		if (((const FFTReadRange *)p)->cqtBin > ((const FFTReadRange *)q)->cqtBin) return -1;
		return 1;
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
	uint16_t output[512] __attribute__ ((aligned (4)));

	
private:
	void init(void);
	void copy_to_fft_buffer(void *destination, const void *source,int subsample);
	const int16_t *window;
	int16_t buffer[2048] __attribute__ ((aligned (4)));
	int16_t tmp_buffer[2048] __attribute__ ((aligned (4)));
	//uint32_t sum[512];
	//uint8_t count;
	uint8_t sample_block;
	bool enabled; //FAT Audio
	uint16_t MEM_STEP;
	int subsample_by;
	int SAMPLING_INDEX;
	uint16_t BLOCKS_PER_FFT;
	uint8_t BLOCK_REFRESH_SIZE;
	uint16_t subsample_lowfreqrange;
	uint16_t subsample_highfreqrange;
	subsample_range ssr;
	//uint8_t naverage;
	volatile bool outputflag;
	audio_block_t *inputQueueArray[1];
	arm_cfft_radix4_instance_q15 fft_inst;
};

#endif
