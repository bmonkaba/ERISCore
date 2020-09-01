/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <Arduino.h>
#include "AudioStream.h"

#if defined(__IMXRT1062__)
  #define MAX_AUDIO_MEMORY 229376
#endif

#define NUM_MASKS  (((MAX_AUDIO_MEMORY / AUDIO_BLOCK_SAMPLES / 2) + 31) / 32)

audio_block_t * AudioStream::memory_pool;
uint32_t AudioStream::memory_pool_available_mask[NUM_MASKS];
uint16_t AudioStream::memory_pool_first_mask;

uint16_t AudioStream::cpu_cycles_total = 0;
uint16_t AudioStream::cpu_cycles_total_max = 0;
uint16_t AudioStream::memory_used = 0;
uint16_t AudioStream::memory_used_max = 0;

void software_isr(void);


// Set up the pool of audio data blocks
// placing them all onto the free list
FLASHMEM void AudioStream::initialize_memory(audio_block_t *data, unsigned int num)
{
	unsigned int i;
	unsigned int maxnum = MAX_AUDIO_MEMORY / AUDIO_BLOCK_SAMPLES / 2;

	//Serial.println("AudioStream initialize_memory");
	//delay(10);
	if (num > maxnum) num = maxnum;
	__disable_irq();
	memory_pool = data;
	memory_pool_first_mask = 0;
	for (i=0; i < NUM_MASKS; i++) {
		memory_pool_available_mask[i] = 0;
	}
	for (i=0; i < num; i++) {
		memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
	}
	for (i=0; i < num; i++) {
		data[i].memory_pool_index = i;
	}
	__enable_irq();

}

// Allocate 1 audio data block.  If successful
// the caller is the only owner of this new block
audio_block_t * AudioStream::allocate(void)
{
	uint32_t n, index, avail;
	uint32_t *p, *end;
	audio_block_t *block;
	uint32_t used;

	p = memory_pool_available_mask;
	end = p + NUM_MASKS;
	__disable_irq();
	index = memory_pool_first_mask;
	p += index;
	while (1) {
		if (p >= end) {
			__enable_irq();
			//Serial.println("alloc:null");
			return NULL;
		}
		avail = *p;
		if (avail) break;
		index++;
		p++;
	}
	n = __builtin_clz(avail);
	avail &= ~(0x80000000 >> n);
	*p = avail;
	if (!avail) index++;
	memory_pool_first_mask = index;
	used = memory_used + 1;
	memory_used = used;
	__enable_irq();
	index = p - memory_pool_available_mask;
	block = memory_pool + ((index << 5) + (31 - n));
	block->ref_count = 1;
	if (used > memory_used_max) memory_used_max = used;
	//Serial.print("alloc:");
	//Serial.println((uint32_t)block, HEX);
	return block;
}

// Release ownership of a data block.  If no
// other streams have ownership, the block is
// returned to the free pool
void AudioStream::release(audio_block_t *block)
{
	//if (block == NULL) return;
	uint32_t mask = (0x80000000 >> (31 - (block->memory_pool_index & 0x1F)));
	uint32_t index = block->memory_pool_index >> 5;

	__disable_irq();
	if (block->ref_count > 1) {
		block->ref_count--;
	} else {
		//Serial.print("reles:");
		//Serial.println((uint32_t)block, HEX);
		memory_pool_available_mask[index] |= mask;
		if (index < memory_pool_first_mask) memory_pool_first_mask = index;
		memory_used--;
	}
	__enable_irq();
}

// Transmit an audio data block
// to all streams that connect to an output.  The block
// becomes owned by all the recepients, but also is still
// owned by this object.  Normally, a block must be released
// by the caller after it's transmitted.  This allows the
// caller to transmit to same block to more than 1 output,
// and then release it once after all transmit calls.
void AudioStream::transmit(audio_block_t *block, unsigned char index)
{
	for (AudioConnection *c = destination_list; c != NULL; c = c->next_dest) {
		if (c->src_index == index) {
			if (c->pDst->inputQueue[c->dest_index] == NULL) {
				c->pDst->inputQueue[c->dest_index] = block;
				block->ref_count++;
			}
		}
	}
}


