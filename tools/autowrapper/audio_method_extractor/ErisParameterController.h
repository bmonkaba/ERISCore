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
    if(0){}else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETLOWPASS,m,sizeof(gPM_SETLOWPASS)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float q;
            total_read = sscanf(p, "%lu,%f,%f" , &stage,&frequency,&q); // ['uint32_t stage', 'float frequency', 'float q = 0.7071f']
            eac->setLowpass(stage,frequency,q);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETHIGHPASS,m,sizeof(gPM_SETHIGHPASS)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float q;
            total_read = sscanf(p, "%lu,%f,%f" , &stage,&frequency,&q); // ['uint32_t stage', 'float frequency', 'float q = 0.7071']
            eac->setHighpass(stage,frequency,q);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETBANDPASS,m,sizeof(gPM_SETBANDPASS)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float q;
            total_read = sscanf(p, "%lu,%f,%f" , &stage,&frequency,&q); // ['uint32_t stage', 'float frequency', 'float q = 1.0']
            eac->setBandpass(stage,frequency,q);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETNOTCH,m,sizeof(gPM_SETNOTCH)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float q;
            total_read = sscanf(p, "%lu,%f,%f" , &stage,&frequency,&q); // ['uint32_t stage', 'float frequency', 'float q = 1.0']
            eac->setNotch(stage,frequency,q);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETLOWSHELF,m,sizeof(gPM_SETLOWSHELF)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float gain;
			float slope;
            total_read = sscanf(p, "%lu,%f,%f,%f" , &stage,&frequency,&gain,&slope); // ['uint32_t stage', 'float frequency', 'float gain', 'float slope = 1.0f']
            eac->setLowShelf(stage,frequency,gain,slope);
        }
    }else if(strncmp(gPC_ERISAUDIOFILTERBIQUAD,c,sizeof(gPC_ERISAUDIOFILTERBIQUAD)) == 0 &&\
        strncmp(gPM_SETHIGHSHELF,m,sizeof(gPM_SETHIGHSHELF)) == 0){
        strcpy(objName,erisAudioFilterBiquad::short_name_lookup);
        strcat(objName,"_");strcat(objName,i);
        erisAudioFilterBiquad* eac = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName(objName));
        if (eac != NULL){
			uint32_t stage;
			float frequency;
			float gain;
			float slope;
            total_read = sscanf(p, "%lu,%f,%f,%f" , &stage,&frequency,&gain,&slope); // ['uint32_t stage', 'float frequency', 'float gain', 'float slope = 1.0f']
            eac->setHighShelf(stage,frequency,gain,slope);
        }
    }}
const char gPC_ERISAUDIOFILTERBIQUAD[] PROGMEM = "erisAudioFilterBiquad";
const char gPM_SETLOWPASS[] PROGMEM = "setLowpass";
const char gPM_SETHIGHPASS[] PROGMEM = "setHighpass";
const char gPM_SETBANDPASS[] PROGMEM = "setBandpass";
const char gPM_SETNOTCH[] PROGMEM = "setNotch";
const char gPM_SETLOWSHELF[] PROGMEM = "setLowShelf";
const char gPM_SETHIGHSHELF[] PROGMEM = "setHighShelf";
