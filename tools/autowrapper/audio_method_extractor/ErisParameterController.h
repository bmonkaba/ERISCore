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
    sci->printf("M APC Request: class: %64s instance: %64s method:%64s params: %64s" , c, i, m, p);//"%64s %64s %64s %64s" , c, i, m, p
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
			uint32_t binNumber;
            total_read = sscanf(p, "%lu" , &binNumber); // ['uint32_t binNumber']
            eac->read(binNumber);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT1024* eac = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t binFirst;
			uint32_t binLast;
            total_read = sscanf(p, "%lu,%lu" , &binFirst,&binLast); // ['uint32_t binFirst', 'uint32_t binLast']
            eac->read(binFirst,binLast);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_AVERAGETOGETHER,m,sizeof(gPM_AVERAGETOGETHER)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT1024* eac = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t n;
            total_read = sscanf(p, "%c" , &n); // ['uint8_t n']
            eac->averageTogether(n);
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_INIT,m,sizeof(gPM_INIT)) == 0){
        strcpy(objName,erisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeFFT1024* eac = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->init();
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
            //total_read = sscanf(p, "" , ); // [' float threshold ']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_THRESHOLD,m,sizeof(gPM_THRESHOLD)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // [' float p ']
            eac->threshold();
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
    }else if(strncmp(gPC_ERISAUDIOANALYZENOTEFREQUENCY,c,sizeof(gPC_ERISAUDIOANALYZENOTEFREQUENCY)) == 0 &&\
        strncmp(gPM_PROCESS,m,sizeof(gPM_PROCESS)) == 0){
        strcpy(objName,erisAudioAnalyzeNoteFrequency::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeNoteFrequency* eac = (erisAudioAnalyzeNoteFrequency*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // [' void ']
            eac->process();
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
    }else if(strncmp(gPC_ERISAUDIOANALYZEPRINT,c,sizeof(gPC_ERISAUDIOANALYZEPRINT)) == 0 &&\
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
            total_read = sscanf(p, "%f,%f" , &level,&edge); // ['float level', 'int edge']
            eac->trigger(level,edge);
        }
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
            //total_read = sscanf(p, "" , ); // ['']
            eac->set_params();
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
    }else if(strncmp(gPC_ERISAUDIOANALYZETONEDETECT,c,sizeof(gPC_ERISAUDIOANALYZETONEDETECT)) == 0 &&\
        strncmp(gPM_BOOL,m,sizeof(gPM_BOOL)) == 0){
        strcpy(objName,erisAudioAnalyzeToneDetect::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioAnalyzeToneDetect* eac = (erisAudioAnalyzeToneDetect*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->bool();
        }
    }else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_GETHALFFILTERLENGTH,m,sizeof(gPM_GETHALFFILTERLENGTH)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getHalfFilterLength();
        }
    }else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_MONITORRESAMPLEBUFFER,m,sizeof(gPM_MONITORRESAMPLEBUFFER)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->monitorResampleBuffer();
        }
    }else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_CONFIGURE,m,sizeof(gPM_CONFIGURE)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->configure();
        }
    }else if(strncmp(gPC_ERISASYNCAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISASYNCAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_CONFIG_SPDIFIN,m,sizeof(gPM_CONFIG_SPDIFIN)) == 0){
        strcpy(objName,erisAsyncAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAsyncAudioInputSPDIF3* eac = (erisAsyncAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->config_spdifIn();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_SETADDRESS,m,sizeof(gPM_SETADDRESS)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t addr;
            total_read = sscanf(p, "%c" , &addr); // ['uint8_t addr']
            eac->setAddress(addr);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_VOLUME,m,sizeof(gPM_VOLUME)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->volume(level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->inputLevel(level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTSELECT,m,sizeof(gPM_INPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->inputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_VOLUME,m,sizeof(gPM_VOLUME)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t channel;
			float level;
            total_read = sscanf(p, "%f,%f" , &channel,&level); // ['int channel', 'float level']
            eac->volume(channel,level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t channel;
			float level;
            total_read = sscanf(p, "%f,%f" , &channel,&level); // ['int channel', 'float level']
            eac->inputLevel(channel,level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_VOLUMEINTEGER,m,sizeof(gPM_VOLUMEINTEGER)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t n;
            total_read = sscanf(p, "%lu" , &n); // ['uint32_t n']
            eac->volumeInteger(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_VOLUMEINTEGER,m,sizeof(gPM_VOLUMEINTEGER)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t channel;
			uint32_t n;
            total_read = sscanf(p, "%f,%lu" , &channel,&n); // ['int channel', 'uint32_t n']
            eac->volumeInteger(channel,n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTLEVELINTEGER,m,sizeof(gPM_INPUTLEVELINTEGER)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t n;
            total_read = sscanf(p, "%li" , &n); // ['int32_t n']
            eac->inputLevelInteger(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTLEVELINTEGER,m,sizeof(gPM_INPUTLEVELINTEGER)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t chnnel;
			int32_t n;
            total_read = sscanf(p, "%f,%li" , &chnnel,&n); // ['int chnnel', 'int32_t n']
            eac->inputLevelInteger(chnnel,n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_VOLUMEBYTE,m,sizeof(gPM_VOLUMEBYTE)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->volumebyte(level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_INPUTLEVELBYTE,m,sizeof(gPM_INPUTLEVELBYTE)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
            total_read = sscanf(p, "%f" , &level); // ['float level']
            eac->inputlevelbyte(level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS42448,c,sizeof(gPC_ERISAUDIOCONTROLCS42448)) == 0 &&\
        strncmp(gPM_WRITE,m,sizeof(gPM_WRITE)) == 0){
        strcpy(objName,erisAudioControlCS42448::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS42448* eac = (erisAudioControlCS42448*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t address;
			uint32_t data;
            total_read = sscanf(p, "%lu,%lu" , &address,&data); // ['uint32_t address', 'uint32_t data']
            eac->write(address,data);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->inputLevel(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_INPUTSELECT,m,sizeof(gPM_INPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->inputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_VOLUME,m,sizeof(gPM_VOLUME)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float left;
			float right;
            total_read = sscanf(p, "%f,%f" , &left,&right); // ['float left', 'float right']
            eac->volume(left,right);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_DACVOLUME,m,sizeof(gPM_DACVOLUME)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float left;
			float right;
            total_read = sscanf(p, "%f,%f" , &left,&right); // ['float left', 'float right']
            eac->dacVolume(left,right);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_MUTEOUTPUT,m,sizeof(gPM_MUTEOUTPUT)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->muteOutput();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_UNMUTEOUTPUT,m,sizeof(gPM_UNMUTEOUTPUT)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->unmuteOutput();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_MUTEINPUT,m,sizeof(gPM_MUTEINPUT)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->muteInput();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_UNMUTEINPUT,m,sizeof(gPM_UNMUTEINPUT)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->unmuteInput();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_ENABLEDITHER,m,sizeof(gPM_ENABLEDITHER)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enableDither();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_DISABLEDITHER,m,sizeof(gPM_DISABLEDITHER)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disableDither();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_WRITE,m,sizeof(gPM_WRITE)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t reg;
			uint32_t val;
            total_read = sscanf(p, "%lu,%lu" , &reg,&val); // ['uint32_t reg', 'uint32_t val']
            eac->write(reg,val);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_VOLUMEINTEGER,m,sizeof(gPM_VOLUMEINTEGER)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t n;
            total_read = sscanf(p, "%lu" , &n); // ['uint32_t n']
            eac->volumeInteger(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLCS4272,c,sizeof(gPC_ERISAUDIOCONTROLCS4272)) == 0 &&\
        strncmp(gPM_INITLOCALREGS,m,sizeof(gPM_INITLOCALREGS)) == 0){
        strcpy(objName,erisAudioControlCS4272::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlCS4272* eac = (erisAudioControlCS4272*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->initLocalRegs();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_SETADDRESS,m,sizeof(gPM_SETADDRESS)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t level;
            total_read = sscanf(p, "%c" , &level); // ['uint8_t level']
            eac->setAddress(level);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->inputLevel(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_MUTEHEADPHONE,m,sizeof(gPM_MUTEHEADPHONE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->muteHeadphone();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_MUTELINEOUT,m,sizeof(gPM_MUTELINEOUT)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->muteLineout();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_INPUTSELECT,m,sizeof(gPM_INPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->inputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_HEADPHONESELECT,m,sizeof(gPM_HEADPHONESELECT)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->headphoneSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_VOLUME,m,sizeof(gPM_VOLUME)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float left;
			float right;
            total_read = sscanf(p, "%f,%f" , &left,&right); // ['float left', 'float right']
            eac->volume(left,right);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_MICGAIN,m,sizeof(gPM_MICGAIN)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t dB;
            total_read = sscanf(p, "%lu" , &dB); // ['uint32_t dB']
            eac->micGain(dB);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_LINEINLEVEL,m,sizeof(gPM_LINEINLEVEL)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t n;
            total_read = sscanf(p, "%c" , &n); // ['uint8_t n']
            eac->lineInLevel(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_LINEINLEVEL,m,sizeof(gPM_LINEINLEVEL)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t left;
			uint8_t right;
            total_read = sscanf(p, "%c,%c" , &left,&right); // ['uint8_t left', 'uint8_t right']
            eac->lineInLevel(left,right);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_DACVOLUMERAMP,m,sizeof(gPM_DACVOLUMERAMP)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->dacVolumeRamp();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_DACVOLUMERAMPLINEAR,m,sizeof(gPM_DACVOLUMERAMPLINEAR)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->dacVolumeRampLinear();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_DACVOLUMERAMPDISABLE,m,sizeof(gPM_DACVOLUMERAMPDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->dacVolumeRampDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ADCHIGHPASSFILTERENABLE,m,sizeof(gPM_ADCHIGHPASSFILTERENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->adcHighPassFilterEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ADCHIGHPASSFILTERFREEZE,m,sizeof(gPM_ADCHIGHPASSFILTERFREEZE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->adcHighPassFilterFreeze();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ADCHIGHPASSFILTERDISABLE,m,sizeof(gPM_ADCHIGHPASSFILTERDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->adcHighPassFilterDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUDIOPREPROCESSORENABLE,m,sizeof(gPM_AUDIOPREPROCESSORENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->audioPreProcessorEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUDIOPOSTPROCESSORENABLE,m,sizeof(gPM_AUDIOPOSTPROCESSORENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->audioPostProcessorEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUDIOPROCESSORDISABLE,m,sizeof(gPM_AUDIOPROCESSORDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->audioProcessorDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_EQBANDS,m,sizeof(gPM_EQBANDS)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float bass;
			float mid_bass;
			float midrange;
			float mid_treble;
			float treble;
            total_read = sscanf(p, "%f,%f,%f,%f,%f" , &bass,&mid_bass,&midrange,&mid_treble,&treble); // ['float bass', 'float mid_bass', 'float midrange', 'float mid_treble', 'float treble']
            eac->eqBands(bass,mid_bass,midrange,mid_treble,treble);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_EQBANDS,m,sizeof(gPM_EQBANDS)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float bass;
			float treble;
            total_read = sscanf(p, "%f,%f" , &bass,&treble); // ['float bass', 'float treble']
            eac->eqBands(bass,treble);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUTOVOLUMEENABLE,m,sizeof(gPM_AUTOVOLUMEENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->autoVolumeEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUTOVOLUMEDISABLE,m,sizeof(gPM_AUTOVOLUMEDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->autoVolumeDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ENHANCEBASSENABLE,m,sizeof(gPM_ENHANCEBASSENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enhanceBassEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_ENHANCEBASSDISABLE,m,sizeof(gPM_ENHANCEBASSDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enhanceBassDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_SURROUNDSOUNDENABLE,m,sizeof(gPM_SURROUNDSOUNDENABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->surroundSoundEnable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_SURROUNDSOUNDDISABLE,m,sizeof(gPM_SURROUNDSOUNDDISABLE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->surroundSoundDisable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_SETMASTERMODE,m,sizeof(gPM_SETMASTERMODE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t freqMCLK_in;
            total_read = sscanf(p, "%lu" , &freqMCLK_in); // ['uint32_t freqMCLK_in']
            eac->setMasterMode(freqMCLK_in);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_VOLUMEINTEGER,m,sizeof(gPM_VOLUMEINTEGER)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t n;
            total_read = sscanf(p, "%lu" , &n); // ['uint32_t n']
            eac->volumeInteger(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t reg;
            total_read = sscanf(p, "%lu" , &reg); // ['uint32_t reg']
            eac->read(reg);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_WRITE,m,sizeof(gPM_WRITE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t reg;
			uint32_t val;
            total_read = sscanf(p, "%lu,%lu" , &reg,&val); // ['uint32_t reg', 'uint32_t val']
            eac->write(reg,val);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_MODIFY,m,sizeof(gPM_MODIFY)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t reg;
			uint32_t val;
			uint32_t iMask;
            total_read = sscanf(p, "%lu,%lu,%lu" , &reg,&val,&iMask); // ['uint32_t reg', 'uint32_t val', 'uint32_t iMask']
            eac->modify(reg,val,iMask);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUTOMATE,m,sizeof(gPM_AUTOMATE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t dap;
			uint8_t eq;
            total_read = sscanf(p, "%c,%c" , &dap,&eq); // ['uint8_t dap', 'uint8_t eq']
            eac->automate(dap,eq);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_AUTOMATE,m,sizeof(gPM_AUTOMATE)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t dap;
			uint8_t eq;
			uint8_t filterCount;
            total_read = sscanf(p, "%c,%c,%c" , &dap,&eq,&filterCount); // ['uint8_t dap', 'uint8_t eq', 'uint8_t filterCount']
            eac->automate(dap,eq,filterCount);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_SETUP,m,sizeof(gPM_SETUP)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->setup();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLSGTL5000,c,sizeof(gPC_ERISAUDIOCONTROLSGTL5000)) == 0 &&\
        strncmp(gPM_LOOP,m,sizeof(gPM_LOOP)) == 0){
        strcpy(objName,erisAudioControlSGTL5000::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlSGTL5000* eac = (erisAudioControlSGTL5000*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->loop();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_OUTPUTSELECT,m,sizeof(gPM_OUTPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->outputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_VOLUME,m,sizeof(gPM_VOLUME)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->volume(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_VOLUME_DB,m,sizeof(gPM_VOLUME_DB)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->volume_dB(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->inputLevel(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_INPUTSELECT,m,sizeof(gPM_INPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->inputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_SETINPUTGAIN_DB,m,sizeof(gPM_SETINPUTGAIN_DB)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->setInputGain_dB(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_SETMICBIAS,m,sizeof(gPM_SETMICBIAS)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->setMicBias(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_ENABLEMICDETECT,m,sizeof(gPM_ENABLEMICDETECT)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['bool']
            eac->enableMicDetect();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_READMICDETECT,m,sizeof(gPM_READMICDETECT)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->readMicDetect();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_READPAGE,m,sizeof(gPM_AIC_READPAGE)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t page;
			uint8_t reg;
            total_read = sscanf(p, "%c,%c" , &page,&reg); // ['uint8_t page', 'uint8_t reg']
            eac->aic_readPage(page,reg);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_WRITEPAGE,m,sizeof(gPM_AIC_WRITEPAGE)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t page;
			uint8_t reg;
			uint8_t val;
            total_read = sscanf(p, "%c,%c,%c" , &page,&reg,&val); // ['uint8_t page', 'uint8_t reg', 'uint8_t val']
            eac->aic_writePage(page,reg,val);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_SETHPFONADC,m,sizeof(gPM_SETHPFONADC)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			bool enable;
			float cutoff_Hz;
			float fs_Hz;
            total_read = sscanf(p, "%c,%f,%f" , &enable,&cutoff_Hz,&fs_Hz); // ['bool enable', 'float cutoff_Hz', 'float fs_Hz']
            eac->setHPFonADC(enable,cutoff_Hz,fs_Hz);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_GETHPCUTOFF_HZ,m,sizeof(gPM_GETHPCUTOFF_HZ)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->getHPCutoff_Hz();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_GETSAMPLERATE_HZ,m,sizeof(gPM_GETSAMPLERATE_HZ)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->getSampleRate_Hz();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_ENABLEAUTOMUTEDAC,m,sizeof(gPM_ENABLEAUTOMUTEDAC)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['bool', 'uint8_t']
            eac->enableAutoMuteDAC();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_RESET,m,sizeof(gPM_AIC_RESET)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->aic_reset();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_INIT,m,sizeof(gPM_AIC_INIT)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->aic_init();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_INITDAC,m,sizeof(gPM_AIC_INITDAC)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->aic_initDAC();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_INITADC,m,sizeof(gPM_AIC_INITADC)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->aic_initADC();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_WRITEADDRESS,m,sizeof(gPM_AIC_WRITEADDRESS)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint16_t address;
			uint8_t val;
            total_read = sscanf(p, "%hu,%c" , &address,&val); // ['uint16_t address', 'uint8_t val']
            eac->aic_writeAddress(address,val);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLTLV320AIC3206,c,sizeof(gPC_ERISAUDIOCONTROLTLV320AIC3206)) == 0 &&\
        strncmp(gPM_AIC_GOTOPAGE,m,sizeof(gPM_AIC_GOTOPAGE)) == 0){
        strcpy(objName,erisAudioControlTLV320AIC3206::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlTLV320AIC3206* eac = (erisAudioControlTLV320AIC3206*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t page;
            total_read = sscanf(p, "%c" , &page); // ['uint8_t page']
            eac->aic_goToPage(page);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_DISABLE,m,sizeof(gPM_DISABLE)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->disable();
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_INPUTLEVEL,m,sizeof(gPM_INPUTLEVEL)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float n;
            total_read = sscanf(p, "%f" , &n); // ['float n']
            eac->inputLevel(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_INPUTSELECT,m,sizeof(gPM_INPUTSELECT)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t n;
            total_read = sscanf(p, "%f" , &n); // ['int n']
            eac->inputSelect(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_WRITE,m,sizeof(gPM_WRITE)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t reg;
			uint32_t val;
            total_read = sscanf(p, "%lu,%lu" , &reg,&val); // ['uint32_t reg', 'uint32_t val']
            eac->write(reg,val);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731,c,sizeof(gPC_ERISAUDIOCONTROLWM8731)) == 0 &&\
        strncmp(gPM_VOLUMEINTEGER,m,sizeof(gPM_VOLUMEINTEGER)) == 0){
        strcpy(objName,erisAudioControlWM8731::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731* eac = (erisAudioControlWM8731*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t n;
            total_read = sscanf(p, "%lu" , &n); // ['uint32_t n']
            eac->volumeInteger(n);
        }
    }else if(strncmp(gPC_ERISAUDIOCONTROLWM8731MASTER,c,sizeof(gPC_ERISAUDIOCONTROLWM8731MASTER)) == 0 &&\
        strncmp(gPM_ENABLE,m,sizeof(gPM_ENABLE)) == 0){
        strcpy(objName,erisAudioControlWM8731master::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioControlWM8731master* eac = (erisAudioControlWM8731master*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->enable();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTBITCRUSHER,c,sizeof(gPC_ERISAUDIOEFFECTBITCRUSHER)) == 0 &&\
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
            total_read = sscanf(p, "%f" , &n_chorus); // ['int n_chorus']
            eac->voices(n_chorus);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDIGITALCOMBINE,c,sizeof(gPC_ERISAUDIOEFFECTDIGITALCOMBINE)) == 0 &&\
        strncmp(gPM_SETCOMBINEMODE,m,sizeof(gPM_SETCOMBINEMODE)) == 0){
        strcpy(objName,erisAudioEffectDigitalCombine::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDigitalCombine* eac = (erisAudioEffectDigitalCombine*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t mode_in;
            total_read = sscanf(p, "%f" , &mode_in); // ['int mode_in']
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
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAY,c,sizeof(gPC_ERISAUDIOEFFECTDELAY)) == 0 &&\
        strncmp(gPM_RECOMPUTE_MAXBLOCKS,m,sizeof(gPM_RECOMPUTE_MAXBLOCKS)) == 0){
        strcpy(objName,erisAudioEffectDelay::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelay* eac = (erisAudioEffectDelay*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->recompute_maxblocks();
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
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAYEXTERNAL,c,sizeof(gPC_ERISAUDIOEFFECTDELAYEXTERNAL)) == 0 &&\
        strncmp(gPM_INITIALIZE,m,sizeof(gPM_INITIALIZE)) == 0){
        strcpy(objName,erisAudioEffectDelayExternal::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelayExternal* eac = (erisAudioEffectDelayExternal*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t samples;
            total_read = sscanf(p, "%lu" , &type,&samples); // ['AudioEffectDelayMemoryType_t type', 'uint32_t samples']
            eac->initialize(type,samples);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTDELAYEXTERNAL,c,sizeof(gPC_ERISAUDIOEFFECTDELAYEXTERNAL)) == 0 &&\
        strncmp(gPM_ZERO,m,sizeof(gPM_ZERO)) == 0){
        strcpy(objName,erisAudioEffectDelayExternal::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectDelayExternal* eac = (erisAudioEffectDelayExternal*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t address;
			uint32_t count;
            total_read = sscanf(p, "%lu,%lu" , &address,&count); // ['uint32_t address', 'uint32_t count']
            eac->zero(address,count);
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
    }else if(strncmp(gPC_ERISAUDIOEFFECTFADE,c,sizeof(gPC_ERISAUDIOEFFECTFADE)) == 0 &&\
        strncmp(gPM_FADEBEGIN,m,sizeof(gPM_FADEBEGIN)) == 0){
        strcpy(objName,erisAudioEffectFade::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectFade* eac = (erisAudioEffectFade*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t newrate;
			uint8_t dir;
            total_read = sscanf(p, "%lu,%c" , &newrate,&dir); // ['uint32_t newrate', 'uint8_t dir']
            eac->fadeBegin(newrate,dir);
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
            total_read = sscanf(p, "%f,%f,%f" , &delay_offset,&d_depth,&delay_rate); // ['int delay_offset', 'int d_depth', 'float delay_rate']
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
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_BEGINFREEZE_INT,m,sizeof(gPM_BEGINFREEZE_INT)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t grain_samples;
            total_read = sscanf(p, "%f" , &grain_samples); // ['int grain_samples']
            eac->beginFreeze_int(grain_samples);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTGRANULAR,c,sizeof(gPC_ERISAUDIOEFFECTGRANULAR)) == 0 &&\
        strncmp(gPM_BEGINPITCHSHIFT_INT,m,sizeof(gPM_BEGINPITCHSHIFT_INT)) == 0){
        strcpy(objName,erisAudioEffectGranular::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectGranular* eac = (erisAudioEffectGranular*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t grain_samples;
            total_read = sscanf(p, "%f" , &grain_samples); // ['int grain_samples']
            eac->beginPitchShift_int(grain_samples);
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTREVERB,c,sizeof(gPC_ERISAUDIOEFFECTREVERB)) == 0 &&\
        strncmp(gPM_REVERBTIME,m,sizeof(gPM_REVERBTIME)) == 0){
        strcpy(objName,erisAudioEffectReverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectReverb* eac = (erisAudioEffectReverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['float']
            eac->reverbTime();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTREVERB,c,sizeof(gPC_ERISAUDIOEFFECTREVERB)) == 0 &&\
        strncmp(gPM_INIT_COMB_FILTERS,m,sizeof(gPM_INIT_COMB_FILTERS)) == 0){
        strcpy(objName,erisAudioEffectReverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectReverb* eac = (erisAudioEffectReverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->init_comb_filters();
        }
    }else if(strncmp(gPC_ERISAUDIOEFFECTREVERB,c,sizeof(gPC_ERISAUDIOEFFECTREVERB)) == 0 &&\
        strncmp(gPM_CLEAR_BUFFERS,m,sizeof(gPM_CLEAR_BUFFERS)) == 0){
        strcpy(objName,erisAudioEffectReverb::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioEffectReverb* eac = (erisAudioEffectReverb*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->clear_buffers();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_RESET,m,sizeof(gPM_RESET)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->reset();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_ENABLEFFT,m,sizeof(gPM_ENABLEFFT)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			bool enable_state;
            total_read = sscanf(p, "%c" , &enable_state); // ['bool enable_state']
            eac->enableFFT(enable_state);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_CONFIGSUBSAMPLE,m,sizeof(gPM_CONFIGSUBSAMPLE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint16_t subsample;
            total_read = sscanf(p, "%hu" , &subsample,&range); // ['uint16_t subsample', 'subsample_range range']
            eac->configSubsample(subsample,range);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_SETACTIVERANGE,m,sizeof(gPM_SETACTIVERANGE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            total_read = sscanf(p, "" , &range); // ['subsample_range range']
            eac->setActiveRange(range);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_TOGGLEACTIVERANGE,m,sizeof(gPM_TOGGLEACTIVERANGE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->toggleActiveRange();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->available();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_READ,m,sizeof(gPM_READ)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t binNumber;
            total_read = sscanf(p, "%lu" , &binNumber); // ['uint32_t binNumber']
            eac->read(binNumber);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_AVERAGETOGETHER,m,sizeof(gPM_AVERAGETOGETHER)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t n;
            total_read = sscanf(p, "%c" , &n); // ['uint8_t n']
            eac->averageTogether(n);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_SPECTRALFILTER,m,sizeof(gPM_SPECTRALFILTER)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->spectralFilter();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_CAPTURE,m,sizeof(gPM_CAPTURE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->capture();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_ANALYZE,m,sizeof(gPM_ANALYZE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->analyze();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZEFFT1024,c,sizeof(gPC_ERISERISAUDIOANALYZEFFT1024)) == 0 &&\
        strncmp(gPM_INIT,m,sizeof(gPM_INIT)) == 0){
        strcpy(objName,eriserisAudioAnalyzeFFT1024::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeFFT1024* eac = (eriserisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->init();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_TRIGGER,m,sizeof(gPM_TRIGGER)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->trigger();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_TRIGGER,m,sizeof(gPM_TRIGGER)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float level;
			int16_t edge;
            total_read = sscanf(p, "%f,%f" , &level,&edge); // ['float level', 'int edge']
            eac->trigger(level,edge);
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_AVAILABLE,m,sizeof(gPM_AVAILABLE)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->available();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTPRODUCT,m,sizeof(gPM_GETDOTPRODUCT)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotProduct();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTPRODUCTAVG,m,sizeof(gPM_GETDOTPRODUCTAVG)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotProductAvg();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTPRODUCTAVGSLOW,m,sizeof(gPM_GETDOTPRODUCTAVGSLOW)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotProductAvgSlow();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTMACD,m,sizeof(gPM_GETDOTMACD)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotMACD();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTDELTA,m,sizeof(gPM_GETDOTDELTA)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotDelta();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTACCELERATION,m,sizeof(gPM_GETDOTACCELERATION)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotAcceleration();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETDOTDELTAMACD,m,sizeof(gPM_GETDOTDELTAMACD)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getDotDeltaMACD();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETEDGECOUNT,m,sizeof(gPM_GETEDGECOUNT)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getEdgeCount();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETEDGECOUNT_CH2,m,sizeof(gPM_GETEDGECOUNT_CH2)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getEdgeCount_ch2();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETEDGEDELAY,m,sizeof(gPM_GETEDGEDELAY)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getEdgeDelay();
        }
    }else if(strncmp(gPC_ERISERISAUDIOANALYZESCOPE,c,sizeof(gPC_ERISERISAUDIOANALYZESCOPE)) == 0 &&\
        strncmp(gPM_GETEDGEDELAY2,m,sizeof(gPM_GETEDGEDELAY2)) == 0){
        strcpy(objName,eriserisAudioAnalyzeScope::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        eriserisAudioAnalyzeScope* eac = (eriserisAudioAnalyzeScope*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getEdgeDelay2();
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
            total_read = sscanf(p, "" , &t_type); // ['short t_type']
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
            total_read = sscanf(p, "%f,%f" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
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
            total_read = sscanf(p, "" , &t_type); // ['short t_type']
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
            total_read = sscanf(p, "%f,%f" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
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
    }else if(strncmp(gPC_ERISAUDIOFILTERFIR,c,sizeof(gPC_ERISAUDIOFILTERFIR)) == 0 &&\
        strncmp(gPM_END,m,sizeof(gPM_END)) == 0){
        strcpy(objName,erisAudioFilterFIR::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterFIR* eac = (erisAudioFilterFIR*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->end();
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_FREQUENCY,m,sizeof(gPM_FREQUENCY)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float FC;
            total_read = sscanf(p, "%f" , &FC); // ['float FC']
            eac->frequency(FC);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_RESONANCE,m,sizeof(gPM_RESONANCE)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float reson;
            total_read = sscanf(p, "%f" , &reson); // ['float reson']
            eac->resonance(reson);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_OCTAVECONTROL,m,sizeof(gPM_OCTAVECONTROL)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float octaves;
            total_read = sscanf(p, "%f" , &octaves); // ['float octaves']
            eac->octaveControl(octaves);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_PASSBANDGAIN,m,sizeof(gPM_PASSBANDGAIN)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float passbandgain;
            total_read = sscanf(p, "%f" , &passbandgain); // ['float passbandgain']
            eac->passbandGain(passbandgain);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_INPUTDRIVE,m,sizeof(gPM_INPUTDRIVE)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float drv;
            total_read = sscanf(p, "%f" , &drv); // ['float drv']
            eac->inputDrive(drv);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_INTERPOLATIONMETHOD,m,sizeof(gPM_INTERPOLATIONMETHOD)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            total_read = sscanf(p, "" , &im); // ['AudioFilterLadderInterpolation im']
            eac->interpolationMethod(im);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_COMPUTE_COEFFS,m,sizeof(gPM_COMPUTE_COEFFS)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float fc;
            total_read = sscanf(p, "%f" , &fc); // ['float fc']
            eac->compute_coeffs(fc);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_INITPOLY,m,sizeof(gPM_INITPOLY)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->initpoly();
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERLADDER,c,sizeof(gPC_ERISAUDIOFILTERLADDER)) == 0 &&\
        strncmp(gPM_RESONATING,m,sizeof(gPM_RESONATING)) == 0){
        strcpy(objName,erisAudioFilterLadder::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterLadder* eac = (erisAudioFilterLadder*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->resonating();
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
    }else if(strncmp(gPC_ERISAUDIOINPUTI2S2SLAVE,c,sizeof(gPC_ERISAUDIOINPUTI2S2SLAVE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputI2S2slave::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputI2S2slave* eac = (erisAudioInputI2S2slave*) (ad->getAudioStreamObjByName(objName));
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
    }else if(strncmp(gPC_ERISAUDIOINPUTPDM,c,sizeof(gPC_ERISAUDIOINPUTPDM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputPDM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputPDM* eac = (erisAudioInputPDM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTPDM2,c,sizeof(gPC_ERISAUDIOINPUTPDM2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputPDM2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputPDM2* eac = (erisAudioInputPDM2*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTSPDIF3,c,sizeof(gPC_ERISAUDIOINPUTSPDIF3)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputSPDIF3::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputSPDIF3* eac = (erisAudioInputSPDIF3*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTTDM,c,sizeof(gPC_ERISAUDIOINPUTTDM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputTDM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputTDM* eac = (erisAudioInputTDM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOINPUTTDM2,c,sizeof(gPC_ERISAUDIOINPUTTDM2)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioInputTDM2::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioInputTDM2* eac = (erisAudioInputTDM2*) (ad->getAudioStreamObjByName(objName));
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
            total_read = sscanf(p, "%f" , &ref); // ['int ref']
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
            total_read = sscanf(p, "%f" , &ref); // ['int ref']
            eac->analogReference(ref);
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
    }else if(strncmp(gPC_ERISAUDIOOUTPUTI2S2SLAVE,c,sizeof(gPC_ERISAUDIOOUTPUTI2S2SLAVE)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputI2S2slave::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputI2S2slave* eac = (erisAudioOutputI2S2slave*) (ad->getAudioStreamObjByName(objName));
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
    }else if(strncmp(gPC_ERISAUDIOOUTPUTMQS,c,sizeof(gPC_ERISAUDIOOUTPUTMQS)) == 0 &&\
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
    }else if(strncmp(gPC_ERISAUDIOOUTPUTPWM,c,sizeof(gPC_ERISAUDIOOUTPUTPWM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputPWM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputPWM* eac = (erisAudioOutputPWM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOOUTPUTPWM,c,sizeof(gPC_ERISAUDIOOUTPUTPWM)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioOutputPWM::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioOutputPWM* eac = (erisAudioOutputPWM*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint8_t pin1;
			uint8_t pin2;
            total_read = sscanf(p, "%c,%c" , &pin1,&pin2); // ['uint8_t pin1', 'uint8_t pin2']
            eac->begin(pin1,pin2);
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
    }else if(strncmp(gPC_ERISAUDIOPLAYMEMORY,c,sizeof(gPC_ERISAUDIOPLAYMEMORY)) == 0 &&\
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
    }else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_PLAY,m,sizeof(gPM_PLAY)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t data;
            total_read = sscanf(p, "%hi" , &data); // ['int16_t data']
            eac->play(data);
        }
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
    }else if(strncmp(gPC_ERISAUDIOPLAYQUEUE,c,sizeof(gPC_ERISAUDIOPLAYQUEUE)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlayQueue::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlayQueue* eac = (erisAudioPlayQueue*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDRAW,c,sizeof(gPC_ERISAUDIOPLAYSDRAW)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioPlaySdRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdRaw* eac = (erisAudioPlaySdRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDRAW,c,sizeof(gPC_ERISAUDIOPLAYSDRAW)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlaySdRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdRaw* eac = (erisAudioPlaySdRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDRAW,c,sizeof(gPC_ERISAUDIOPLAYSDRAW)) == 0 &&\
        strncmp(gPM_ISPLAYING,m,sizeof(gPM_ISPLAYING)) == 0){
        strcpy(objName,erisAudioPlaySdRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdRaw* eac = (erisAudioPlaySdRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPlaying();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDRAW,c,sizeof(gPC_ERISAUDIOPLAYSDRAW)) == 0 &&\
        strncmp(gPM_POSITIONMILLIS,m,sizeof(gPM_POSITIONMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySdRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdRaw* eac = (erisAudioPlaySdRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->positionMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDRAW,c,sizeof(gPC_ERISAUDIOPLAYSDRAW)) == 0 &&\
        strncmp(gPM_LENGTHMILLIS,m,sizeof(gPM_LENGTHMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySdRaw::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdRaw* eac = (erisAudioPlaySdRaw*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->lengthMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_BEGIN,m,sizeof(gPM_BEGIN)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->begin();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_TOGGLEPLAYPAUSE,m,sizeof(gPM_TOGGLEPLAYPAUSE)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->togglePlayPause();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_STOP,m,sizeof(gPM_STOP)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->stop();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_ISPLAYING,m,sizeof(gPM_ISPLAYING)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPlaying();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_ISPAUSED,m,sizeof(gPM_ISPAUSED)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isPaused();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_ISSTOPPED,m,sizeof(gPM_ISSTOPPED)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->isStopped();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_POSITIONMILLIS,m,sizeof(gPM_POSITIONMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->positionMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_LENGTHMILLIS,m,sizeof(gPM_LENGTHMILLIS)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->lengthMillis();
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_CONSUME,m,sizeof(gPM_CONSUME)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t size;
            total_read = sscanf(p, "%lu" , &size); // ['uint32_t size']
            eac->consume(size);
        }
    }else if(strncmp(gPC_ERISAUDIOPLAYSDWAV,c,sizeof(gPC_ERISAUDIOPLAYSDWAV)) == 0 &&\
        strncmp(gPM_PARSE_FORMAT,m,sizeof(gPM_PARSE_FORMAT)) == 0){
        strcpy(objName,erisAudioPlaySdWav::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioPlaySdWav* eac = (erisAudioPlaySdWav*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['void']
            eac->parse_format();
        }
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
    }else if(strncmp(gPC_ERISQUANTIZER,c,sizeof(gPC_ERISQUANTIZER)) == 0 &&\
        strncmp(gPM_CONFIGURE,m,sizeof(gPM_CONFIGURE)) == 0){
        strcpy(objName,erisQuantizer::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisQuantizer* eac = (erisQuantizer*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			bool noiseShaping;
			bool dither;
			float factor;
            total_read = sscanf(p, "%c,%c,%f" , &noiseShaping,&dither,&factor); // ['bool noiseShaping', 'bool dither', 'float factor']
            eac->configure(noiseShaping,dither,factor);
        }
    }else if(strncmp(gPC_ERISQUANTIZER,c,sizeof(gPC_ERISQUANTIZER)) == 0 &&\
        strncmp(gPM_RESET,m,sizeof(gPM_RESET)) == 0){
        strcpy(objName,erisQuantizer::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisQuantizer* eac = (erisQuantizer*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->reset();
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
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_RESET,m,sizeof(gPM_RESET)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->reset();
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_CONFIGURE,m,sizeof(gPM_CONFIGURE)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float fs;
			float newFs;
            total_read = sscanf(p, "%f,%f" , &fs,&newFs); // ['float fs', 'float newFs']
            eac->configure(fs,newFs);
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_ADDTOSAMPLEDIFF,m,sizeof(gPM_ADDTOSAMPLEDIFF)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t diff;
            total_read = sscanf(p, "%Lf" , &diff); // ['double diff']
            eac->addToSampleDiff(diff);
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_ADDTOPOS,m,sizeof(gPM_ADDTOPOS)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t val;
            total_read = sscanf(p, "%Lf" , &val); // ['double val']
            eac->addToPos(val);
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_FIXSTEP,m,sizeof(gPM_FIXSTEP)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->fixStep();
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_INITIALIZED,m,sizeof(gPM_INITIALIZED)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->initialized();
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_GETHALFFILTERLENGTH,m,sizeof(gPM_GETHALFFILTERLENGTH)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
            //total_read = sscanf(p, "" , ); // ['']
            eac->getHalfFilterLength();
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_GETKAISEREXACT,m,sizeof(gPM_GETKAISEREXACT)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float beta;
            total_read = sscanf(p, "%f" , &beta); // ['float beta']
            eac->getKaiserExact(beta);
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_SETKAISERWINDOW,m,sizeof(gPM_SETKAISERWINDOW)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float beta;
			int32_t noSamples;
            total_read = sscanf(p, "%f,%li" , &beta,&noSamples); // ['float beta', 'int32_t noSamples']
            eac->setKaiserWindow(beta,noSamples);
        }
    }else if(strncmp(gPC_ERISRESAMPLER,c,sizeof(gPC_ERISRESAMPLER)) == 0 &&\
        strncmp(gPM_SETFILTER,m,sizeof(gPM_SETFILTER)) == 0){
        strcpy(objName,erisResampler::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisResampler* eac = (erisResampler*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int32_t halfFiltLength;
			int32_t overSampling;
			float cutOffFrequ;
			float kaiserBeta;
            total_read = sscanf(p, "%li,%li,%f,%f" , &halfFiltLength,&overSampling,&cutOffFrequ,&kaiserBeta); // ['int32_t halfFiltLength', 'int32_t overSampling', 'float cutOffFrequ', 'float kaiserBeta']
            eac->setFilter(halfFiltLength,overSampling,cutOffFrequ,kaiserBeta);
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
        erisAudioSynthToneSweep* eac = (erisAudioSynthToneSweep*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			float t_amp;
			int16_t t_lo;
			int16_t t_hi;
			float t_time;
            total_read = sscanf(p, "%f,%f,%f,%f" , &t_amp,&t_lo,&t_hi,&t_time); // ['float t_amp', 'int t_lo', 'int t_hi', 'float t_time']
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
            total_read = sscanf(p, "" , &t_type); // ['short t_type']
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
            total_read = sscanf(p, "%f,%f" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
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
            total_read = sscanf(p, "" , &t_type); // ['short t_type']
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
            total_read = sscanf(p, "%f,%f" , &t_amp,&t_freq,&t_type); // ['float t_amp', 'float t_freq', 'short t_type']
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
    }else if(strncmp(gPC_ERISAUDIOSYNTHWAVETABLE,c,sizeof(gPC_ERISAUDIOSYNTHWAVETABLE)) == 0 &&\
        strncmp(gPM_SETSTATE,m,sizeof(gPM_SETSTATE)) == 0){
        strcpy(objName,erisAudioSynthWavetable::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioSynthWavetable* eac = (erisAudioSynthWavetable*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			int16_t note;
			int16_t amp;
			float freq;
            total_read = sscanf(p, "%f,%f,%f" , &note,&amp,&freq); // ['int note', 'int amp', 'float freq']
            eac->setState(note,amp,freq);
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
    }}
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
const char gPC_ERISERISAUDIOANALYZEFFT1024[] PROGMEM = "eriserisAudioAnalyzeFFT1024";
const char gPC_ERISERISAUDIOANALYZESCOPE[] PROGMEM = "eriserisAudioAnalyzeScope";
const char gPC_ERISAUDIOSYNTHWAVEFORM[] PROGMEM = "erisAudioSynthWaveform";
const char gPC_ERISAUDIOSYNTHWAVEFORMMODULATED[] PROGMEM = "erisAudioSynthWaveformModulated";
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
const char gPC_ERISAUDIOSYNTHWAVETABLE[] PROGMEM = "erisAudioSynthWavetable";
const char gPC_ERISAUDIOSYNTHNOISEWHITE[] PROGMEM = "erisAudioSynthNoiseWhite";
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
const char gPM_RESET[] PROGMEM = "reset";
const char gPM_ENABLEFFT[] PROGMEM = "enableFFT";
const char gPM_CONFIGSUBSAMPLE[] PROGMEM = "configSubsample";
const char gPM_SETACTIVERANGE[] PROGMEM = "setActiveRange";
const char gPM_TOGGLEACTIVERANGE[] PROGMEM = "toggleActiveRange";
const char gPM_SPECTRALFILTER[] PROGMEM = "spectralFilter";
const char gPM_CAPTURE[] PROGMEM = "capture";
const char gPM_ANALYZE[] PROGMEM = "analyze";
const char gPM_GETDOTPRODUCT[] PROGMEM = "getDotProduct";
const char gPM_GETDOTPRODUCTAVG[] PROGMEM = "getDotProductAvg";
const char gPM_GETDOTPRODUCTAVGSLOW[] PROGMEM = "getDotProductAvgSlow";
const char gPM_GETDOTMACD[] PROGMEM = "getDotMACD";
const char gPM_GETDOTDELTA[] PROGMEM = "getDotDelta";
const char gPM_GETDOTACCELERATION[] PROGMEM = "getDotAcceleration";
const char gPM_GETDOTDELTAMACD[] PROGMEM = "getDotDeltaMACD";
const char gPM_GETEDGECOUNT[] PROGMEM = "getEdgeCount";
const char gPM_GETEDGECOUNT_CH2[] PROGMEM = "getEdgeCount_ch2";
const char gPM_GETEDGEDELAY[] PROGMEM = "getEdgeDelay";
const char gPM_GETEDGEDELAY2[] PROGMEM = "getEdgeDelay2";
const char gPM_FREQUENCY[] PROGMEM = "frequency";
const char gPM_PHASE[] PROGMEM = "phase";
const char gPM_AMPLITUDE[] PROGMEM = "amplitude";
const char gPM_OFFSET[] PROGMEM = "offset";
const char gPM_PULSEWIDTH[] PROGMEM = "pulseWidth";
const char gPM_FREQUENCYMODULATION[] PROGMEM = "frequencyModulation";
const char gPM_PHASEMODULATION[] PROGMEM = "phaseModulation";
const char gPM_END[] PROGMEM = "end";
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
const char gPM_CLEAR[] PROGMEM = "clear";
const char gPM_FREEBUFFER[] PROGMEM = "freeBuffer";
const char gPM_ADDTOSAMPLEDIFF[] PROGMEM = "addToSampleDiff";
const char gPM_ADDTOPOS[] PROGMEM = "addToPos";
const char gPM_FIXSTEP[] PROGMEM = "fixStep";
const char gPM_INITIALIZED[] PROGMEM = "initialized";
const char gPM_GETKAISEREXACT[] PROGMEM = "getKaiserExact";
const char gPM_SETKAISERWINDOW[] PROGMEM = "setKaiserWindow";
const char gPM_SETFILTER[] PROGMEM = "setFilter";
const char gPM_LENGTH[] PROGMEM = "length";
const char gPM_SECONDMIX[] PROGMEM = "secondMix";
const char gPM_PITCHMOD[] PROGMEM = "pitchMod";
const char gPM_SETFREQUENCY[] PROGMEM = "setFrequency";
const char gPM_GETENVSTATE[] PROGMEM = "getEnvState";
const char gPM_SETSTATE[] PROGMEM = "setState";
