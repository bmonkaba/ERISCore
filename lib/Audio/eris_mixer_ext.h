#include "Arduino.h"
#include "AudioStream.h"

#if defined(__ARM_ARCH_7EM__)
#ifndef _erisMIXERext
#define _erisMIXERext
class AudioMixer8 : public AudioStream{
public:
	AudioMixer8(void) : AudioStream(8, inputQueueArray) {
		for (int i=0; i<8; i++) multiplier[i] = 65536;
	}
	virtual void update(void);
	void gain(unsigned int channel, float gain) {
		if (channel >= 8) return;
		if (gain > 32767.0f) gain = 32767.0f;
		else if (gain < -32767.0f) gain = -32767.0f;
		multiplier[channel] = gain * 65536.0f; // TODO: proper roundoff?
	}
private:
	int32_t multiplier[8];
	audio_block_t *inputQueueArray[8];
};
#endif
#endif