// Receive block from an input.  The block's data
// may be shared with other streams, so it must not be written
audio_block_t * AudioStream::receiveReadOnly(unsigned int index)
{
	audio_block_t *in;

	if (index >= num_inputs) return NULL;
	in = inputQueue[index];
	inputQueue[index] = NULL;
	return in;
}

// Receive block from an input.  The block will not
// be shared, so its contents may be changed.
audio_block_t * AudioStream::receiveWritable(unsigned int index)
{
	audio_block_t *in, *p;

	if (index >= num_inputs) return NULL;
	in = inputQueue[index];
	inputQueue[index] = NULL;
	if (in && in->ref_count > 1) {
		p = allocate();
		if (p) memcpy(p->data, in->data, sizeof(p->data));
		in->ref_count--;
		in = p;
	}
	return in;
}



void AudioConnection::connect(void)
{
	AudioConnection *p;

	if (isConnected) return;
	if (dest_index > pDst->num_inputs) return;
	__disable_irq();
	p = pSrc->destination_list;
	if (p == NULL) {
		pSrc->destination_list = this;
	} else {
		while (p->next_dest) {
			if (p->pSrc == this->pSrc && p->pDst == this->pDst
				&& p->src_index == this->src_index && p->dest_index == this->dest_index) {
				//Source and destination already connected through another connection, abort
				__enable_irq();
				return;
			}
			p = p->next_dest;
		}
		p->next_dest = this;
	}
	this->next_dest = NULL;
	pSrc->numConnections++;
	pSrc->active = true;

	pDst->numConnections++;
	pDst->active = true;

	isConnected = true;

	__enable_irq();
}

bool AudioConnection::disconnect(void)
{
	AudioConnection *p;

	if (!isConnected) return false;
	if (dest_index > pDst->num_inputs) return false;
	Serial.println(F("(eris)AudioConnection:disconnect()"));
	Serial.print(F("\tsrc name:"));Serial.println(pSrc->shortName);
	Serial.print(F("\tsrc ptr:"));Serial.println((uint32_t)pSrc);
	Serial.print(F("\tsrc index:"));Serial.println((uint32_t)src_index);
	Serial.print(F("\tdst name:"));Serial.println(pDst->shortName);
	Serial.print(F("\tdst ptr:"));Serial.println((uint32_t)pDst);
	Serial.print(F("\tdst index:"));Serial.println((uint32_t)dest_index);
	Serial.print(F("\tpSrc->destination_list:"));Serial.println((uint32_t)pSrc->destination_list);
	
	__disable_irq();
	// Remove destination from source list
	p = pSrc->destination_list;
	if (p == NULL) {
//>>> PAH re-enable the IRQ
		__enable_irq();
		return false;
	} else if (p == this) {
		if (p->next_dest) {
			pSrc->destination_list = next_dest;
		} else {
			pSrc->destination_list = NULL;
		}
	} else {
		while (p) {
			if (p == this) {
				if (p->next_dest) {
					p = next_dest;
					break;
				} else {
					p = NULL;
					break;
				}
			}
			p = p->next_dest;
		}
		//Serial.println(F("(eris)AudioConnection:disconnect() complete"));
	}
//>>> PAH release the audio buffer properly
	//Remove possible pending src block from destination
	if(pDst->inputQueue[dest_index] != NULL) {
		AudioStream::release(pDst->inputQueue[dest_index]);
		// release() re-enables the IRQ. Need it to be disabled a little longer
		__disable_irq();
		pDst->inputQueue[dest_index] = NULL;
	}

	//Check if the disconnected AudioStream objects should still be active
	pSrc->numConnections--;
	if (pSrc->numConnections == 0) {
		pSrc->active = false;
	}

	pDst->numConnections--;
	if (pDst->numConnections == 0) {
		pDst->active = false;
	}

	isConnected = false;

	return true;
	__enable_irq();
}


