#ifndef __erisAudio
#define __erisAudio

//block the following includes provided by Audio.h
#define play_sd_raw_h_
#define play_sd_wav_h_

#include <Audio.h>
#include <new>

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
		shortName="i2s-in";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AudioInputI2S::update();}
};
class erisAudioInputI2SQuad:public AudioInputI2SQuad{
public:
	erisAudioInputI2SQuad():AudioInputI2SQuad(){
		shortName="i2s_quad";
		unum_inputs=0;
		unum_outputs=4;
		category="input-function";
	}
	void update(){this->AudioInputI2SQuad::update();}
};
class erisAudioInputI2SHex:public AudioInputI2SHex{
public:
	erisAudioInputI2SHex():AudioInputI2SHex(){
		shortName="i2s_hex";
		unum_inputs=0;
		unum_outputs=6;
		category="input-function";
	}
	void update(){this->AudioInputI2SHex::update();}
};
class erisAudioInputI2SOct:public AudioInputI2SOct{
public:
	erisAudioInputI2SOct():AudioInputI2SOct(){
		shortName="i2s_oct";
		unum_inputs=0;
		unum_outputs=8;
		category="input-function";
	}
	void update(){this->AudioInputI2SOct::update();}
};
class erisAudioInputI2Sslave:public AudioInputI2Sslave{
public:
	erisAudioInputI2Sslave():AudioInputI2Sslave(){
		shortName="i2sslave";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AudioInputI2Sslave::update();}
};
class erisAudioInputI2S2:public AudioInputI2S2{
public:
	erisAudioInputI2S2():AudioInputI2S2(){
		shortName="i2s2-in";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AudioInputI2S2::update();}
};
class erisAudioInputSPDIF3:public AudioInputSPDIF3{
public:
	erisAudioInputSPDIF3():AudioInputSPDIF3(){
		shortName="spdif3";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AudioInputSPDIF3::update();}
};
class erisAsyncAudioInputSPDIF3:public AsyncAudioInputSPDIF3{
public:
	erisAsyncAudioInputSPDIF3():AsyncAudioInputSPDIF3(){
		shortName="spdif_async";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AsyncAudioInputSPDIF3::update();}
};
class erisAudioInputAnalog:public AudioInputAnalog{
public:
	erisAudioInputAnalog():AudioInputAnalog(){
		shortName="adc";
		unum_inputs=0;
		unum_outputs=1;
		category="input-function";
	}
	void update(){this->AudioInputAnalog::update();}
};
class erisAudioInputAnalogStereo:public AudioInputAnalogStereo{
public:
	erisAudioInputAnalogStereo():AudioInputAnalogStereo(){
		shortName="adcs";
		unum_inputs=0;
		unum_outputs=2;
		category="input-function";
	}
	void update(){this->AudioInputAnalogStereo::update();}
};
class erisAudioInputPDM:public AudioInputPDM{
public:
	erisAudioInputPDM():AudioInputPDM(){
		shortName="pdm";
		unum_inputs=0;
		unum_outputs=1;
		category="input-function";
	}
	void update(){this->AudioInputPDM::update();}
};
class erisAudioInputTDM:public AudioInputTDM{
public:
	erisAudioInputTDM():AudioInputTDM(){
		shortName="tdm";
		unum_inputs=0;
		unum_outputs=16;
		category="input-function";
	}
	void update(){this->AudioInputTDM::update();}
};
class erisAudioInputTDM2:public AudioInputTDM2{
public:
	erisAudioInputTDM2():AudioInputTDM2(){
		shortName="tdm2";
		unum_inputs=0;
		unum_outputs=16;
		category="input-function";
	}
	void update(){this->AudioInputTDM2::update();}
};
class erisAudioOutputI2S:public AudioOutputI2S{
public:
	erisAudioOutputI2S():AudioOutputI2S(){
		shortName="i2s-out";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2S::update();}
};
class erisAudioOutputI2SQuad:public AudioOutputI2SQuad{
public:
	erisAudioOutputI2SQuad():AudioOutputI2SQuad(){
		shortName="i2s_quad";
		unum_inputs=4;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2SQuad::update();}
};
class erisAudioOutputI2SHex:public AudioOutputI2SHex{
public:
	erisAudioOutputI2SHex():AudioOutputI2SHex(){
		shortName="i2s_hex";
		unum_inputs=6;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2SHex::update();}
};
class erisAudioOutputI2SOct:public AudioOutputI2SOct{
public:
	erisAudioOutputI2SOct():AudioOutputI2SOct(){
		shortName="i2s_oct";
		unum_inputs=8;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2SOct::update();}
};
class erisAudioOutputI2Sslave:public AudioOutputI2Sslave{
public:
	erisAudioOutputI2Sslave():AudioOutputI2Sslave(){
		shortName="i2sslave";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2Sslave::update();}
};
class erisAudioOutputI2S2:public AudioOutputI2S2{
public:
	erisAudioOutputI2S2():AudioOutputI2S2(){
		shortName="i2s2-out";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputI2S2::update();}
};
class erisAudioOutputSPDIF:public AudioOutputSPDIF{
public:
	erisAudioOutputSPDIF():AudioOutputSPDIF(){
		shortName="spdif";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputSPDIF::update();}
};
class erisAudioOutputSPDIF2:public AudioOutputSPDIF2{
public:
	erisAudioOutputSPDIF2():AudioOutputSPDIF2(){
		shortName="spdif2";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputSPDIF2::update();}
};
class erisAudioOutputSPDIF3:public AudioOutputSPDIF3{
public:
	erisAudioOutputSPDIF3():AudioOutputSPDIF3(){
		shortName="spdif3";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputSPDIF3::update();}
};
class erisAudioOutputPT8211:public AudioOutputPT8211{
public:
	erisAudioOutputPT8211():AudioOutputPT8211(){
		shortName="pt8211";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputPT8211::update();}
};
class erisAudioOutputPT8211_2:public AudioOutputPT8211_2{
public:
	erisAudioOutputPT8211_2():AudioOutputPT8211_2(){
		shortName="pt8211_2";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputPT8211_2::update();}
};
class erisAudioOutputAnalog:public AudioOutputAnalog{
public:
	erisAudioOutputAnalog():AudioOutputAnalog(){
		shortName="dac";
		unum_inputs=1;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputAnalog::update();}
};
class erisAudioOutputAnalogStereo:public AudioOutputAnalogStereo{
public:
	erisAudioOutputAnalogStereo():AudioOutputAnalogStereo(){
		shortName="dacs";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputAnalogStereo::update();}
};
class erisAudioOutputPWM:public AudioOutputPWM{
public:
	erisAudioOutputPWM():AudioOutputPWM(){
		shortName="pwm";
		unum_inputs=1;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputPWM::update();}
};
class erisAudioOutputMQS:public AudioOutputMQS{
public:
	erisAudioOutputMQS():AudioOutputMQS(){
		shortName="mqs";
		unum_inputs=2;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputMQS::update();}
};
class erisAudioOutputTDM:public AudioOutputTDM{
public:
	erisAudioOutputTDM():AudioOutputTDM(){
		shortName="tdm";
		unum_inputs=16;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputTDM::update();}
};
class erisAudioOutputTDM2:public AudioOutputTDM2{
public:
	erisAudioOutputTDM2():AudioOutputTDM2(){
		shortName="tdm2";
		unum_inputs=16;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputTDM2::update();}
};
class erisAudioOutputADAT:public AudioOutputADAT{
public:
	erisAudioOutputADAT():AudioOutputADAT(){
		shortName="adat";
		unum_inputs=8;
		unum_outputs=0;
		category="output-function";
	}
	void update(){this->AudioOutputADAT::update();}
};
class erisAudioAmplifier:public AudioAmplifier{
public:
	erisAudioAmplifier():AudioAmplifier(){
		shortName="amp";
		unum_inputs=1;
		unum_outputs=1;
		category="mixer-function";
	}
	void update(){this->AudioAmplifier::update();}
};
class erisAudioMixer4:public AudioMixer4{
public:
	erisAudioMixer4():AudioMixer4(){
		shortName="mixer";
		unum_inputs=4;
		unum_outputs=1;
		category="mixer-function";
	}
	void update(){this->AudioMixer4::update();}
};
class erisAudioPlayMemory:public AudioPlayMemory{
public:
	erisAudioPlayMemory():AudioPlayMemory(){
		shortName="playMem";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	void update(){this->AudioPlayMemory::update();}
};
class erisAudioPlaySerialflashRaw:public AudioPlaySerialflashRaw{
public:
	erisAudioPlaySerialflashRaw():AudioPlaySerialflashRaw(){
		shortName="playFlashRaw";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	void update(){this->AudioPlaySerialflashRaw::update();}
};
class erisAudioPlayQueue:public AudioPlayQueue{
public:
	erisAudioPlayQueue():AudioPlayQueue(){
		shortName="queue";
		unum_inputs=0;
		unum_outputs=1;
		category="play-function";
	}
	void update(){this->AudioPlayQueue::update();}
};
class erisAudioRecordQueue:public AudioRecordQueue{
public:
	erisAudioRecordQueue():AudioRecordQueue(){
		shortName="queue";
		unum_inputs=1;
		unum_outputs=0;
		category="record-function";
	}
	void update(){this->AudioRecordQueue::update();}
};
class erisAudioSynthWavetable:public AudioSynthWavetable{
public:
	erisAudioSynthWavetable():AudioSynthWavetable(){
		shortName="wavetable";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWavetable::update();}
};
class erisAudioSynthSimpleDrum:public AudioSynthSimpleDrum{
public:
	erisAudioSynthSimpleDrum():AudioSynthSimpleDrum(){
		shortName="drum";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthSimpleDrum::update();}
};
class erisAudioSynthKarplusStrong:public AudioSynthKarplusStrong{
public:
	erisAudioSynthKarplusStrong():AudioSynthKarplusStrong(){
		shortName="string";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthKarplusStrong::update();}
};
class erisAudioSynthWaveformSine:public AudioSynthWaveformSine{
public:
	erisAudioSynthWaveformSine():AudioSynthWaveformSine(){
		shortName="sine";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformSine::update();}
};
class erisAudioSynthWaveformSineHires:public AudioSynthWaveformSineHires{
public:
	erisAudioSynthWaveformSineHires():AudioSynthWaveformSineHires(){
		shortName="sine_hires";
		unum_inputs=0;
		unum_outputs=2;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformSineHires::update();}
};
class erisAudioSynthWaveformSineModulated:public AudioSynthWaveformSineModulated{
public:
	erisAudioSynthWaveformSineModulated():AudioSynthWaveformSineModulated(){
		shortName="sine_fm";
		unum_inputs=1;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformSineModulated::update();}
};
class erisAudioSynthWaveform:public AudioSynthWaveform{
public:
	erisAudioSynthWaveform():AudioSynthWaveform(){
		shortName="waveform";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveform::update();}
};
class erisAudioSynthWaveformModulated:public AudioSynthWaveformModulated{
public:
	erisAudioSynthWaveformModulated():AudioSynthWaveformModulated(){
		shortName="waveformMod";
		unum_inputs=2;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformModulated::update();}
};
class erisAudioSynthWaveformPWM:public AudioSynthWaveformPWM{
public:
	erisAudioSynthWaveformPWM():AudioSynthWaveformPWM(){
		shortName="pwm";
		unum_inputs=1;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformPWM::update();}
};
class erisAudioSynthToneSweep:public AudioSynthToneSweep{
public:
	erisAudioSynthToneSweep():AudioSynthToneSweep(){
		shortName="tonesweep";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthToneSweep::update();}
};
class erisAudioSynthWaveformDc:public AudioSynthWaveformDc{
public:
	erisAudioSynthWaveformDc():AudioSynthWaveformDc(){
		shortName="dc";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthWaveformDc::update();}
};
class erisAudioSynthNoiseWhite:public AudioSynthNoiseWhite{
public:
	erisAudioSynthNoiseWhite():AudioSynthNoiseWhite(){
		shortName="noise";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthNoiseWhite::update();}
};
class erisAudioSynthNoisePink:public AudioSynthNoisePink{
public:
	erisAudioSynthNoisePink():AudioSynthNoisePink(){
		shortName="pink";
		unum_inputs=0;
		unum_outputs=1;
		category="synth-function";
	}
	void update(){this->AudioSynthNoisePink::update();}
};
class erisAudioEffectFade:public AudioEffectFade{
public:
	erisAudioEffectFade():AudioEffectFade(){
		shortName="fade";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectFade::update();}
};
class erisAudioEffectChorus:public AudioEffectChorus{
public:
	erisAudioEffectChorus():AudioEffectChorus(){
		shortName="chorus";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectChorus::update();}
};
class erisAudioEffectFlange:public AudioEffectFlange{
public:
	erisAudioEffectFlange():AudioEffectFlange(){
		shortName="flange";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectFlange::update();}
};
class erisAudioEffectReverb:public AudioEffectReverb{
public:
	erisAudioEffectReverb():AudioEffectReverb(){
		shortName="reverb";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectReverb::update();}
};
class erisAudioEffectFreeverb:public AudioEffectFreeverb{
public:
	erisAudioEffectFreeverb():AudioEffectFreeverb(){
		shortName="freeverb";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectFreeverb::update();}
};
class erisAudioEffectFreeverbStereo:public AudioEffectFreeverbStereo{
public:
	erisAudioEffectFreeverbStereo():AudioEffectFreeverbStereo(){
		shortName="freeverbs";
		unum_inputs=1;
		unum_outputs=2;
		category="effect-function";
	}
	void update(){this->AudioEffectFreeverbStereo::update();}
};
class erisAudioEffectEnvelope:public AudioEffectEnvelope{
public:
	erisAudioEffectEnvelope():AudioEffectEnvelope(){
		shortName="envelope";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectEnvelope::update();}
};
class erisAudioEffectMultiply:public AudioEffectMultiply{
public:
	erisAudioEffectMultiply():AudioEffectMultiply(){
		shortName="multiply";
		unum_inputs=2;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectMultiply::update();}
};
class erisAudioEffectDelay:public AudioEffectDelay{
public:
	erisAudioEffectDelay():AudioEffectDelay(){
		shortName="delay";
		unum_inputs=1;
		unum_outputs=8;
		category="effect-function";
	}
	void update(){this->AudioEffectDelay::update();}
};
class erisAudioEffectDelayExternal:public AudioEffectDelayExternal{
public:
	erisAudioEffectDelayExternal():AudioEffectDelayExternal(){
		shortName="delayExt";
		unum_inputs=1;
		unum_outputs=8;
		category="effect-function";
	}
	void update(){this->AudioEffectDelayExternal::update();}
};
class erisAudioEffectBitcrusher:public AudioEffectBitcrusher{
public:
	erisAudioEffectBitcrusher():AudioEffectBitcrusher(){
		shortName="bitcrusher";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectBitcrusher::update();}
};
class erisAudioEffectMidSide:public AudioEffectMidSide{
public:
	erisAudioEffectMidSide():AudioEffectMidSide(){
		shortName="midside";
		unum_inputs=2;
		unum_outputs=2;
		category="effect-function";
	}
	void update(){this->AudioEffectMidSide::update();}
};
class erisAudioEffectWaveshaper:public AudioEffectWaveshaper{
public:
	erisAudioEffectWaveshaper():AudioEffectWaveshaper(){
		shortName="waveshape";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectWaveshaper::update();}
};
class erisAudioEffectGranular:public AudioEffectGranular{
public:
	erisAudioEffectGranular():AudioEffectGranular(){
		shortName="granular";
		unum_inputs=1;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectGranular::update();}
};
class erisAudioEffectDigitalCombine:public AudioEffectDigitalCombine{
public:
	erisAudioEffectDigitalCombine():AudioEffectDigitalCombine(){
		shortName="combine";
		unum_inputs=2;
		unum_outputs=1;
		category="effect-function";
	}
	void update(){this->AudioEffectDigitalCombine::update();}
};
class erisAudioFilterBiquad:public AudioFilterBiquad{
public:
	erisAudioFilterBiquad():AudioFilterBiquad(){
		shortName="biquad";
		unum_inputs=1;
		unum_outputs=1;
		category="filter-function";
	}
	void update(){this->AudioFilterBiquad::update();}
};
class erisAudioFilterFIR:public AudioFilterFIR{
public:
	erisAudioFilterFIR():AudioFilterFIR(){
		shortName="fir";
		unum_inputs=1;
		unum_outputs=1;
		category="filter-function";
	}
	void update(){this->AudioFilterFIR::update();}
};
class erisAudioFilterStateVariable:public AudioFilterStateVariable{
public:
	erisAudioFilterStateVariable():AudioFilterStateVariable(){
		shortName="filter";
		unum_inputs=2;
		unum_outputs=3;
		category="filter-function";
	}
	void update(){this->AudioFilterStateVariable::update();}
};
class erisAudioAnalyzePeak:public AudioAnalyzePeak{
public:
	erisAudioAnalyzePeak():AudioAnalyzePeak(){
		shortName="peak";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzePeak::update();}
};
class erisAudioAnalyzeRMS:public AudioAnalyzeRMS{
public:
	erisAudioAnalyzeRMS():AudioAnalyzeRMS(){
		shortName="rms";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzeRMS::update();}
};
class erisAudioAnalyzeFFT256:public AudioAnalyzeFFT256{
public:
	erisAudioAnalyzeFFT256():AudioAnalyzeFFT256(){
		shortName="fft256";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzeFFT256::update();}
};
class __erisAudioAnalyzeFFT1024:public AudioAnalyzeFFT1024{ //standard libary version not used
public:
	__erisAudioAnalyzeFFT1024():AudioAnalyzeFFT1024(){
		shortName="fft1024";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzeFFT1024::update();}
};
class erisAudioAnalyzeToneDetect:public AudioAnalyzeToneDetect{
public:
	erisAudioAnalyzeToneDetect():AudioAnalyzeToneDetect(){
		shortName="tone";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzeToneDetect::update();}
};
class erisAudioAnalyzeNoteFrequency:public AudioAnalyzeNoteFrequency{
public:
	erisAudioAnalyzeNoteFrequency():AudioAnalyzeNoteFrequency(){
		shortName="notefreq";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzeNoteFrequency::update();}
};
class erisAudioAnalyzePrint:public AudioAnalyzePrint{
public:
	erisAudioAnalyzePrint():AudioAnalyzePrint(){
		shortName="print";
		unum_inputs=1;
		unum_outputs=0;
		category="analyze-function";
	}
	void update(){this->AudioAnalyzePrint::update();}
};

#endif
