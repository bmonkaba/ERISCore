/**
 * @file erisAudio.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief audio block class extentions (note: autogenerated code)  
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __erisAudio
#define __erisAudio


//block the following includes provided by Audio.h
#define play_sd_raw_h_
#define play_sd_wav_h_
//a modified synth waveform header is contained in the eris library
//block the import from audio.h

#include "eris_synth_waveform.h"
#include "eris_analyze_fft1024.h"
#include "eris_analyze_scope.h"

#include "Audio.h"
#include "AudioStream.h"

#include "eris_mixer_ext.h"

//now load the modified version
//#undef synth_waveform_h_
#include <new>






class erisAudioMixer8:public AudioMixer8{
public:
	erisAudioMixer8():AudioMixer8(){
		short_name="console";
		unum_inputs=8;
		unum_outputs=1;
		category="mixer-function";
	}
	const char* short_name_lookup = "console";
	void update(){this->AudioMixer8::update();}
};



// handcode
//
/*
class erisAudioStream:virtual public AudioStream{
public:
	erisAudioStream(void) : AudioStream() {  };
	const char* shortName;
	const char* category;
	uint8_t unum_inputs;
    uint8_t unum_outputs;
	uint8_t instance;
protected:
	//friend class AudioConnection;
};
*/
/*
class erisAudioConnection:public AudioConnection
{
	public:
		erisAudioConnection(erisAudioStream *psource, unsigned char sourceOutput, erisAudioStream *pdestination, unsigned char destinationInput): AudioConnection(psource, sourceOutput, pdestination, destinationInput){
		};

		void rewire(erisAudioStream* psource, unsigned char sourceOutput,erisAudioStream* pdestination, unsigned char destinationInput){
			//disconnect();
			pSrc = psource;
			pDst = pdestination;
			src_index = sourceOutput;
			dest_index = destinationInput;
			reconnect();
		};
		void reconnect();

		friend class erisAudioStream;
		~erisAudioConnection();				
	protected:

};
*/



///
// end hand code
///

// Wrapped classes (automated)

class erisAudioInputI2S:public AudioInputI2S{
public:
	erisAudioInputI2S():AudioInputI2S(){
		short_name= short_name_lookup;
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2S::update();}
};


class erisAudioInputI2SQuad:public AudioInputI2SQuad{
public:
	erisAudioInputI2SQuad():AudioInputI2SQuad(){
		short_name=short_name_lookup;
		unum_inputs=0;
		unum_outputs=4;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2SQuad::update();}
};


class erisAudioInputI2SHex:public AudioInputI2SHex{
public:
	erisAudioInputI2SHex():AudioInputI2SHex(){
		short_name="i2s_hex";
		unum_inputs=0;
		unum_outputs=6;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2SHex::update();}
};


class erisAudioInputI2SOct:public AudioInputI2SOct{
public:
	erisAudioInputI2SOct():AudioInputI2SOct(){
		short_name="i2s_oct";
		unum_inputs=0;
		unum_outputs=8;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2SOct::update();}
};


class erisAudioInputI2Sslave:public AudioInputI2Sslave{
public:
	erisAudioInputI2Sslave():AudioInputI2Sslave(){
		short_name="i2sslave";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2Sslave::update();}
};


class erisAudioInputI2S2:public AudioInputI2S2{
public:
	erisAudioInputI2S2():AudioInputI2S2(){
		short_name="i2s2-in";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputI2S2::update();}
};

