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


/**
 * @file eris_analyze_scope.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright portions Copyright (c) 2021
 * 
 */



#ifndef eris_analyze_scope_h_
#define eris_analyze_scope_h_

#include "Arduino.h"
#include "AudioStream.h"


#define OSCOPE_CAPTURE_LENGTH 320 

class erisAudioAnalyzeScope : public AudioStream
{
public:
	erisAudioAnalyzeScope(void) : AudioStream(2, inputQueueArray),
	  myname(NULL), state(0), trigger_edge(0), delay_length(0), mem_length(OSCOPE_CAPTURE_LENGTH) {
		shortName="scope";
		unum_inputs=2;
		unum_outputs=0;
		category="analyze-function";
		count = 0;
		trigger_level = 0;
		hdivide(5);
		autoTrigger = true;
		isAvailable = false;
		trigger_wait_count = 0;
		edgeCount = 0;
		edgeCount_ch2 = 0;
		peakValue = 0;
		peakValue_output = 0;
		dot=0;
		dotAvg=0;
		dotLast=0;
		dotAvgSlow=0;
		dotDelta = 0;
		dotAccel=0;
		dotDeltaMACD=0;
		dotMACD=0;
		edgeTimer=0;
		edgeDelay=0;
		edgeTimer2=0;
		edgeDelay2=0;

		//buffered outputs
		edgeCount_output = 0;
		edgeCount_ch2_output = 0;
		dot_output=0;
		dotAvg_output=0;
		dotLast_output=0;
		dotAvgSlow_output=0;
		dotDelta_output = 0;
		dotAccel_output=0;
		dotDeltaMACD_output=0;
		dotMACD_output=0;
		edgeDelay_output=0;
		edgeDelay2_output=0;

		memset(memory[0],0,sizeof(memory[0]));
		memset(memory[1],0,sizeof(memory[1]));
		
	}

	virtual void update(void);
	void name(const char *str) { myname = str; }
	void trigger(void);
	void trigger(float level, int edge);
	void delay(uint32_t num) { delay_length = num; }
	void length(uint32_t num) { mem_length = num; }
	int16_t read(int8_t channel, uint16_t mem_index);
	int16_t getPeakValue(){return peakValue_output;}
	bool available(void);
	void hdivide(int8_t horizontal_division){h_div = horizontal_division;auto_h_div = h_div; h_div_count=0;};
	uint8_t getHDiv(){return h_div;}
	int32_t getDotProduct(){return dot_output;}
	int32_t getDotProductAvg(){return dotAvg_output;}
	int32_t getDotProductAvgSlow(){return dotAvgSlow_output;}
	int32_t getDotMACD(){return dotMACD_output;}
	int32_t getDotDelta(){return dotDelta_output;}
	int32_t getDotAcceleration(){return dotAccel_output;}
	int32_t getDotDeltaMACD(){return dotDeltaMACD_output;}
	int32_t getEdgeCount(){return edgeCount_output;}
	int32_t getEdgeCount_ch2(){return edgeCount_ch2_output;}
	int32_t getEdgeDelay(){return edgeDelay_output;}
	int32_t getEdgeDelay2(){return edgeDelay2_output;}
	
private:
	const char *myname;
	uint8_t state;
	int16_t memory[2][OSCOPE_CAPTURE_LENGTH] __attribute__ ((aligned (4)));  //buffered output
	int16_t _memory[2][OSCOPE_CAPTURE_LENGTH] __attribute__ ((aligned (4))); //working memory
	bool autoTrigger;
	bool isAvailable;
	uint8_t h_div;
	int8_t h_div_count;
	int8_t auto_h_div;
	uint8_t trigger_edge; // trigger type, 0=none, 2=RISING, 3=FALLING
	int16_t trigger_level;
	int16_t trigger_wait_count;
	uint32_t delay_length; // number of samples between trigger and sampling
	uint32_t mem_length; // number of samples to capture
	uint32_t edgeCount;
	uint32_t edgeCount_ch2;
	uint32_t count;
	int16_t peakValue;
	uint32_t edgeTimer;
	uint32_t edgeDelay;
	uint32_t edgeTimer2;
	uint32_t edgeDelay2;
	q63_t dot;
	q63_t dotLast;
	int32_t dotAvg;
	int32_t dotAvgSlow;
	int32_t dotDelta;
	int32_t dotAccel;
	int32_t dotDeltaMACD;
	int32_t dotMACD;
	//buffered output
	volatile int16_t peakValue_output;
	volatile uint32_t edgeCount_output;
	volatile uint32_t edgeCount_ch2_output;
	volatile uint32_t edgeDelay_output;
	volatile uint32_t edgeDelay2_output;
	volatile q63_t dot_output;
	volatile q63_t dotLast_output;
	volatile int32_t dotAvg_output;
	volatile int32_t dotAvgSlow_output;
	volatile int32_t dotDelta_output;
	volatile int32_t dotAccel_output;
	volatile int32_t dotDeltaMACD_output;
	volatile int32_t dotMACD_output;

	audio_block_t *inputQueueArray[2];
};

#endif