//ADDED TO SUPPORT ERIS CORE
bool AudioConnection::reconnect(){
	Serial.println(F("(eris)AudioConnection:reconnect() connecting by ptr"));
	Serial.print(F("\tsrc name:"));Serial.print(pSrc->shortName);
	Serial.print(F("\tsrc index:"));Serial.print((uint32_t)src_index);
	Serial.print(F("\tsrc instance:"));Serial.print((uint32_t)pSrc->instance);
	Serial.print(F("\tsrc ptr:"));Serial.println((uint32_t)pSrc);

	Serial.print(F("\tdst name:"));Serial.print(pDst->shortName);
	Serial.print(F("\tdst index:"));Serial.print((uint32_t)dest_index);
	Serial.print(F("\tdst instance:"));Serial.print((uint32_t)pDst->instance);
	Serial.print(F("\tdst ptr:"));Serial.print((uint32_t)pDst);
	Serial.print(F("\tpSrc->destination_list ptr:"));Serial.println((uint32_t)pSrc->destination_list);
	AudioConnection *p;

	if (isConnected){
		Serial.println(F("(eris)AudioConnection:reconnect() Warning: Already Connected"));
		return false;
	}
	if (dest_index > pDst->num_inputs){
		Serial.println(F("(eris)AudioConnection:reconnect() Warning: Invalid destination port"));
		return false;
	}
	__disable_irq();
	p = pSrc->destination_list;
	if (p == NULL) {
		//Serial.println("(eris)AudioConnection:reconnect() making first connection");
		pSrc->destination_list = this;
	} else {
		while (p->next_dest) {
			//Serial.println("(eris)AudioConnection:reconnect() adding a connection");
			if (p->pSrc == this->pSrc && p->pDst == this->pDst
				&& p->src_index == this->src_index && p->dest_index == this->dest_index) {
				//Source and destination already connected through another connection, abort
				Serial.println("(eris)AudioConnection:reconnect() Warning: connection already exists");
				__enable_irq();
				return false;
			}
			p = p->next_dest;
		}
		p->next_dest = this;
	}
	this->next_dest = NULL;
	pSrc->numConnections++;
	pSrc->active = true;

	pDst->numConnections++;
	pDst->active = true;

	isConnected = true;

	__enable_irq();
	Serial.println(F("(eris)AudioConnection:reconnect() connection complete"));
	return true;
}


// When an object has taken responsibility for calling update_all()
// at each block interval (approx 2.9ms), this variable is set to
// true.  Objects that are capable of calling update_all(), typically
// input and output based on interrupts, must check this variable in
// their constructors.
bool AudioStream::update_scheduled = false;

bool AudioStream::update_setup(void)
{
	if (update_scheduled) return false;
	attachInterruptVector(IRQ_SOFTWARE, software_isr);
	NVIC_SET_PRIORITY(IRQ_SOFTWARE, 208); // 255 = lowest priority
	NVIC_ENABLE_IRQ(IRQ_SOFTWARE);
	update_scheduled = true;
	return true;
}

void AudioStream::update_stop(void)
{
	NVIC_DISABLE_IRQ(IRQ_SOFTWARE);
	update_scheduled = false;
}

AudioStream * AudioStream::first_update = NULL;

void software_isr(void) // AudioStream::update_all()
{
	AudioStream *p;

	uint32_t totalcycles = ARM_DWT_CYCCNT;
	//digitalWriteFast(2, HIGH);
	for (p = AudioStream::first_update; p; p = p->next_update) {
		if (p->active) {
			uint32_t cycles = ARM_DWT_CYCCNT;
			p->update();
			// TODO: traverse inputQueueArray and release
			// any input blocks that weren't consumed?
			cycles = (ARM_DWT_CYCCNT - cycles) >> 6;
			p->cpu_cycles = cycles;
			if (cycles > p->cpu_cycles_max) p->cpu_cycles_max = cycles;
		}
	}
	//digitalWriteFast(2, LOW);
	totalcycles = (ARM_DWT_CYCCNT - totalcycles) >> 6;
	AudioStream::cpu_cycles_total = totalcycles;
	if (totalcycles > AudioStream::cpu_cycles_total_max)
		AudioStream::cpu_cycles_total_max = totalcycles;

	asm("DSB");
}

