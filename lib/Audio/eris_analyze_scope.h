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
		hdivide(1);
	}

	virtual void update(void);
	void name(const char *str) { myname = str; }
	void trigger(void);
	void trigger(float level, int edge);
	void delay(uint32_t num) { delay_length = num; }
	void length(uint32_t num) { mem_length = num; }
	int16_t read(int8_t channel, uint16_t mem_index);
	bool available(void);
	void hdivide(int8_t horizontal_division){h_div = horizontal_division;h_div_count=0;};
private:
	const char *myname;
	uint8_t state;
	int16_t memory[2][320];
	int8_t h_div;
	int8_t h_div_count;
	uint8_t trigger_edge; // trigger type, 0=none, 2=RISING, 3=FALLING
	int16_t trigger_level;
	uint32_t delay_length; // number of samples between trigger and sampling
	uint32_t mem_length; // number of samples to capture
	uint32_t count;
	audio_block_t *inputQueueArray[2];
};

#endif
