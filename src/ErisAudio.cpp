#include "ErisAudio.h"

const char* erisAudioInputI2S::short_name_lookup PROGMEM = "i2s-in";
const char* erisAudioInputI2SQuad::short_name_lookup PROGMEM = "i2s_quad";
const char* erisAudioInputI2SHex::short_name_lookup PROGMEM = "i2s_hex";
const char* erisAudioInputI2SOct::short_name_lookup PROGMEM = "i2s_oct";
const char* erisAudioInputI2Sslave::short_name_lookup PROGMEM = "i2sslave";
const char* erisAudioInputI2S2::short_name_lookup PROGMEM = "i2s2-in";
/*
const char* erisAudioInputSPDIF3::short_name_lookup PROGMEM = "spdif3";
const char* erisAsyncAudioInputSPDIF3::short_name_lookup PROGMEM = "spdif_async";
const char* erisAudioInputAnalog::short_name_lookup PROGMEM = "adc";
const char* erisAudioInputAnalogStereo::short_name_lookup PROGMEM = "adcs";
const char* erisAudioInputPDM::short_name_lookup PROGMEM = "pdm";
const char* erisAudioInputTDM::short_name_lookup PROGMEM = "tdm";
const char* erisAudioInputTDM2::short_name_lookup PROGMEM = "tdm2";
*/
const char* erisAudioOutputI2S::short_name_lookup PROGMEM = "i2s-out";
const char* erisAudioOutputI2SQuad::short_name_lookup PROGMEM = "i2s_quad";
const char* erisAudioOutputI2SHex::short_name_lookup PROGMEM = "i2s_hex";
const char* erisAudioOutputI2SOct::short_name_lookup PROGMEM = "i2s_oct";
const char* erisAudioOutputI2Sslave::short_name_lookup PROGMEM = "i2sslave";
const char* erisAudioOutputI2S2::short_name_lookup PROGMEM = "i2s2-out";
/*
const char* erisAudioOutputSPDIF::short_name_lookup PROGMEM = "spdif";
const char* erisAudioOutputSPDIF2::short_name_lookup PROGMEM = "spdif2";
const char* erisAudioOutputSPDIF3::short_name_lookup PROGMEM = "spdif3";
const char* erisAudioOutputPT8211::short_name_lookup PROGMEM = "pt8211";
const char* erisAudioOutputPT8211_2::short_name_lookup PROGMEM = "pt8211_2";
const char* erisAudioOutputAnalog::short_name_lookup PROGMEM = "dac";
const char* erisAudioOutputAnalogStereo::short_name_lookup PROGMEM = "dacs";
const char* erisAudioOutputPWM::short_name_lookup PROGMEM = "pwm";
const char* erisAudioOutputMQS::short_name_lookup PROGMEM = "mqs";
const char* erisAudioOutputTDM::short_name_lookup PROGMEM = "tdm";
const char* erisAudioOutputTDM2::short_name_lookup PROGMEM = "tdm2";
const char* erisAudioOutputADAT::short_name_lookup PROGMEM = "adat";
*/
const char* erisAudioAmplifier::short_name_lookup PROGMEM = "amp";
const char* erisAudioMixer4::short_name_lookup PROGMEM = "mixer";

const char* erisAudioPlayMemory::short_name_lookup PROGMEM = "playMem";
const char* erisAudioPlaySerialflashRaw::short_name_lookup PROGMEM = "playFlashRaw";
const char* erisAudioPlayQueue::short_name_lookup PROGMEM = "play_queue";
const char* erisAudioRecordQueue::short_name_lookup PROGMEM = "record_queue";

const char* erisAudioSynthWavetable::short_name_lookup PROGMEM = "wavetable";
const char* erisAudioSynthSimpleDrum::short_name_lookup PROGMEM = "drum";
const char* erisAudioSynthKarplusStrong::short_name_lookup PROGMEM = "string";
const char* erisAudioSynthWaveformSine::short_name_lookup PROGMEM = "string";
const char* erisAudioSynthWaveformSineHires::short_name_lookup PROGMEM = "sine_hires";
const char* erisAudioSynthWaveformSineModulated::short_name_lookup PROGMEM = "sine_fm";
const char* erisAudioSynthWaveform::short_name_lookup PROGMEM = "waveform";
const char* erisAudioSynthWaveformModulated::short_name_lookup PROGMEM = "waveformMod";
const char* erisAudioSynthWaveform2k::short_name_lookup PROGMEM = "waveform2k";
const char* erisAudioSynthWaveformModulated2k::short_name_lookup PROGMEM = "waveformMod2k";
const char* erisAudioSynthWaveformPWM::short_name_lookup PROGMEM = "pwm";
const char* erisAudioSynthToneSweep::short_name_lookup PROGMEM = "tonesweep";
const char* erisAudioSynthWaveformDc::short_name_lookup PROGMEM = "dc";
const char* erisAudioSynthNoiseWhite::short_name_lookup PROGMEM = "noise";
const char* erisAudioSynthNoisePink::short_name_lookup PROGMEM = "pink";
const char* erisAudioEffectFade::short_name_lookup PROGMEM = "fade";
const char* erisAudioEffectChorus::short_name_lookup PROGMEM = "chorus";
const char* erisAudioEffectFlange::short_name_lookup PROGMEM = "flange";
const char* erisAudioEffectReverb::short_name_lookup PROGMEM = "reverb";
const char* erisAudioEffectFreeverb::short_name_lookup PROGMEM = "freeverb";
const char* erisAudioEffectFreeverbStereo::short_name_lookup PROGMEM = "freeverbs";
const char* erisAudioEffectEnvelope::short_name_lookup PROGMEM = "envelope";
const char* erisAudioEffectMultiply::short_name_lookup PROGMEM = "multiply";
const char* erisAudioEffectDelay::short_name_lookup PROGMEM = "delay";
const char* erisAudioEffectDelayExternal::short_name_lookup PROGMEM = "delayExt";
const char* erisAudioEffectBitcrusher::short_name_lookup PROGMEM = "bitcrusher";
const char* erisAudioEffectMidSide::short_name_lookup PROGMEM = "midside";
const char* erisAudioEffectWaveshaper::short_name_lookup PROGMEM = "waveshape";
const char* erisAudioEffectGranular::short_name_lookup PROGMEM = "granular";
const char* erisAudioEffectDigitalCombine::short_name_lookup PROGMEM = "combine";
const char* erisAudioFilterBiquad::short_name_lookup PROGMEM = "biquad";
const char* erisAudioFilterFIR::short_name_lookup PROGMEM = "fir";

const char* erisAudioFilterStateVariable::short_name_lookup PROGMEM = "filter";
const char* erisAudioAnalyzePeak::short_name_lookup PROGMEM = "peak";
const char* erisAudioAnalyzeRMS::short_name_lookup PROGMEM = "rms";
const char* erisAudioAnalyzeFFT256::short_name_lookup PROGMEM = "fft256";
const char* __erisAudioAnalyzeFFT1024::short_name_lookup PROGMEM = "fft1024";
const char* erisAudioAnalyzeToneDetect::short_name_lookup PROGMEM = "tone";
const char* erisAudioAnalyzeNoteFrequency::short_name_lookup PROGMEM = "notefreq";
//const char* erisAudioAnalyzePrint::short_name_lookup PROGMEM = "print";
