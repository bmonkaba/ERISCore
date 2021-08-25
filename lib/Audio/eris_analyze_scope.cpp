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
 * @file eris_analyze_scope.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright portions Copyright (c) 2021
 * 
 */


#include <Arduino.h>
#include <arm_math.h>
#include "eris_analyze_scope.h"


#define SCOPE_MAX_TRIGGER_WAIT	800  

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
					if(trigger_wait_count > SCOPE_MAX_TRIGGER_WAIT){
						//force the trigger
						found=true;
						edgeTimer=0;
						edgeTimer2=0;
						break;
					}
					if (block->data[i] > 0 && block->data[i-1] < 0){
						offset = i;
						if(isDualChannel){
							if (blockb->data[i] > 0){
								found = true;
							}
						}else found = true;
						if (found){
							edgeTimer=0;
							edgeTimer2=0;
							break;
						}
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
					if((edgeCount > 0) && (edgeTimer > 20)){
						if (edgeDelay==0){edgeDelay = edgeTimer;
						} else {
							edgeDelay = (edgeDelay+edgeTimer)/2;
						}
					}
					edgeTimer=0;
				} 
				if (isDualChannel && (offset >= h_div) && blockb->data[offset] <= 0 && blockb->data[offset-h_div] >0){
					if((edgeCount_ch2 > 0) && (edgeTimer2 > 20)){
						if (edgeDelay2==0){edgeDelay2 = edgeTimer2;
						} else {
							edgeDelay2 = (edgeDelay2 + edgeTimer2)/2;
						}
					}
					edgeTimer2=0;
				} 

				h_div_count++;
				//for the captured samples
				if(h_div_count==h_div){
					h_div_count=0;
					count--;
					_memory[0][mem_length - count - 1 ] = block->data[offset];
					peakValue = max(peakValue,abs(block->data[offset]));
					if (isDualChannel){ 
						_memory[1][mem_length - count - 1 ] = blockb->data[offset];
						peakValue = max(peakValue,abs(blockb->data[offset]));
						if ((offset >= h_div) && (blockb->data[offset] <= -10 ) && (blockb->data[offset-h_div] > 10)){
							edgeCount_ch2++;
						}
					}else _memory[1][mem_length - count - 1 ] = 0;

					if ((offset >= h_div) && (block->data[offset] <= -10 ) && (block->data[offset-h_div] > 10)){
						edgeCount++;
					}
				}
				offset++;
			}
			
			
			if (count == 0){
				//Serial.println(edgeCount);
					//only update the values if data is available
					if(peakValue >0){
						if((edgeCount < 8) && (auto_h_div < 16)) auto_h_div++;
						if((edgeCount > 11) && (auto_h_div > 1)) auto_h_div--;
						//calculate the dot product if dual channel
						if (isDualChannel){
							q63_t lastDelta;
							lastDelta= dotDelta;
							dotLast = dot/1000000;	
							arm_dot_prod_q15(&_memory[0][0],&_memory[1][0],mem_length,&dot);
							dot = dot /1000000;
							dotDelta = ((dot) - (dotLast));
							dotAccel = ((dotDelta)-(lastDelta));		
							dotAvg = (dotAvg/2) + (dot/2);
							dotAvgSlow = (dotAvg/2) + (dotAvgSlow/2);
							dotMACD = ((dotAvg) - (dotAvgSlow));
							//(dotMACD>0)?:dotMACD*=-1;
							dotDeltaMACD = ((dotMACD) - (dotDeltaMACD));
							memcpy(&memory[1],&_memory[1],sizeof(_memory[1]));
						}
						//buffered outputs
						memcpy(&memory[0],&_memory[0],sizeof(_memory[0]));
						edgeCount_output = edgeCount;
						edgeCount_ch2_output = edgeCount_ch2;
						dot_output=dot;
						dotAvg_output=dotAvg;
						dotLast_output=dotLast;
						dotAvgSlow_output=dotAvgSlow;
						dotDelta_output = dotDelta;
						dotAccel_output=dotAccel;
						dotDeltaMACD_output=dotDeltaMACD;
						dotMACD_output=dotMACD;
						edgeDelay_output=edgeDelay;
						edgeDelay2_output=edgeDelay2;
						peakValue_output = peakValue;
					}
				isAvailable = true;
				if (autoTrigger){
					trigger();
				}else state = STATE_IDLE;
				trigger_wait_count = 0;
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
	h_div = auto_h_div;
	if (count ==0){ //ignore external trigger requests if already capturing
		if (delay_length > 0) {
			count = delay_length;
			state = STATE_DELAY;
		} else {
			count = mem_length;
			state = STATE_PRINTING;
		}
	}
}

bool erisAudioAnalyzeScope::available(){
	if (isAvailable){
		isAvailable = false;
		trigger();
		return true;
	} 
	return false;
}

