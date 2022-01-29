#ifndef ErisAudioParameterController__
#define ErisAudioParameterController__

/**
 * @file SvcErisAudioParameterController.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "AppBaseClass.h"


//globals - parameter class
const char gPC_ERISAUDIOANALYZEFFT1024[] PROGMEM = "erisAudioAnalyzeFFT1024";
const char gPC_ERISAUDIOANALYZEFFT256[] PROGMEM = "erisAudioAnalyzeFFT256";
const char gPC_ERISAUDIOANALYZENOTEFREQUENCY[] PROGMEM = "erisAudioAnalyzeNoteFrequency";
const char gPC_ERISAUDIOANALYZEPEAK[] PROGMEM = "erisAudioAnalyzePeak";
const char gPC_ERISAUDIOANALYZEPRINT[] PROGMEM = "erisAudioAnalyzePrint";
const char gPC_ERISAUDIOANALYZERMS[] PROGMEM = "erisAudioAnalyzeRMS";
const char gPC_ERISAUDIOANALYZETONEDETECT[] PROGMEM = "erisAudioAnalyzeToneDetect";
const char gPC_ERISASYNCAUDIOINPUTSPDIF3[] PROGMEM = "erisAsyncAudioInputSPDIF3";
const char gPC_ERISAUDIOCONTROLCS42448[] PROGMEM = "erisAudioControlCS42448";
const char gPC_ERISAUDIOCONTROLCS4272[] PROGMEM = "erisAudioControlCS4272";
const char gPC_ERISAUDIOCONTROLSGTL5000[] PROGMEM = "erisAudioControlSGTL5000";
const char gPC_ERISAUDIOCONTROLTLV320AIC3206[] PROGMEM = "erisAudioControlTLV320AIC3206";
const char gPC_ERISAUDIOCONTROLWM8731[] PROGMEM = "erisAudioControlWM8731";
const char gPC_ERISAUDIOCONTROLWM8731MASTER[] PROGMEM = "erisAudioControlWM8731master";
const char gPC_ERISAUDIOEFFECTBITCRUSHER[] PROGMEM = "erisAudioEffectBitcrusher";
const char gPC_ERISAUDIOEFFECTCHORUS[] PROGMEM = "erisAudioEffectChorus";
const char gPC_ERISAUDIOEFFECTDIGITALCOMBINE[] PROGMEM = "erisAudioEffectDigitalCombine";
const char gPC_ERISAUDIOEFFECTDELAY[] PROGMEM = "erisAudioEffectDelay";
const char gPC_ERISAUDIOEFFECTDELAYEXTERNAL[] PROGMEM = "erisAudioEffectDelayExternal";
const char gPC_ERISAUDIOEFFECTENVELOPE[] PROGMEM = "erisAudioEffectEnvelope";
const char gPC_ERISAUDIOEFFECTFADE[] PROGMEM = "erisAudioEffectFade";
const char gPC_ERISAUDIOEFFECTFLANGE[] PROGMEM = "erisAudioEffectFlange";
const char gPC_ERISAUDIOEFFECTFREEVERB[] PROGMEM = "erisAudioEffectFreeverb";
const char gPC_ERISAUDIOEFFECTFREEVERBSTEREO[] PROGMEM = "erisAudioEffectFreeverbStereo";
const char gPC_ERISAUDIOEFFECTGRANULAR[] PROGMEM = "erisAudioEffectGranular";
const char gPC_ERISAUDIOEFFECTREVERB[] PROGMEM = "erisAudioEffectReverb";
const char gPC_ERISAUDIOFILTERFIR[] PROGMEM = "erisAudioFilterFIR";
const char gPC_ERISAUDIOFILTERLADDER[] PROGMEM = "erisAudioFilterLadder";
const char gPC_ERISAUDIOFILTERSTATEVARIABLE[] PROGMEM = "erisAudioFilterStateVariable";
const char gPC_ERISAUDIOINPUTI2S[] PROGMEM = "erisAudioInputI2S";
const char gPC_ERISAUDIOINPUTI2SSLAVE[] PROGMEM = "erisAudioInputI2Sslave";
const char gPC_ERISAUDIOINPUTI2S2[] PROGMEM = "erisAudioInputI2S2";
const char gPC_ERISAUDIOINPUTI2S2SLAVE[] PROGMEM = "erisAudioInputI2S2slave";
const char gPC_ERISAUDIOINPUTI2SHEX[] PROGMEM = "erisAudioInputI2SHex";
const char gPC_ERISAUDIOINPUTI2SOCT[] PROGMEM = "erisAudioInputI2SOct";
const char gPC_ERISAUDIOINPUTI2SQUAD[] PROGMEM = "erisAudioInputI2SQuad";
const char gPC_ERISAUDIOINPUTPDM[] PROGMEM = "erisAudioInputPDM";
const char gPC_ERISAUDIOINPUTPDM2[] PROGMEM = "erisAudioInputPDM2";
const char gPC_ERISAUDIOINPUTSPDIF3[] PROGMEM = "erisAudioInputSPDIF3";
const char gPC_ERISAUDIOINPUTTDM[] PROGMEM = "erisAudioInputTDM";
const char gPC_ERISAUDIOINPUTTDM2[] PROGMEM = "erisAudioInputTDM2";
const char gPC_ERISAUDIOMIXER4[] PROGMEM = "erisAudioMixer4";
const char gPC_ERISAUDIOAMPLIFIER[] PROGMEM = "erisAudioAmplifier";
const char gPC_ERISAUDIOOUTPUTADAT[] PROGMEM = "erisAudioOutputADAT";
const char gPC_ERISAUDIOOUTPUTANALOG[] PROGMEM = "erisAudioOutputAnalog";
const char gPC_ERISAUDIOOUTPUTANALOGSTEREO[] PROGMEM = "erisAudioOutputAnalogStereo";
const char gPC_ERISAUDIOOUTPUTI2S[] PROGMEM = "erisAudioOutputI2S";
const char gPC_ERISAUDIOOUTPUTI2SSLAVE[] PROGMEM = "erisAudioOutputI2Sslave";
const char gPC_ERISAUDIOOUTPUTI2S2[] PROGMEM = "erisAudioOutputI2S2";
const char gPC_ERISAUDIOOUTPUTI2S2SLAVE[] PROGMEM = "erisAudioOutputI2S2slave";
const char gPC_ERISAUDIOOUTPUTI2SHEX[] PROGMEM = "erisAudioOutputI2SHex";
const char gPC_ERISAUDIOOUTPUTI2SOCT[] PROGMEM = "erisAudioOutputI2SOct";
const char gPC_ERISAUDIOOUTPUTI2SQUAD[] PROGMEM = "erisAudioOutputI2SQuad";
const char gPC_ERISAUDIOOUTPUTMQS[] PROGMEM = "erisAudioOutputMQS";
const char gPC_ERISAUDIOOUTPUTPT8211[] PROGMEM = "erisAudioOutputPT8211";
const char gPC_ERISAUDIOOUTPUTPT8211_2[] PROGMEM = "erisAudioOutputPT8211_2";
const char gPC_ERISAUDIOOUTPUTPWM[] PROGMEM = "erisAudioOutputPWM";
const char gPC_ERISAUDIOOUTPUTSPDIF[] PROGMEM = "erisAudioOutputSPDIF";
const char gPC_ERISAUDIOOUTPUTSPDIF2[] PROGMEM = "erisAudioOutputSPDIF2";
const char gPC_ERISAUDIOOUTPUTSPDIF3[] PROGMEM = "erisAudioOutputSPDIF3";
const char gPC_ERISAUDIOOUTPUTTDM[] PROGMEM = "erisAudioOutputTDM";
const char gPC_ERISAUDIOOUTPUTTDM2[] PROGMEM = "erisAudioOutputTDM2";
const char gPC_ERISAUDIOPLAYMEMORY[] PROGMEM = "erisAudioPlayMemory";
const char gPC_ERISAUDIOPLAYQUEUE[] PROGMEM = "erisAudioPlayQueue";
const char gPC_ERISAUDIOPLAYSDRAW[] PROGMEM = "erisAudioPlaySdRaw";
const char gPC_ERISAUDIOPLAYSDWAV[] PROGMEM = "erisAudioPlaySdWav";
const char gPC_ERISAUDIOPLAYSERIALFLASHRAW[] PROGMEM = "erisAudioPlaySerialflashRaw";
const char gPC_ERISQUANTIZER[] PROGMEM = "erisQuantizer";
const char gPC_ERISAUDIORECORDQUEUE[] PROGMEM = "erisAudioRecordQueue";
const char gPC_ERISRESAMPLER[] PROGMEM = "erisResampler";
const char gPC_ERISAUDIOSYNTHWAVEFORMDC[] PROGMEM = "erisAudioSynthWaveformDc";
const char gPC_ERISAUDIOSYNTHKARPLUSSTRONG[] PROGMEM = "erisAudioSynthKarplusStrong";
const char gPC_ERISAUDIOSYNTHNOISEPINK[] PROGMEM = "erisAudioSynthNoisePink";
const char gPC_ERISAUDIOSYNTHWAVEFORMPWM[] PROGMEM = "erisAudioSynthWaveformPWM";
const char gPC_ERISAUDIOSYNTHSIMPLEDRUM[] PROGMEM = "erisAudioSynthSimpleDrum";
const char gPC_ERISAUDIOSYNTHWAVEFORMSINE[] PROGMEM = "erisAudioSynthWaveformSine";
const char gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES[] PROGMEM = "erisAudioSynthWaveformSineHires";
const char gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED[] PROGMEM = "erisAudioSynthWaveformSineModulated";
const char gPC_ERISAUDIOSYNTHTONESWEEP[] PROGMEM = "erisAudioSynthToneSweep";
const char gPC_ERISAUDIOSYNTHWAVEFORM[] PROGMEM = "erisAudioSynthWaveform";
const char gPC_ERISAUDIOSYNTHWAVEFORMMODULATED[] PROGMEM = "erisAudioSynthWaveformModulated";
const char gPC_ERISAUDIOSYNTHWAVETABLE[] PROGMEM = "erisAudioSynthWavetable";
const char gPC_ERISAUDIOSYNTHNOISEWHITE[] PROGMEM = "erisAudioSynthNoiseWhite";

//globals - parameter methods
const char gPM_AVAILABLE[] PROGMEM = "available";
const char gPM_READ[] PROGMEM = "read";
const char gPM_AVERAGETOGETHER[] PROGMEM = "averageTogether";
const char gPM_INIT[] PROGMEM = "init";
const char gPM_BEGIN[] PROGMEM = "begin";
const char gPM_THRESHOLD[] PROGMEM = "threshold";
const char gPM_PROBABILITY[] PROGMEM = "probability";
const char gPM_PROCESS[] PROGMEM = "process";
const char gPM_READPEAKTOPEAK[] PROGMEM = "readPeakToPeak";
const char gPM_TRIGGER[] PROGMEM = "trigger";
const char gPM_SET_PARAMS[] PROGMEM = "set_params";
const char gPM_BOOL[] PROGMEM = "bool";
const char gPM_GETHALFFILTERLENGTH[] PROGMEM = "getHalfFilterLength";
const char gPM_MONITORRESAMPLEBUFFER[] PROGMEM = "monitorResampleBuffer";
const char gPM_CONFIGURE[] PROGMEM = "configure";
const char gPM_CONFIG_SPDIFIN[] PROGMEM = "config_spdifIn";
const char gPM_SETADDRESS[] PROGMEM = "setAddress";
const char gPM_ENABLE[] PROGMEM = "enable";
const char gPM_DISABLE[] PROGMEM = "disable";
const char gPM_VOLUME[] PROGMEM = "volume";
const char gPM_INPUTLEVEL[] PROGMEM = "inputLevel";
const char gPM_INPUTSELECT[] PROGMEM = "inputSelect";
const char gPM_VOLUMEINTEGER[] PROGMEM = "volumeInteger";
const char gPM_INPUTLEVELINTEGER[] PROGMEM = "inputLevelInteger";
const char gPM_VOLUMEBYTE[] PROGMEM = "volumebyte";
const char gPM_INPUTLEVELBYTE[] PROGMEM = "inputlevelbyte";
const char gPM_WRITE[] PROGMEM = "write";
const char gPM_DACVOLUME[] PROGMEM = "dacVolume";
const char gPM_MUTEOUTPUT[] PROGMEM = "muteOutput";
const char gPM_UNMUTEOUTPUT[] PROGMEM = "unmuteOutput";
const char gPM_MUTEINPUT[] PROGMEM = "muteInput";
const char gPM_UNMUTEINPUT[] PROGMEM = "unmuteInput";
const char gPM_ENABLEDITHER[] PROGMEM = "enableDither";
const char gPM_DISABLEDITHER[] PROGMEM = "disableDither";
const char gPM_INITLOCALREGS[] PROGMEM = "initLocalRegs";
const char gPM_MUTEHEADPHONE[] PROGMEM = "muteHeadphone";
const char gPM_MUTELINEOUT[] PROGMEM = "muteLineout";
const char gPM_HEADPHONESELECT[] PROGMEM = "headphoneSelect";
const char gPM_MICGAIN[] PROGMEM = "micGain";
const char gPM_LINEINLEVEL[] PROGMEM = "lineInLevel";
const char gPM_DACVOLUMERAMP[] PROGMEM = "dacVolumeRamp";
const char gPM_DACVOLUMERAMPLINEAR[] PROGMEM = "dacVolumeRampLinear";
const char gPM_DACVOLUMERAMPDISABLE[] PROGMEM = "dacVolumeRampDisable";
const char gPM_ADCHIGHPASSFILTERENABLE[] PROGMEM = "adcHighPassFilterEnable";
const char gPM_ADCHIGHPASSFILTERFREEZE[] PROGMEM = "adcHighPassFilterFreeze";
const char gPM_ADCHIGHPASSFILTERDISABLE[] PROGMEM = "adcHighPassFilterDisable";
const char gPM_AUDIOPREPROCESSORENABLE[] PROGMEM = "audioPreProcessorEnable";
const char gPM_AUDIOPOSTPROCESSORENABLE[] PROGMEM = "audioPostProcessorEnable";
const char gPM_AUDIOPROCESSORDISABLE[] PROGMEM = "audioProcessorDisable";
const char gPM_EQBANDS[] PROGMEM = "eqBands";
const char gPM_AUTOVOLUMEENABLE[] PROGMEM = "autoVolumeEnable";
const char gPM_AUTOVOLUMEDISABLE[] PROGMEM = "autoVolumeDisable";
const char gPM_ENHANCEBASSENABLE[] PROGMEM = "enhanceBassEnable";
const char gPM_ENHANCEBASSDISABLE[] PROGMEM = "enhanceBassDisable";
const char gPM_SURROUNDSOUNDENABLE[] PROGMEM = "surroundSoundEnable";
const char gPM_SURROUNDSOUNDDISABLE[] PROGMEM = "surroundSoundDisable";
const char gPM_SETMASTERMODE[] PROGMEM = "setMasterMode";
const char gPM_MODIFY[] PROGMEM = "modify";
const char gPM_AUTOMATE[] PROGMEM = "automate";
const char gPM_SETUP[] PROGMEM = "setup";
const char gPM_LOOP[] PROGMEM = "loop";
const char gPM_OUTPUTSELECT[] PROGMEM = "outputSelect";
const char gPM_VOLUME_DB[] PROGMEM = "volume_dB";
const char gPM_SETINPUTGAIN_DB[] PROGMEM = "setInputGain_dB";
const char gPM_SETMICBIAS[] PROGMEM = "setMicBias";
const char gPM_ENABLEMICDETECT[] PROGMEM = "enableMicDetect";
const char gPM_READMICDETECT[] PROGMEM = "readMicDetect";
const char gPM_AIC_READPAGE[] PROGMEM = "aic_readPage";
const char gPM_AIC_WRITEPAGE[] PROGMEM = "aic_writePage";
const char gPM_SETHPFONADC[] PROGMEM = "setHPFonADC";
const char gPM_GETHPCUTOFF_HZ[] PROGMEM = "getHPCutoff_Hz";
const char gPM_GETSAMPLERATE_HZ[] PROGMEM = "getSampleRate_Hz";
const char gPM_ENABLEAUTOMUTEDAC[] PROGMEM = "enableAutoMuteDAC";
const char gPM_AIC_RESET[] PROGMEM = "aic_reset";
const char gPM_AIC_INIT[] PROGMEM = "aic_init";
const char gPM_AIC_INITDAC[] PROGMEM = "aic_initDAC";
const char gPM_AIC_INITADC[] PROGMEM = "aic_initADC";
const char gPM_AIC_WRITEADDRESS[] PROGMEM = "aic_writeAddress";
const char gPM_AIC_GOTOPAGE[] PROGMEM = "aic_goToPage";
const char gPM_BITS[] PROGMEM = "bits";
const char gPM_SAMPLERATE[] PROGMEM = "sampleRate";
const char gPM_VOICES[] PROGMEM = "voices";
const char gPM_SETCOMBINEMODE[] PROGMEM = "setCombineMode";
const char gPM_DELAY[] PROGMEM = "delay";
const char gPM_RECOMPUTE_MAXBLOCKS[] PROGMEM = "recompute_maxblocks";
const char gPM_INITIALIZE[] PROGMEM = "initialize";
const char gPM_ZERO[] PROGMEM = "zero";
const char gPM_NOTEON[] PROGMEM = "noteOn";
const char gPM_NOTEOFF[] PROGMEM = "noteOff";
const char gPM_ATTACK[] PROGMEM = "attack";
const char gPM_HOLD[] PROGMEM = "hold";
const char gPM_DECAY[] PROGMEM = "decay";
const char gPM_SUSTAIN[] PROGMEM = "sustain";
const char gPM_RELEASE[] PROGMEM = "release";
const char gPM_RELEASENOTEON[] PROGMEM = "releaseNoteOn";
const char gPM_ISACTIVE[] PROGMEM = "isActive";
const char gPM_ISSUSTAIN[] PROGMEM = "isSustain";
const char gPM_FADEIN[] PROGMEM = "fadeIn";
const char gPM_FADEOUT[] PROGMEM = "fadeOut";
const char gPM_FADEBEGIN[] PROGMEM = "fadeBegin";
const char gPM_ROOMSIZE[] PROGMEM = "roomsize";
const char gPM_DAMPING[] PROGMEM = "damping";
const char gPM_SETSPEED[] PROGMEM = "setSpeed";
const char gPM_BEGINFREEZE[] PROGMEM = "beginFreeze";
const char gPM_BEGINPITCHSHIFT[] PROGMEM = "beginPitchShift";
const char gPM_STOP[] PROGMEM = "stop";
const char gPM_BEGINFREEZE_INT[] PROGMEM = "beginFreeze_int";
const char gPM_BEGINPITCHSHIFT_INT[] PROGMEM = "beginPitchShift_int";
const char gPM_REVERBTIME[] PROGMEM = "reverbTime";
const char gPM_INIT_COMB_FILTERS[] PROGMEM = "init_comb_filters";
const char gPM_CLEAR_BUFFERS[] PROGMEM = "clear_buffers";
const char gPM_END[] PROGMEM = "end";
const char gPM_FREQUENCY[] PROGMEM = "frequency";
const char gPM_RESONANCE[] PROGMEM = "resonance";
const char gPM_OCTAVECONTROL[] PROGMEM = "octaveControl";
const char gPM_PASSBANDGAIN[] PROGMEM = "passbandGain";
const char gPM_INPUTDRIVE[] PROGMEM = "inputDrive";
const char gPM_INTERPOLATIONMETHOD[] PROGMEM = "interpolationMethod";
const char gPM_COMPUTE_COEFFS[] PROGMEM = "compute_coeffs";
const char gPM_INITPOLY[] PROGMEM = "initpoly";
const char gPM_RESONATING[] PROGMEM = "resonating";
const char gPM_GAIN[] PROGMEM = "gain";
const char gPM_ANALOGREFERENCE[] PROGMEM = "analogReference";
const char gPM_ISPLAYING[] PROGMEM = "isPlaying";
const char gPM_POSITIONMILLIS[] PROGMEM = "positionMillis";
const char gPM_LENGTHMILLIS[] PROGMEM = "lengthMillis";
const char gPM_PLAY[] PROGMEM = "play";
const char gPM_PLAYBUFFER[] PROGMEM = "playBuffer";
const char gPM_TOGGLEPLAYPAUSE[] PROGMEM = "togglePlayPause";
const char gPM_ISPAUSED[] PROGMEM = "isPaused";
const char gPM_ISSTOPPED[] PROGMEM = "isStopped";
const char gPM_CONSUME[] PROGMEM = "consume";
const char gPM_PARSE_FORMAT[] PROGMEM = "parse_format";
const char gPM_RESET[] PROGMEM = "reset";
const char gPM_CLEAR[] PROGMEM = "clear";
const char gPM_FREEBUFFER[] PROGMEM = "freeBuffer";
const char gPM_ADDTOSAMPLEDIFF[] PROGMEM = "addToSampleDiff";
const char gPM_ADDTOPOS[] PROGMEM = "addToPos";
const char gPM_FIXSTEP[] PROGMEM = "fixStep";
const char gPM_INITIALIZED[] PROGMEM = "initialized";
const char gPM_GETKAISEREXACT[] PROGMEM = "getKaiserExact";
const char gPM_SETKAISERWINDOW[] PROGMEM = "setKaiserWindow";
const char gPM_SETFILTER[] PROGMEM = "setFilter";
const char gPM_AMPLITUDE[] PROGMEM = "amplitude";
const char gPM_LENGTH[] PROGMEM = "length";
const char gPM_SECONDMIX[] PROGMEM = "secondMix";
const char gPM_PITCHMOD[] PROGMEM = "pitchMod";
const char gPM_PHASE[] PROGMEM = "phase";
const char gPM_OFFSET[] PROGMEM = "offset";
const char gPM_PULSEWIDTH[] PROGMEM = "pulseWidth";
const char gPM_FREQUENCYMODULATION[] PROGMEM = "frequencyModulation";
const char gPM_PHASEMODULATION[] PROGMEM = "phaseModulation";
const char gPM_SETFREQUENCY[] PROGMEM = "setFrequency";
const char gPM_GETENVSTATE[] PROGMEM = "getEnvState";
const char gPM_SETSTATE[] PROGMEM = "setState";



/**
 * @brief Parameter controller for interacting with the audio components through the SvcSerialCommandInterface (SCI)
 * note: hand/autogenerated code bootstraped from the autowrapper script (see the tools folder)
 * 
 */
class SvcErisAudioParameterController:public AppBaseClass {
  public:
    SvcErisAudioParameterController():AppBaseClass(){
        strcpy(name,"APC");
    }; 
    //define event handlers
    void messageHandler(AppBaseClass *sender, const char *message) override;
  protected:
    void render() override{};    //called only when the app is active
    void update() override{}; //allways called even if app is not active
    void onFocus() override{};   //called when given focus
    void onFocusLost() override{}; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{};
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{};
    void onAnalog1(uint16_t uval, float fval) override{};
    void onAnalog2(uint16_t uval, float fval) override{};
    void onAnalog3(uint16_t uval, float fval) override{};
    void onAnalog4(uint16_t uval, float fval) override{};
};


#endif
