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

#ifndef eris_analyze_scope_h_
#define eris_analyze_scope_h_

#include "Arduino.h"
#include "AudioStream.h"

class erisAudioAnalyzeScope : public AudioStream
{
public:
	erisAudioAnalyzeScope(void) : AudioStream(2, inputQueueArray),
	  myname(NULL), state(0), trigger_edge(0), delay_length(0), mem_length(320) {
		shortName="scope";
		unum_inputs=2;
		unum_outputs=0;
		category="analyze-function";
		count = 0;
		trigger_level = 0;
		hdivide(5);
		autoTrigger = false;
		isAvailable = false;
		edgeCount = 0;
		edgeCount_ch2 = 0;
		peakValue = 0;
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
		
	}

	virtual void update(void);
	void name(const char *str) { myname = str; }
	void trigger(void);
	void trigger(float level, int edge);
	void delay(uint32_t num) { delay_length = num; }
	void length(uint32_t num) { mem_length = num; }
	int16_t read(int8_t channel, uint16_t mem_index);
	int16_t getPeakValue(){return peakValue;}
	bool available(void);
	void hdivide(int8_t horizontal_division){h_div = horizontal_division;auto_h_div = h_div; h_div_count=0;};
	uint8_t getHDiv(){return h_div;}
	q63_t getDotProduct(){return dot;}
	q63_t getDotProductAvg(){return dotAvg;}
	q63_t getDotMACD(){return dotMACD;}
	q63_t getDotDelta(){return dotDelta;}
	q63_t getDotAcceleration(){return dotAccel;}
	q63_t getDotDeltaMACD(){return dotDeltaMACD;}
	uint32_t getEdgeCount(){return edgeCount;}
	uint32_t getEdgeCount_ch2(){return edgeCount_ch2;}
	uint32_t getEdgeDelay(){return edgeDelay;}
	uint32_t getEdgeDelay2(){return edgeDelay2;}
	
private:
	const char *myname;
	uint8_t state;
	int16_t memory[2][640] __attribute__ ((aligned (4)));
	bool autoTrigger;
	bool isAvailable;
	uint8_t h_div;
	int8_t h_div_count;
	int8_t auto_h_div;
	uint8_t trigger_edge; // trigger type, 0=none, 2=RISING, 3=FALLING
	int16_t trigger_level;
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
	q63_t dotAvg;
	q63_t dotAvgSlow;
	q63_t dotDelta;
	q63_t dotAccel;
	q63_t dotDeltaMACD;
	q63_t dotMACD;
	audio_block_t *inputQueueArray[2];
};

#endif