/*
class erisAudioInputSPDIF3:public AudioInputSPDIF3{
public:
	erisAudioInputSPDIF3():AudioInputSPDIF3(){
		short_name="spdif3";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputSPDIF3::update();}
};


class erisAsyncAudioInputSPDIF3:public AsyncAudioInputSPDIF3{
public:
	erisAsyncAudioInputSPDIF3():AsyncAudioInputSPDIF3(){
		short_name="spdif_async";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AsyncAudioInputSPDIF3::update();}
};


class erisAudioInputAnalog:public AudioInputAnalog{
public:
	erisAudioInputAnalog():AudioInputAnalog(){
		short_name="adc";
		unum_inputs=0;
		unum_outputs=1;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputAnalog::update();}
};


class erisAudioInputAnalogStereo:public AudioInputAnalogStereo{
public:
	erisAudioInputAnalogStereo():AudioInputAnalogStereo(){
		short_name="adcs";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputAnalogStereo::update();}
};


class erisAudioInputPDM:public AudioInputPDM{
public:
	erisAudioInputPDM():AudioInputPDM(){
		short_name="pdm";
		unum_inputs=0;
		unum_outputs=1;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputPDM::update();}
};


class erisAudioInputTDM:public AudioInputTDM{
public:
	erisAudioInputTDM():AudioInputTDM(){
		short_name="tdm";
		unum_inputs=0;
		unum_outputs=16;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputTDM::update();}
};


class erisAudioInputTDM2:public AudioInputTDM2{
public:
	erisAudioInputTDM2():AudioInputTDM2(){
		short_name="tdm2";
		unum_inputs=0;
		unum_outputs=16;
		category="input-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioInputTDM2::update();}
};
*/


class erisAudioOutputI2S:public AudioOutputI2S{
public:
	erisAudioOutputI2S():AudioOutputI2S(){
		short_name="i2s-out";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2S::update();}
};


class erisAudioOutputI2SQuad:public AudioOutputI2SQuad{
public:
	erisAudioOutputI2SQuad():AudioOutputI2SQuad(){
		short_name="i2s_quad";
		unum_inputs=4;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2SQuad::update();}
};


class erisAudioOutputI2SHex:public AudioOutputI2SHex{
public:
	erisAudioOutputI2SHex():AudioOutputI2SHex(){
		short_name="i2s_hex";
		unum_inputs=6;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2SHex::update();}
};


class erisAudioOutputI2SOct:public AudioOutputI2SOct{
public:
	erisAudioOutputI2SOct():AudioOutputI2SOct(){
		short_name="i2s_oct";
		unum_inputs=8;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2SOct::update();}
};


class erisAudioOutputI2Sslave:public AudioOutputI2Sslave{
public:
	erisAudioOutputI2Sslave():AudioOutputI2Sslave(){
		short_name="i2sslave";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2Sslave::update();}
};


class erisAudioOutputI2S2:public AudioOutputI2S2{
public:
	erisAudioOutputI2S2():AudioOutputI2S2(){
		short_name="i2s2-out";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputI2S2::update();}
};

/*
class erisAudioOutputSPDIF:public AudioOutputSPDIF{
public:
	erisAudioOutputSPDIF():AudioOutputSPDIF(){
		short_name="spdif";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputSPDIF::update();}
};


class erisAudioOutputSPDIF2:public AudioOutputSPDIF2{
public:
	erisAudioOutputSPDIF2():AudioOutputSPDIF2(){
		short_name="spdif2";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputSPDIF2::update();}
};


class erisAudioOutputSPDIF3:public AudioOutputSPDIF3{
public:
	erisAudioOutputSPDIF3():AudioOutputSPDIF3(){
		short_name="spdif3";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputSPDIF3::update();}
};


class erisAudioOutputPT8211:public AudioOutputPT8211{
public:
	erisAudioOutputPT8211():AudioOutputPT8211(){
		short_name="pt8211";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputPT8211::update();}
};


class erisAudioOutputPT8211_2:public AudioOutputPT8211_2{
public:
	erisAudioOutputPT8211_2():AudioOutputPT8211_2(){
		short_name="pt8211_2";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputPT8211_2::update();}
};


class erisAudioOutputAnalog:public AudioOutputAnalog{
public:
	erisAudioOutputAnalog():AudioOutputAnalog(){
		short_name="dac";
		unum_inputs=1;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputAnalog::update();}
};


class erisAudioOutputAnalogStereo:public AudioOutputAnalogStereo{
public:
	erisAudioOutputAnalogStereo():AudioOutputAnalogStereo(){
		short_name="dacs";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputAnalogStereo::update();}
};


class erisAudioOutputPWM:public AudioOutputPWM{
public:
	erisAudioOutputPWM():AudioOutputPWM(){
		short_name="pwm";
		unum_inputs=1;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputPWM::update();}
};


class erisAudioOutputMQS:public AudioOutputMQS{
public:
	erisAudioOutputMQS():AudioOutputMQS(){
		short_name="mqs";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputMQS::update();}
};


class erisAudioOutputTDM:public AudioOutputTDM{
public:
	erisAudioOutputTDM():AudioOutputTDM(){
		short_name="tdm";
		unum_inputs=16;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputTDM::update();}
};

class erisAudioOutputTDM2:public AudioOutputTDM2{
public:
	erisAudioOutputTDM2():AudioOutputTDM2(){
		short_name="tdm2";
		unum_inputs=16;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputTDM2::update();}
};


class erisAudioOutputADAT:public AudioOutputADAT{
public:
	erisAudioOutputADAT():AudioOutputADAT(){
		short_name="adat";
		unum_inputs=8;
		unum_outputs=0;
		category="output-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioOutputADAT::update();}
};
*/

