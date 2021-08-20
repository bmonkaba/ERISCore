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
#include <arm_math.h>
#include "eris_analyze_scope.h"


#define STATE_IDLE          0  // doing nothing
#define STATE_WAIT_TRIGGER  1  // looking for trigger condition
#define STATE_DELAY         2  // waiting from trigger to print
#define STATE_PRINTING      3  // printing data

void erisAudioAnalyzeScope::update(void)
{
	audio_block_t *block;
	audio_block_t *blockb; //2nd channel (optional)
	bool isDualChannel;

	uint32_t offset;
	uint32_t remain;

	//capture only if channel 0 is connected
	block = receiveReadOnly(0);
	if (!block){
		blockb = receiveReadOnly(1); 
		if (blockb) release(blockb);
		return;
	}
	
	blockb = receiveReadOnly(1);
	if (!blockb) {
		isDualChannel = false;
	} else isDualChannel = true;

	offset = 0;
	while (offset < AUDIO_BLOCK_SAMPLES) {
		remain = AUDIO_BLOCK_SAMPLES - offset;
		switch (state) {
		  case STATE_WAIT_TRIGGER:
			// TODO: implement this....
			offset = AUDIO_BLOCK_SAMPLES;
			break;

		  case STATE_DELAY:
			//Serial.printf("STATE_DELAY, count = %u\n", count);
			if (remain < count) {
				count -= remain;
				offset = AUDIO_BLOCK_SAMPLES;
			} else {
				offset += count;
				count = mem_length;
				state = STATE_PRINTING;
			}
			break;

		  case STATE_PRINTING:
		  	//wait for the zero crossing on ch1 
			if (count == mem_length){ 
				dot = 0;
				dotAvg = 0;
				dotAvgSlow = 0;
				dotDelta = 0;
				dotDeltaMACD = 0;
				dotAccel = 0;
				dotMACD = 0;
				edgeCount = 0;
				edgeCount_ch2 = 0;
				peakValue = 0;
				edgeTimer=0;
				edgeDelay=0;
				edgeTimer2=0;
				edgeDelay2=0;				

				bool found = false;
				for(int16_t i=1;i < AUDIO_BLOCK_SAMPLES-h_div;i++){
					if (block->data[i] > 0 && block->data[i-1] < 0){
						offset = i;
						found = true;
						edgeTimer=0;
						edgeTimer2=0;
						break;
					}
				}
				//skip to the next block
				if(!found) {offset = AUDIO_BLOCK_SAMPLES;};

			}
			
			while ((offset < AUDIO_BLOCK_SAMPLES) && (count > 0)){
				edgeTimer++;
				edgeTimer2++;
				//for every sample
				if ((offset >= h_div) && block->data[offset] <= 0 && block->data[offset-h_div] >0){
					if(edgeCount > 0){
						if (edgeDelay==0){edgeDelay = edgeTimer;
						} else {
							edgeDelay = max(edgeDelay, edgeTimer);
						}
					}
					edgeTimer=0;
				} 
				if (isDualChannel && (offset >= h_div) && blockb->data[offset] <= 0 && blockb->data[offset-h_div] >0){
					if(edgeCount_ch2 > 0){
						if (edgeDelay2==0){edgeDelay2 = edgeTimer2;
						} else {
							edgeDelay2 = max(edgeDelay2,edgeTimer2);
						}
					}
					edgeTimer2=0;
				} 

				h_div_count++;
				//for the captured samples
				if(h_div_count==h_div){
					h_div_count=0;
					count--;
					memory[0][mem_length - count - 1 ] = block->data[offset];
					peakValue = max(peakValue,abs(block->data[offset]));
					if (isDualChannel){ 
						memory[1][mem_length - count - 1 ] = blockb->data[offset];
						peakValue = max(peakValue,abs(blockb->data[offset]));
						if ((offset >= h_div) && (blockb->data[offset] <= 0 ) && (blockb->data[offset-h_div] > 0)){
							edgeCount_ch2++;
						}
					}else memory[1][mem_length - count - 1 ] = 0;

					if ((offset >= h_div) && (block->data[offset] <= 0 ) && (block->data[offset-h_div] > 0)){
						edgeCount++;
					}
				}
				offset++;
			}
			
			
			if (count == 0){
				//Serial.println(edgeCount);
				if((edgeCount < 6) && (auto_h_div < 64)) auto_h_div++;
				if((edgeCount > 8) && (auto_h_div > 3)) auto_h_div--;
				//calculate the dot product if dual channel
				if (isDualChannel){
					q63_t lastDelta;
					lastDelta= dotDelta;
					dotLast = dot;			
					arm_dot_prod_q15(&memory[0][0],&memory[1][0],mem_length,&dot);
					dotDelta = ((dot/2) - (dotLast/2)) * 2;
					dotAccel = ((dotDelta/2)-(lastDelta/2)) * 2;		
					dotAvg = ((dotAvg*0.75) + (dot*0.25));
					dotAvgSlow = (dotAvg*0.95) + (dot*0.05);
					dotMACD = ((dotAvg/2) - (dotAvgSlow/2)) * 2;
					//(dotMACD>0)?:dotMACD*=-1;
					dotDeltaMACD = ((dotMACD/2) - (dotDeltaMACD/2)) * 2;
					
				}	
				isAvailable = true;
				if (autoTrigger){
					trigger();
				}else state = STATE_IDLE;
			}
			break;

		  default: // STATE_IDLE
			offset = AUDIO_BLOCK_SAMPLES;
			break;
		}
	}
	release(block);
	if(isDualChannel) release(blockb);
}


int16_t erisAudioAnalyzeScope::read(int8_t channel, uint16_t mem_index){
	if (mem_index > mem_length) return 0;
	return memory[channel][mem_index];
}

void erisAudioAnalyzeScope::trigger(void)
{
	uint32_t n = delay_length;

	h_div = auto_h_div;

	if (n > 0) {
		count = n;
		state = 2;
	} else {
		count = mem_length;
		state = STATE_PRINTING;
	}
}

bool erisAudioAnalyzeScope::available(){
	if (isAvailable){
		isAvailable = false;
		return true;
	} 
	return false;
}

