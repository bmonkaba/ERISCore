#include "ErisAudio.h"
#include "SvcErisAudioParameterController.h"

void FLASHMEM SvcErisAudioParameterController::messageHandler(AppBaseClass *sender, const char *message){
    char c[64];
    char i[64];
    char m[64];
    char p[64];
    char objName[64];

    int total_read;
    //class,instance,method,params
    sci->printf("M ");sci->println(message);

    total_read = sscanf(message, "%s %s %s %s" , c, i, m, p);
    sci->printf("M APC Request: class: %64s instance: %64s method:%64s params: %64s\n" , c, i, m, p);//"%64s %64s %64s %64s" , c, i, m, p
    sci->send();
    if(0){}else if(strncmp(gPC_ERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT1024* eac = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT1024* eac = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint16_t binNumber;
            total_read = sscanf(p, "%hu" , &binNumber); // ['uint32_t binNumber']
            eac->read(binNumber);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT256,c,sizeof(gPC_ERISAUDIOANALYZEFFT256)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT256::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT256* eac = (erisAudioAnalyzeFFT256*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT256,c,sizeof(gPC_ERISAUDIOANALYZEFFT256)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT256::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT256* eac = (erisAudioAnalyzeFFT256*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t binNumber;
            total_read = sscanf(p, "%lu" , &binNumber); // ['uint32_t binNumber']
            eac->read(binNumber);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT256,c,sizeof(gPC_ERISAUDIOANALYZEFFT256)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT256::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT256* eac = (erisAudioAnalyzeFFT256*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t binFirst;
			uint32_t binLast;
            total_read = sscanf(p, "%lu,%lu" , &binFirst,&binLast); // ['uint32_t binFirst', 'uint32_t binLast']
            eac->read(binFirst,binLast);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT256,c,sizeof(gPC_ERISAUDIOANALYZEFFT256)) == 0 &&\
        strncmp(gPM_AVERAGETOGETHER,m,sizeof(gPM_AVERAGETOGETHER)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT256::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT256* eac = (erisAudioAnalyzeFFT256*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t n;
            total_read = sscanf(p, "%c" , &n); // ['uint8_t n']
            eac->averageTogether(n);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            float thresh;
            total_read = sscanf(p, "%f" , &thresh); // [' float threshold ']
            eac->begin(thresh);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_THRESHOLD,m,sizeof(gPM_THRESHOLD)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            float thresh;
            total_read = sscanf(p, "%f" , &thresh); // [' float p']
            eac->threshold(thresh);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // [' void ']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // [' void ']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_PROBABILITY,m,sizeof(gPM_PROBABILITY)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // [' void ']
            eac->probability();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEPEAK,c,sizeof(gPC_ERISAUDIOANALYZEPEAK)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzePeak::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzePeak* eac = (erisAudioAnalyzePeak*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEPEAK,c,sizeof(gPC_ERISAUDIOANALYZEPEAK)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzePeak::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzePeak* eac = (erisAudioAnalyzePeak*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEPEAK,c,sizeof(gPC_ERISAUDIOANALYZEPEAK)) == 0 &&\
        strncmp(gPM_READPEAKTOPEAK,m,sizeof(gPM_READPEAKTOPEAK)) == 0){
        strcpy(objName,erisAudioAnalyzePeak::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzePeak* eac = (erisAudioAnalyzePeak*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->readPeakToPeak();
        }
    /*}else if(strncmp(gPC_ERISAUDIOANALYZEPRINT,c,sizeof(gPC_ERISAUDIOANALYZEPRINT)) == 0 &&\
        strncmp(gPM_TRIGGER,m,sizeof(gPM_TRIGGER)) == 0){
        strcpy(objName,erisAudioAnalyzePrint::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzePrint* eac = (erisAudioAnalyzePrint*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->trigger();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEPRINT,c,sizeof(gPC_ERISAUDIOANALYZEPRINT)) == 0 &&\
        strncmp(gPM_TRIGGER,m,sizeof(gPM_TRIGGER)) == 0){
        strcpy(objName,erisAudioAnalyzePrint::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzePrint* eac = (erisAudioAnalyzePrint*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
			int16_t edge;
            total_read = sscanf(p, "%f,%hi" , &level,&edge); // ['float level', 'int edge']
            eac->trigger(level,edge);
        }*/
    }else if(strncmp(gPC_ERISAUDIOANALYZERMS,c,sizeof(gPC_ERISAUDIOANALYZERMS)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzeRMS::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeRMS* eac = (erisAudioAnalyzeRMS*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZERMS,c,sizeof(gPC_ERISAUDIOANALYZERMS)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeRMS::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeRMS* eac = (erisAudioAnalyzeRMS*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZETONEDETECT,c,sizeof(gPC_ERISAUDIOANALYZETONEDETECT)) == 0 &&\
        strncmp(gPM_SET_PARAMS,m,sizeof(gPM_SET_PARAMS)) == 0){
        strcpy(objName,erisAudioAnalyzeToneDetect::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeToneDetect* eac = (erisAudioAnalyzeToneDetect*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t coef;
			uint16_t cycles;
			uint16_t len;
            total_read = sscanf(p, "%li,%hu,%hu" , &coef,&cycles,&len); // ['int32_t coef', 'uint16_t cycles', 'uint16_t len']
            eac->set_params(coef,cycles,len);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZETONEDETECT,c,sizeof(gPC_ERISAUDIOANALYZETONEDETECT)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioAnalyzeToneDetect::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeToneDetect* eac = (erisAudioAnalyzeToneDetect*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZETONEDETECT,c,sizeof(gPC_ERISAUDIOANALYZETONEDETECT)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeToneDetect::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeToneDetect* eac = (erisAudioAnalyzeToneDetect*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZETONEDETECT,c,sizeof(gPC_ERISAUDIOANALYZETONEDETECT)) == 0 &&\
        strncmp(gPM_THRESHOLD,m,sizeof(gPM_THRESHOLD)) == 0){
        strcpy(objName,erisAudioAnalyzeToneDetect::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeToneDetect* eac = (erisAudioAnalyzeToneDetect*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->threshold(level);
        }
    }/*else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->begin();
        }
    }*/else if(strncmp(gPC_ERISAUDIOEFFECTBITCRUSHER,c,sizeof(gPC_ERISAUDIOEFFECTBITCRUSHER)) == 0 &&\
        strncmp(gPM_BITS,m,sizeof(gPM_BITS)) == 0){
        strcpy(objName,erisAudioEffectBitcrusher::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectBitcrusher* eac = (erisAudioEffectBitcrusher*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t b;
            total_read = sscanf(p, "%c" , &b); // ['uint8_t b']
            eac->bits(b);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTBITCRUSHER,c,sizeof(gPC_ERISAUDIOEFFECTBITCRUSHER)) == 0 &&\
        strncmp(gPM_SAMPLERATE,m,sizeof(gPM_SAMPLERATE)) == 0){
        strcpy(objName,erisAudioEffectBitcrusher::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectBitcrusher* eac = (erisAudioEffectBitcrusher*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float hz;
            total_read = sscanf(p, "%f" , &hz); // ['float hz']
            eac->sampleRate(hz);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTCHORUS,c,sizeof(gPC_ERISAUDIOEFFECTCHORUS)) == 0 &&\
        strncmp(gPM_VOICES,m,sizeof(gPM_VOICES)) == 0){
        strcpy(objName,erisAudioEffectChorus::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectChorus* eac = (erisAudioEffectChorus*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n_chorus;
            total_read = sscanf(p, "%hi" , &n_chorus); // ['int n_chorus']
            eac->voices(n_chorus);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDIGITALCOMBINE,c,sizeof(gPC_ERISAUDIOEFFECTDIGITALCOMBINE)) == 0 &&\
        strncmp(gPM_SETCOMBINEMODE,m,sizeof(gPM_SETCOMBINEMODE)) == 0){
        strcpy(objName,erisAudioEffectDigitalCombine::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDigitalCombine* eac = (erisAudioEffectDigitalCombine*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t mode_in;
            total_read = sscanf(p, "%hi" , &mode_in); // ['int mode_in']
            eac->setCombineMode(mode_in);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAY,c,sizeof(gPC_ERISAUDIOEFFECTDELAY)) == 0 &&\
        strncmp(gPM_DELAY,m,sizeof(gPM_DELAY)) == 0){
        strcpy(objName,erisAudioEffectDelay::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelay* eac = (erisAudioEffectDelay*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t channel;
			float milliseconds;
            total_read = sscanf(p, "%c,%f" , &channel,&milliseconds); // ['uint8_t channel', 'float milliseconds']
            eac->delay(channel,milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAY,c,sizeof(gPC_ERISAUDIOEFFECTDELAY)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioEffectDelay::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelay* eac = (erisAudioEffectDelay*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t channel;
            total_read = sscanf(p, "%c" , &channel); // ['uint8_t channel']
            eac->disable(channel);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAYEXTERNAL,c,sizeof(gPC_ERISAUDIOEFFECTDELAYEXTERNAL)) == 0 &&\
        strncmp(gPM_DELAY,m,sizeof(gPM_DELAY)) == 0){
        strcpy(objName,erisAudioEffectDelayExternal::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelayExternal* eac = (erisAudioEffectDelayExternal*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t channel;
			float milliseconds;
            total_read = sscanf(p, "%c,%f" , &channel,&milliseconds); // ['uint8_t channel', 'float milliseconds']
            eac->delay(channel,milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAYEXTERNAL,c,sizeof(gPC_ERISAUDIOEFFECTDELAYEXTERNAL)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioEffectDelayExternal::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelayExternal* eac = (erisAudioEffectDelayExternal*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t channel;
            total_read = sscanf(p, "%c" , &channel); // ['uint8_t channel']
            eac->disable(channel);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_NOTEON,m,sizeof(gPM_NOTEON)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->noteOn();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_NOTEOFF,m,sizeof(gPM_NOTEOFF)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->noteOff();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_DELAY,m,sizeof(gPM_DELAY)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->delay(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_ATTACK,m,sizeof(gPM_ATTACK)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->attack(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_HOLD,m,sizeof(gPM_HOLD)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->hold(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_DECAY,m,sizeof(gPM_DECAY)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->decay(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_SUSTAIN,m,sizeof(gPM_SUSTAIN)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->sustain(level);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_RELEASE,m,sizeof(gPM_RELEASE)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->release(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_RELEASENOTEON,m,sizeof(gPM_RELEASENOTEON)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float milliseconds;
            total_read = sscanf(p, "%f" , &milliseconds); // ['float milliseconds']
            eac->releaseNoteOn(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_ISACTIVE,m,sizeof(gPM_ISACTIVE)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->isActive();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTENVELOPE,c,sizeof(gPC_ERISAUDIOEFFECTENVELOPE)) == 0 &&\
        strncmp(gPM_ISSUSTAIN,m,sizeof(gPM_ISSUSTAIN)) == 0){
        strcpy(objName,erisAudioEffectEnvelope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectEnvelope* eac = (erisAudioEffectEnvelope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->isSustain();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFADE,c,sizeof(gPC_ERISAUDIOEFFECTFADE)) == 0 &&\
        strncmp(gPM_FADEIN,m,sizeof(gPM_FADEIN)) == 0){
        strcpy(objName,erisAudioEffectFade::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFade* eac = (erisAudioEffectFade*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t milliseconds;
            total_read = sscanf(p, "%lu" , &milliseconds); // ['uint32_t milliseconds']
            eac->fadeIn(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFADE,c,sizeof(gPC_ERISAUDIOEFFECTFADE)) == 0 &&\
        strncmp(gPM_FADEOUT,m,sizeof(gPM_FADEOUT)) == 0){
        strcpy(objName,erisAudioEffectFade::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFade* eac = (erisAudioEffectFade*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t milliseconds;
            total_read = sscanf(p, "%lu" , &milliseconds); // ['uint32_t milliseconds']
            eac->fadeOut(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFLANGE,c,sizeof(gPC_ERISAUDIOEFFECTFLANGE)) == 0 &&\
        strncmp(gPM_VOICES,m,sizeof(gPM_VOICES)) == 0){
        strcpy(objName,erisAudioEffectFlange::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFlange* eac = (erisAudioEffectFlange*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t delay_offset;
			int16_t d_depth;
			float delay_rate;
            total_read = sscanf(p, "%hi,%hi,%f" , &delay_offset,&d_depth,&delay_rate); // ['int delay_offset', 'int d_depth', 'float delay_rate']
            eac->voices(delay_offset,d_depth,delay_rate);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFREEVERB,c,sizeof(gPC_ERISAUDIOEFFECTFREEVERB)) == 0 &&\
        strncmp(gPM_ROOMSIZE,m,sizeof(gPM_ROOMSIZE)) == 0){
        strcpy(objName,erisAudioEffectFreeverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFreeverb* eac = (erisAudioEffectFreeverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->roomsize(n);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFREEVERB,c,sizeof(gPC_ERISAUDIOEFFECTFREEVERB)) == 0 &&\
        strncmp(gPM_DAMPING,m,sizeof(gPM_DAMPING)) == 0){
        strcpy(objName,erisAudioEffectFreeverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFreeverb* eac = (erisAudioEffectFreeverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->damping(n);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFREEVERBSTEREO,c,sizeof(gPC_ERISAUDIOEFFECTFREEVERBSTEREO)) == 0 &&\
        strncmp(gPM_ROOMSIZE,m,sizeof(gPM_ROOMSIZE)) == 0){
        strcpy(objName,erisAudioEffectFreeverbStereo::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFreeverbStereo* eac = (erisAudioEffectFreeverbStereo*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->roomsize(n);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTFREEVERBSTEREO,c,sizeof(gPC_ERISAUDIOEFFECTFREEVERBSTEREO)) == 0 &&\
        strncmp(gPM_DAMPING,m,sizeof(gPM_DAMPING)) == 0){
        strcpy(objName,erisAudioEffectFreeverbStereo::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFreeverbStereo* eac = (erisAudioEffectFreeverbStereo*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->damping(n);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_SETSPEED,m,sizeof(gPM_SETSPEED)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float ratio;
            total_read = sscanf(p, "%f" , &ratio); // ['float ratio']
            eac->setSpeed(ratio);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_BEGINFREEZE,m,sizeof(gPM_BEGINFREEZE)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float grain_length;
            total_read = sscanf(p, "%f" , &grain_length); // ['float grain_length']
            eac->beginFreeze(grain_length);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_BEGINPITCHSHIFT,m,sizeof(gPM_BEGINPITCHSHIFT)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float grain_length;
            total_read = sscanf(p, "%f" , &grain_length); // ['float grain_length']
            eac->beginPitchShift(grain_length);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTREVERB,c,sizeof(gPC_ERISAUDIOEFFECTREVERB)) == 0 &&\
        strncmp(gPM_REVERBTIME,m,sizeof(gPM_REVERBTIME)) == 0){
        strcpy(objName,erisAudioEffectReverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectReverb* eac = (erisAudioEffectReverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            float time;
            total_read = sscanf(p, "%f" , &time); // ['float']
            eac->reverbTime(time);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERFIR,c,sizeof(gPC_ERISAUDIOFILTERFIR)) == 0 &&\
        strncmp(gPM_END,m,sizeof(gPM_END)) == 0){
        strcpy(objName,erisAudioFilterFIR::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterFIR* eac = (erisAudioFilterFIR*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->end();
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERSTATEVARIABLE,c,sizeof(gPC_ERISAUDIOFILTERSTATEVARIABLE)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioFilterStateVariable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterStateVariable* eac = (erisAudioFilterStateVariable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERSTATEVARIABLE,c,sizeof(gPC_ERISAUDIOFILTERSTATEVARIABLE)) == 0 &&\
        strncmp(gPM_RESONANCE,m,sizeof(gPM_RESONANCE)) == 0){
        strcpy(objName,erisAudioFilterStateVariable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterStateVariable* eac = (erisAudioFilterStateVariable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float q;
            total_read = sscanf(p, "%f" , &q); // ['float q']
            eac->resonance(q);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERSTATEVARIABLE,c,sizeof(gPC_ERISAUDIOFILTERSTATEVARIABLE)) == 0 &&\
        strncmp(gPM_OCTAVECONTROL,m,sizeof(gPM_OCTAVECONTROL)) == 0){
        strcpy(objName,erisAudioFilterStateVariable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterStateVariable* eac = (erisAudioFilterStateVariable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->octaveControl(n);
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2S,c,sizeof(gPC_ERISAUDIOINPUTI2S)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2S::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2S* eac = (erisAudioInputI2S*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2SSLAVE,c,sizeof(gPC_ERISAUDIOINPUTI2SSLAVE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2Sslave::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2Sslave* eac = (erisAudioInputI2Sslave*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2S2,c,sizeof(gPC_ERISAUDIOINPUTI2S2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2S2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2S2* eac = (erisAudioInputI2S2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2SHEX,c,sizeof(gPC_ERISAUDIOINPUTI2SHEX)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2SHex::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2SHex* eac = (erisAudioInputI2SHex*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2SOCT,c,sizeof(gPC_ERISAUDIOINPUTI2SOCT)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2SOct::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2SOct* eac = (erisAudioInputI2SOct*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTI2SQUAD,c,sizeof(gPC_ERISAUDIOINPUTI2SQUAD)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2SQuad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2SQuad* eac = (erisAudioInputI2SQuad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOMIXER4,c,sizeof(gPC_ERISAUDIOMIXER4)) == 0 &&\
        strncmp(gPM_GAIN,m,sizeof(gPM_GAIN)) == 0){
        strcpy(objName,erisAudioMixer4::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioMixer4* eac = (erisAudioMixer4*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t channel;
			float gain;
            total_read = sscanf(p, "%lu,%f" , &channel,&gain); // ['uint32_t channel', 'float gain']
            eac->gain(channel,gain);
        }
    }else if(strncmp(gPC_ERISAUDIOMIXER4,c,sizeof(gPC_ERISAUDIOMIXER4)) == 0 &&\
        strncmp(gPM_GAIN,m,sizeof(gPM_GAIN)) == 0){
        strcpy(objName,erisAudioMixer4::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioMixer4* eac = (erisAudioMixer4*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t channel;
			float gain;
            total_read = sscanf(p, "%lu,%f" , &channel,&gain); // ['uint32_t channel', 'float gain']
            eac->gain(channel,gain);
        }
    }else if(strncmp(gPC_ERISAUDIOAMPLIFIER,c,sizeof(gPC_ERISAUDIOAMPLIFIER)) == 0 &&\
        strncmp(gPM_GAIN,m,sizeof(gPM_GAIN)) == 0){
        strcpy(objName,erisAudioAmplifier::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAmplifier* eac = (erisAudioAmplifier*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->gain(n);
        }
    /*
    }else if(strncmp(gPC_ERISAUDIOOUTPUTADAT,c,sizeof(gPC_ERISAUDIOOUTPUTADAT)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputADAT::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputADAT* eac = (erisAudioOutputADAT*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    
    }else if(strncmp(gPC_ERISAUDIOOUTPUTANALOG,c,sizeof(gPC_ERISAUDIOOUTPUTANALOG)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputAnalog::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputAnalog* eac = (erisAudioOutputAnalog*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTANALOG,c,sizeof(gPC_ERISAUDIOOUTPUTANALOG)) == 0 &&\
        strncmp(gPM_ANALOGREFERENCE,m,sizeof(gPM_ANALOGREFERENCE)) == 0){
        strcpy(objName,erisAudioOutputAnalog::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputAnalog* eac = (erisAudioOutputAnalog*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t ref;
            total_read = sscanf(p, "%hi" , &ref); // ['int ref']
            eac->analogReference(ref);
        }
    
    }else if(strncmp(gPC_ERISAUDIOOUTPUTANALOGSTEREO,c,sizeof(gPC_ERISAUDIOOUTPUTANALOGSTEREO)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputAnalogStereo::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputAnalogStereo* eac = (erisAudioOutputAnalogStereo*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTANALOGSTEREO,c,sizeof(gPC_ERISAUDIOOUTPUTANALOGSTEREO)) == 0 &&\
        strncmp(gPM_ANALOGREFERENCE,m,sizeof(gPM_ANALOGREFERENCE)) == 0){
        strcpy(objName,erisAudioOutputAnalogStereo::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputAnalogStereo* eac = (erisAudioOutputAnalogStereo*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t ref;
            total_read = sscanf(p, "%hi" , &ref); // ['int ref']
            eac->analogReference(ref);
    }*/    
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2S,c,sizeof(gPC_ERISAUDIOOUTPUTI2S)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2S::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2S* eac = (erisAudioOutputI2S*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2SSLAVE,c,sizeof(gPC_ERISAUDIOOUTPUTI2SSLAVE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2Sslave::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2Sslave* eac = (erisAudioOutputI2Sslave*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2S,c,sizeof(gPC_ERISAUDIOOUTPUTI2S)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2S::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2S* eac = (erisAudioOutputI2S*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2SSLAVE,c,sizeof(gPC_ERISAUDIOOUTPUTI2SSLAVE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2Sslave::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2Sslave* eac = (erisAudioOutputI2Sslave*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2S2,c,sizeof(gPC_ERISAUDIOOUTPUTI2S2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2S2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2S2* eac = (erisAudioOutputI2S2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2SHEX,c,sizeof(gPC_ERISAUDIOOUTPUTI2SHEX)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2SHex::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2SHex* eac = (erisAudioOutputI2SHex*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2SOCT,c,sizeof(gPC_ERISAUDIOOUTPUTI2SOCT)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2SOct::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2SOct* eac = (erisAudioOutputI2SOct*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2SQUAD,c,sizeof(gPC_ERISAUDIOOUTPUTI2SQUAD)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2SQuad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2SQuad* eac = (erisAudioOutputI2SQuad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }/*else if(strncmp(gPC_ERISAUDIOOUTPUTMQS,c,sizeof(gPC_ERISAUDIOOUTPUTMQS)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputMQS::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputMQS* eac = (erisAudioOutputMQS*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTPT8211,c,sizeof(gPC_ERISAUDIOOUTPUTPT8211)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputPT8211::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputPT8211* eac = (erisAudioOutputPT8211*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTPT8211,c,sizeof(gPC_ERISAUDIOOUTPUTPT8211)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputPT8211::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputPT8211* eac = (erisAudioOutputPT8211*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTPT8211_2,c,sizeof(gPC_ERISAUDIOOUTPUTPT8211_2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputPT8211_2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputPT8211_2* eac = (erisAudioOutputPT8211_2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTSPDIF,c,sizeof(gPC_ERISAUDIOOUTPUTSPDIF)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputSPDIF::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputSPDIF* eac = (erisAudioOutputSPDIF*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTSPDIF2,c,sizeof(gPC_ERISAUDIOOUTPUTSPDIF2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputSPDIF2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputSPDIF2* eac = (erisAudioOutputSPDIF2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTSPDIF3,c,sizeof(gPC_ERISAUDIOOUTPUTSPDIF3)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputSPDIF3* eac = (erisAudioOutputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTTDM,c,sizeof(gPC_ERISAUDIOOUTPUTTDM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputTDM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputTDM* eac = (erisAudioOutputTDM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTTDM2,c,sizeof(gPC_ERISAUDIOOUTPUTTDM2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputTDM2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputTDM2* eac = (erisAudioOutputTDM2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }*/else if(strncmp(gPC_ERISAUDIOPLAYMEMORY,c,sizeof(gPC_ERISAUDIOPLAYMEMORY)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlayMemory::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayMemory* eac = (erisAudioPlayMemory*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYMEMORY,c,sizeof(gPC_ERISAUDIOPLAYMEMORY)) == 0 &&\
        strncmp(gPM_ISPLAYING,m,sizeof(gPM_ISPLAYING)) == 0){
        strcpy(objName,erisAudioPlayMemory::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayMemory* eac = (erisAudioPlayMemory*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPlaying();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYMEMORY,c,sizeof(gPC_ERISAUDIOPLAYMEMORY)) == 0 &&\
        strncmp(gPM_POSITIONMILLIS,m,sizeof(gPM_POSITIONMILLIS)) == 0){
        strcpy(objName,erisAudioPlayMemory::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayMemory* eac = (erisAudioPlayMemory*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->positionMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYMEMORY,c,sizeof(gPC_ERISAUDIOPLAYMEMORY)) == 0 &&\
        strncmp(gPM_LENGTHMILLIS,m,sizeof(gPM_LENGTHMILLIS)) == 0){
        strcpy(objName,erisAudioPlayMemory::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayMemory* eac = (erisAudioPlayMemory*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->lengthMillis();
        }
    /*}else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_PLAY,m,sizeof(gPM_PLAY)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t data;
            total_read = sscanf(p, "%hi" , &data); // ['int16_t data']
            eac->play(data);
        }*/
    }else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_PLAYBUFFER,m,sizeof(gPM_PLAYBUFFER)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->playBuffer();
        }
    /*}else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }*/
    }else if(strncmp(gPC_ERISAUDIOPLAYSERIALFLASHRAW,c,sizeof(gPC_ERISAUDIOPLAYSERIALFLASHRAW)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioPlaySerialflashRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySerialflashRaw* eac = (erisAudioPlaySerialflashRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSERIALFLASHRAW,c,sizeof(gPC_ERISAUDIOPLAYSERIALFLASHRAW)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlaySerialflashRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySerialflashRaw* eac = (erisAudioPlaySerialflashRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSERIALFLASHRAW,c,sizeof(gPC_ERISAUDIOPLAYSERIALFLASHRAW)) == 0 &&\
        strncmp(gPM_ISPLAYING,m,sizeof(gPM_ISPLAYING)) == 0){
        strcpy(objName,erisAudioPlaySerialflashRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySerialflashRaw* eac = (erisAudioPlaySerialflashRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPlaying();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSERIALFLASHRAW,c,sizeof(gPC_ERISAUDIOPLAYSERIALFLASHRAW)) == 0 &&\
        strncmp(gPM_POSITIONMILLIS,m,sizeof(gPM_POSITIONMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySerialflashRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySerialflashRaw* eac = (erisAudioPlaySerialflashRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->positionMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSERIALFLASHRAW,c,sizeof(gPC_ERISAUDIOPLAYSERIALFLASHRAW)) == 0 &&\
        strncmp(gPM_LENGTHMILLIS,m,sizeof(gPM_LENGTHMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySerialflashRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySerialflashRaw* eac = (erisAudioPlaySerialflashRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->lengthMillis();
        }
    }else if(strncmp(gPC_ERISAUDIORECORDQUEUE,c,sizeof(gPC_ERISAUDIORECORDQUEUE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioRecordQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioRecordQueue* eac = (erisAudioRecordQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIORECORDQUEUE,c,sizeof(gPC_ERISAUDIORECORDQUEUE)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,erisAudioRecordQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioRecordQueue* eac = (erisAudioRecordQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISAUDIORECORDQUEUE,c,sizeof(gPC_ERISAUDIORECORDQUEUE)) == 0 &&\
        strncmp(gPM_CLEAR,m,sizeof(gPM_CLEAR)) == 0){
        strcpy(objName,erisAudioRecordQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioRecordQueue* eac = (erisAudioRecordQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->clear();
        }
    }else if(strncmp(gPC_ERISAUDIORECORDQUEUE,c,sizeof(gPC_ERISAUDIORECORDQUEUE)) == 0 &&\
        strncmp(gPM_FREEBUFFER,m,sizeof(gPM_FREEBUFFER)) == 0){
        strcpy(objName,erisAudioRecordQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioRecordQueue* eac = (erisAudioRecordQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->freeBuffer();
        }
    }else if(strncmp(gPC_ERISAUDIORECORDQUEUE,c,sizeof(gPC_ERISAUDIORECORDQUEUE)) == 0 &&\
        strncmp(gPM_END,m,sizeof(gPM_END)) == 0){
        strcpy(objName,erisAudioRecordQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioRecordQueue* eac = (erisAudioRecordQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->end();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMDC,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMDC)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformDc::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformDc* eac = (erisAudioSynthWaveformDc*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMDC,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMDC)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformDc::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformDc* eac = (erisAudioSynthWaveformDc*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
			float milliseconds;
            total_read = sscanf(p, "%f,%f" , &n,&milliseconds); // ['float n', 'float milliseconds']
            eac->amplitude(n,milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMDC,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMDC)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioSynthWaveformDc::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformDc* eac = (erisAudioSynthWaveformDc*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHKARPLUSSTRONG,c,sizeof(gPC_ERISAUDIOSYNTHKARPLUSSTRONG)) == 0 &&\
        strncmp(gPM_NOTEON,m,sizeof(gPM_NOTEON)) == 0){
        strcpy(objName,erisAudioSynthKarplusStrong::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthKarplusStrong* eac = (erisAudioSynthKarplusStrong*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float frequency;
			float velocity;
            total_read = sscanf(p, "%f,%f" , &frequency,&velocity); // ['float frequency', 'float velocity']
            eac->noteOn(frequency,velocity);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHKARPLUSSTRONG,c,sizeof(gPC_ERISAUDIOSYNTHKARPLUSSTRONG)) == 0 &&\
        strncmp(gPM_NOTEOFF,m,sizeof(gPM_NOTEOFF)) == 0){
        strcpy(objName,erisAudioSynthKarplusStrong::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthKarplusStrong* eac = (erisAudioSynthKarplusStrong*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float velocity;
            total_read = sscanf(p, "%f" , &velocity); // ['float velocity']
            eac->noteOff(velocity);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHNOISEPINK,c,sizeof(gPC_ERISAUDIOSYNTHNOISEPINK)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthNoisePink::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthNoisePink* eac = (erisAudioSynthNoisePink*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMPWM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMPWM)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveformPWM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformPWM* eac = (erisAudioSynthWaveformPWM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMPWM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMPWM)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformPWM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformPWM* eac = (erisAudioSynthWaveformPWM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHSIMPLEDRUM,c,sizeof(gPC_ERISAUDIOSYNTHSIMPLEDRUM)) == 0 &&\
        strncmp(gPM_NOTEON,m,sizeof(gPM_NOTEON)) == 0){
        strcpy(objName,erisAudioSynthSimpleDrum::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthSimpleDrum* eac = (erisAudioSynthSimpleDrum*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->noteOn();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHSIMPLEDRUM,c,sizeof(gPC_ERISAUDIOSYNTHSIMPLEDRUM)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthSimpleDrum::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthSimpleDrum* eac = (erisAudioSynthSimpleDrum*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHSIMPLEDRUM,c,sizeof(gPC_ERISAUDIOSYNTHSIMPLEDRUM)) == 0 &&\
        strncmp(gPM_LENGTH,m,sizeof(gPM_LENGTH)) == 0){
        strcpy(objName,erisAudioSynthSimpleDrum::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthSimpleDrum* eac = (erisAudioSynthSimpleDrum*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t milliseconds;
            total_read = sscanf(p, "%li" , &milliseconds); // ['int32_t milliseconds']
            eac->length(milliseconds);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHSIMPLEDRUM,c,sizeof(gPC_ERISAUDIOSYNTHSIMPLEDRUM)) == 0 &&\
        strncmp(gPM_SECONDMIX,m,sizeof(gPM_SECONDMIX)) == 0){
        strcpy(objName,erisAudioSynthSimpleDrum::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthSimpleDrum* eac = (erisAudioSynthSimpleDrum*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->secondMix(level);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHSIMPLEDRUM,c,sizeof(gPC_ERISAUDIOSYNTHSIMPLEDRUM)) == 0 &&\
        strncmp(gPM_PITCHMOD,m,sizeof(gPM_PITCHMOD)) == 0){
        strcpy(objName,erisAudioSynthSimpleDrum::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthSimpleDrum* eac = (erisAudioSynthSimpleDrum*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float depth;
            total_read = sscanf(p, "%f" , &depth); // ['float depth']
            eac->pitchMod(depth);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINE,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINE)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveformSine::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSine* eac = (erisAudioSynthWaveformSine*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINE,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINE)) == 0 &&\
        strncmp(gPM_PHASE,m,sizeof(gPM_PHASE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSine::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSine* eac = (erisAudioSynthWaveformSine*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float angle;
            total_read = sscanf(p, "%f" , &angle); // ['float angle']
            eac->phase(angle);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINE,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINE)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSine::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSine* eac = (erisAudioSynthWaveformSine*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineHires::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineHires* eac = (erisAudioSynthWaveformSineHires*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES)) == 0 &&\
        strncmp(gPM_PHASE,m,sizeof(gPM_PHASE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineHires::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineHires* eac = (erisAudioSynthWaveformSineHires*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float angle;
            total_read = sscanf(p, "%f" , &angle); // ['float angle']
            eac->phase(angle);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEHIRES)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineHires::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineHires* eac = (erisAudioSynthWaveformSineHires*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineModulated* eac = (erisAudioSynthWaveformSineModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED)) == 0 &&\
        strncmp(gPM_PHASE,m,sizeof(gPM_PHASE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineModulated* eac = (erisAudioSynthWaveformSineModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float angle;
            total_read = sscanf(p, "%f" , &angle); // ['float angle']
            eac->phase(angle);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMSINEMODULATED)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformSineModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformSineModulated* eac = (erisAudioSynthWaveformSineModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHTONESWEEP,c,sizeof(gPC_ERISAUDIOSYNTHTONESWEEP)) == 0 &&\
        strncmp(gPM_PLAY,m,sizeof(gPM_PLAY)) == 0){
        strcpy(objName,erisAudioSynthToneSweep::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        Serial.print("M ");Serial.println(objName);
        erisAudioSynthToneSweep* eac = (erisAudioSynthToneSweep*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float t_amp;
			int16_t t_lo;
			int16_t t_hi;
			float t_time;
            total_read = sscanf(p, "%f,%hi,%hi,%f" , &t_amp,&t_lo,&t_hi,&t_time); // ['float t_amp', 'int t_lo', 'int t_hi', 'float t_time']
            eac->play(t_amp,t_lo,t_hi,t_time);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHTONESWEEP,c,sizeof(gPC_ERISAUDIOSYNTHTONESWEEP)) == 0 &&\
        strncmp(gPM_ISPLAYING,m,sizeof(gPM_ISPLAYING)) == 0){
        strcpy(objName,erisAudioSynthToneSweep::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthToneSweep* eac = (erisAudioSynthToneSweep*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPlaying();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHTONESWEEP,c,sizeof(gPC_ERISAUDIOSYNTHTONESWEEP)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioSynthToneSweep::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthToneSweep* eac = (erisAudioSynthToneSweep*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->read();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_PHASE,m,sizeof(gPM_PHASE)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float angle;
            total_read = sscanf(p, "%f" , &angle); // ['float angle']
            eac->phase(angle);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_OFFSET,m,sizeof(gPM_OFFSET)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->offset(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_PULSEWIDTH,m,sizeof(gPM_PULSEWIDTH)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->pulseWidth(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            short t_type;
            total_read = sscanf(p, "%hi" , &t_type); // ['short t_type']
            eac->begin(t_type);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORM,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioSynthWaveform::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveform* eac = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float t_amp;
			float t_freq;
            short t_type;
            total_read = sscanf(p, "%f,%f,%hi" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
            eac->begin(t_amp,t_freq,t_type);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->frequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_OFFSET,m,sizeof(gPM_OFFSET)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->offset(n);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            short t_type;
            total_read = sscanf(p, "%hi" , &t_type); // ['short t_type']
            eac->begin(t_type);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float t_amp;
			float t_freq;
            short t_type;
            total_read = sscanf(p, "%f,%f,%hi" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
            eac->begin(t_amp,t_freq,t_type);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_FREQUENCYMODULATION,m,sizeof(gPM_FREQUENCYMODULATION)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float octaves;
            total_read = sscanf(p, "%f" , &octaves); // ['float octaves']
            eac->frequencyModulation(octaves);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED,c,sizeof(gPC_ERISAUDIOSYNTHWAVEFORMMODULATED)) == 0 &&\
        strncmp(gPM_PHASEMODULATION,m,sizeof(gPM_PHASEMODULATION)) == 0){
        strcpy(objName,erisAudioSynthWaveformModulated::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWaveformModulated* eac = (erisAudioSynthWaveformModulated*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float degrees;
            total_read = sscanf(p, "%f" , &degrees); // ['float degrees']
            eac->phaseModulation(degrees);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVETABLE,c,sizeof(gPC_ERISAUDIOSYNTHWAVETABLE)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthWavetable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWavetable* eac = (erisAudioSynthWavetable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float v;
            total_read = sscanf(p, "%f" , &v); // ['float v']
            eac->amplitude(v);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVETABLE,c,sizeof(gPC_ERISAUDIOSYNTHWAVETABLE)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioSynthWavetable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWavetable* eac = (erisAudioSynthWavetable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVETABLE,c,sizeof(gPC_ERISAUDIOSYNTHWAVETABLE)) == 0 &&\
        strncmp(gPM_SETFREQUENCY,m,sizeof(gPM_SETFREQUENCY)) == 0){
        strcpy(objName,erisAudioSynthWavetable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWavetable* eac = (erisAudioSynthWavetable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float freq;
            total_read = sscanf(p, "%f" , &freq); // ['float freq']
            eac->setFrequency(freq);
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVETABLE,c,sizeof(gPC_ERISAUDIOSYNTHWAVETABLE)) == 0 &&\
        strncmp(gPM_GETENVSTATE,m,sizeof(gPM_GETENVSTATE)) == 0){
        strcpy(objName,erisAudioSynthWavetable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWavetable* eac = (erisAudioSynthWavetable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->getEnvState();
        }
    }else if(strncmp(gPC_ERISAUDIOSYNTHNOISEWHITE,c,sizeof(gPC_ERISAUDIOSYNTHNOISEWHITE)) == 0 &&\
        strncmp(gPM_AMPLITUDE,m,sizeof(gPM_AMPLITUDE)) == 0){
        strcpy(objName,erisAudioSynthNoiseWhite::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthNoiseWhite* eac = (erisAudioSynthNoiseWhite*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->amplitude(n);
        }
    }
}