class erisAudioAmplifier:public AudioAmplifier{
public:
	erisAudioAmplifier():AudioAmplifier(){
		short_name="amp";
		unum_inputs=1;
		unum_outputs=1;
		category="mixer-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAmplifier::update();}
};


class erisAudioMixer4:public AudioMixer4{
public:
	erisAudioMixer4():AudioMixer4(){
		short_name="mixer";
		unum_inputs=4;
		unum_outputs=1;
		category="mixer-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioMixer4::update();}
};


class erisAudioPlayMemory:public AudioPlayMemory{
public:
	erisAudioPlayMemory():AudioPlayMemory(){
		short_name="playMem";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioPlayMemory::update();}
};


class erisAudioPlaySerialflashRaw:public AudioPlaySerialflashRaw{
public:
	erisAudioPlaySerialflashRaw():AudioPlaySerialflashRaw(){
		short_name="playFlashRaw";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioPlaySerialflashRaw::update();}
};


class erisAudioPlayQueue:public AudioPlayQueue{
public:
	erisAudioPlayQueue():AudioPlayQueue(){
		short_name="play_queue";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioPlayQueue::update();}
};


class erisAudioRecordQueue:public AudioRecordQueue{
public:
	erisAudioRecordQueue():AudioRecordQueue(){
		short_name="record_queue";
		unum_inputs=1;
		unum_outputs=0;
		category="record-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioRecordQueue::update();}
};


class erisAudioSynthWavetable:public AudioSynthWavetable{
public:
	erisAudioSynthWavetable():AudioSynthWavetable(){
		short_name="wavetable";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWavetable::update();}
};


class erisAudioSynthSimpleDrum:public AudioSynthSimpleDrum{
public:
	erisAudioSynthSimpleDrum():AudioSynthSimpleDrum(){
		short_name="drum";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthSimpleDrum::update();}
};


class erisAudioSynthKarplusStrong:public AudioSynthKarplusStrong{
public:
	erisAudioSynthKarplusStrong():AudioSynthKarplusStrong(){
		short_name="string";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthKarplusStrong::update();}
};


class erisAudioSynthWaveformSine:public AudioSynthWaveformSine{
public:
	erisAudioSynthWaveformSine():AudioSynthWaveformSine(){
		short_name="sine";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformSine::update();}
};


class erisAudioSynthWaveformSineHires:public AudioSynthWaveformSineHires{
public:
	erisAudioSynthWaveformSineHires():AudioSynthWaveformSineHires(){
		short_name="sine_hires";
		unum_inputs=0;
		unum_outputs=2;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformSineHires::update();}
};


class erisAudioSynthWaveformSineModulated:public AudioSynthWaveformSineModulated{
public:
	erisAudioSynthWaveformSineModulated():AudioSynthWaveformSineModulated(){
		short_name="sine_fm";
		unum_inputs=1;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformSineModulated::update();}
};


//standard oscillators silently upgraded to support 2K arb waveforms
class erisAudioSynthWaveform:public AudioSynthWaveform2k{
public:
	erisAudioSynthWaveform():AudioSynthWaveform2k(){
		short_name="waveform";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveform2k::update();}
};


class erisAudioSynthWaveformModulated:public AudioSynthWaveformModulated2k{
public:
	erisAudioSynthWaveformModulated():AudioSynthWaveformModulated2k(){
		short_name="waveformMod";
		unum_inputs=2;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformModulated2k::update();}
};

class erisAudioSynthWaveform2k:public AudioSynthWaveform2k{
public:
	erisAudioSynthWaveform2k():AudioSynthWaveform2k(){
		short_name="waveform2k";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveform2k::update();}
};


class erisAudioSynthWaveformModulated2k:public AudioSynthWaveformModulated2k{
public:
	erisAudioSynthWaveformModulated2k():AudioSynthWaveformModulated2k(){
		short_name="waveformMod2K";
		unum_inputs=2;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformModulated2k::update();}
};

class erisAudioSynthWaveformPWM:public AudioSynthWaveformPWM{
public:
	erisAudioSynthWaveformPWM():AudioSynthWaveformPWM(){
		short_name="pwm";
		unum_inputs=1;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformPWM::update();}
};


class erisAudioSynthToneSweep:public AudioSynthToneSweep{
public:
	erisAudioSynthToneSweep():AudioSynthToneSweep(){
		short_name="tonesweep";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthToneSweep::update();}
};


class erisAudioSynthWaveformDc:public AudioSynthWaveformDc{
public:
	erisAudioSynthWaveformDc():AudioSynthWaveformDc(){
		short_name="dc";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthWaveformDc::update();}
};


class erisAudioSynthNoiseWhite:public AudioSynthNoiseWhite{
public:
	erisAudioSynthNoiseWhite():AudioSynthNoiseWhite(){
		short_name="noise";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthNoiseWhite::update();}
};


class erisAudioSynthNoisePink:public AudioSynthNoisePink{
public:
	erisAudioSynthNoisePink():AudioSynthNoisePink(){
		short_name="pink";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioSynthNoisePink::update();}
};


class erisAudioEffectFade:public AudioEffectFade{
public:
	erisAudioEffectFade():AudioEffectFade(){
		short_name="fade";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectFade::update();}
};


class erisAudioEffectChorus:public AudioEffectChorus{
public:
	erisAudioEffectChorus():AudioEffectChorus(){
		short_name="chorus";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectChorus::update();}
};


class erisAudioEffectFlange:public AudioEffectFlange{
public:
	erisAudioEffectFlange():AudioEffectFlange(){
		short_name="flange";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectFlange::update();}
};


class erisAudioEffectReverb:public AudioEffectReverb{
public:
	erisAudioEffectReverb():AudioEffectReverb(){
		short_name="reverb";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectReverb::update();}
};


class erisAudioEffectFreeverb:public AudioEffectFreeverb{
public:
	erisAudioEffectFreeverb():AudioEffectFreeverb(){
		short_name="freeverb";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectFreeverb::update();}
};


class erisAudioEffectFreeverbStereo:public AudioEffectFreeverbStereo{
public:
	erisAudioEffectFreeverbStereo():AudioEffectFreeverbStereo(){
		short_name="freeverbs";
		unum_inputs=1;
		unum_outputs=2;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectFreeverbStereo::update();}
};


class erisAudioEffectEnvelope:public AudioEffectEnvelope{
public:
	erisAudioEffectEnvelope():AudioEffectEnvelope(){
		short_name="envelope";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectEnvelope::update();}
};


class erisAudioEffectMultiply:public AudioEffectMultiply{
public:
	erisAudioEffectMultiply():AudioEffectMultiply(){
		short_name="multiply";
		unum_inputs=2;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectMultiply::update();}
};


class erisAudioEffectDelay:public AudioEffectDelay{
public:
	erisAudioEffectDelay():AudioEffectDelay(){
		short_name="delay";
		unum_inputs=1;
		unum_outputs=8;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectDelay::update();}
};


class erisAudioEffectDelayExternal:public AudioEffectDelayExternal{
public:
	erisAudioEffectDelayExternal():AudioEffectDelayExternal(){
		short_name="delayExt";
		unum_inputs=1;
		unum_outputs=8;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectDelayExternal::update();}
};


class erisAudioEffectBitcrusher:public AudioEffectBitcrusher{
public:
	erisAudioEffectBitcrusher():AudioEffectBitcrusher(){
		short_name="bitcrusher";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectBitcrusher::update();}
};


class erisAudioEffectMidSide:public AudioEffectMidSide{
public:
	erisAudioEffectMidSide():AudioEffectMidSide(){
		short_name="midside";
		unum_inputs=2;
		unum_outputs=2;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectMidSide::update();}
};


class erisAudioEffectWaveshaper:public AudioEffectWaveshaper{
public:
	erisAudioEffectWaveshaper():AudioEffectWaveshaper(){
		short_name="waveshape";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectWaveshaper::update();}
};


class erisAudioEffectGranular:public AudioEffectGranular{
public:
	erisAudioEffectGranular():AudioEffectGranular(){
		short_name="granular";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectGranular::update();}
};


class erisAudioEffectDigitalCombine:public AudioEffectDigitalCombine{
public:
	erisAudioEffectDigitalCombine():AudioEffectDigitalCombine(){
		short_name="combine";
		unum_inputs=2;
		unum_outputs=1;
		category="effect-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioEffectDigitalCombine::update();}
};


class erisAudioFilterBiquad:public AudioFilterBiquad{
public:
	erisAudioFilterBiquad():AudioFilterBiquad(){
		short_name="biquad";
		unum_inputs=1;
		unum_outputs=1;
		category="filter-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioFilterBiquad::update();}
};


class erisAudioFilterFIR:public AudioFilterFIR{
public:
	erisAudioFilterFIR():AudioFilterFIR(){
		short_name="fir";
		unum_inputs=1;
		unum_outputs=1;
		category="filter-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioFilterFIR::update();}
};


class erisAudioFilterStateVariable:public AudioFilterStateVariable{
public:
	erisAudioFilterStateVariable():AudioFilterStateVariable(){
		short_name="filter";
		unum_inputs=2;
		unum_outputs=3;
		category="filter-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioFilterStateVariable::update();}
};


class erisAudioAnalyzePeak:public AudioAnalyzePeak{
public:
	erisAudioAnalyzePeak():AudioAnalyzePeak(){
		short_name="peak";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzePeak::update();}
};

class erisAudioAnalyzeRMS:public AudioAnalyzeRMS{
public:
	erisAudioAnalyzeRMS():AudioAnalyzeRMS(){
		short_name="rms";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzeRMS::update();}
};


class erisAudioAnalyzeFFT256:public AudioAnalyzeFFT256{
public:
	erisAudioAnalyzeFFT256():AudioAnalyzeFFT256(){
		short_name="fft256";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzeFFT256::update();}
};


class __erisAudioAnalyzeFFT1024:public AudioAnalyzeFFT1024{ //standard libary version not used
public:
	__erisAudioAnalyzeFFT1024():AudioAnalyzeFFT1024(){
		short_name="fft1024";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzeFFT1024::update();}
};


class erisAudioAnalyzeToneDetect:public AudioAnalyzeToneDetect{
public:
	erisAudioAnalyzeToneDetect():AudioAnalyzeToneDetect(){
		short_name="tone";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzeToneDetect::update();}
};


class erisAudioAnalyzeNoteFrequency:public AudioAnalyzeNoteFrequency{
public:
	erisAudioAnalyzeNoteFrequency():AudioAnalyzeNoteFrequency(){
		short_name="notefreq";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzeNoteFrequency::update();}
};

/*
class erisAudioAnalyzePrint:public AudioAnalyzePrint{
public:
	erisAudioAnalyzePrint():AudioAnalyzePrint(){
		short_name="print";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	static const char* short_name_lookup;
	void update(){this->AudioAnalyzePrint::update();}
};
*/

#endif
