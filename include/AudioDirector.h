#ifndef __AudioDirector
#define  __AudioDirector
#include <String.h>
#include "erisAudio.h"

#define MAX_AUDIO_STREAM_OBJECTS 100
#define MAX_AUDIO_MEMORY_BLOCKS 20
#define MAX_CATEGORIES 16
#define MAX_UNIQUE_NAMES_PER_CATEGORY 16
#define MAX_CONNECTIONS 50

const char* nullStr = "NULL";


/*
Audio director class is responsible for proving an interface for the managment 
of the audio blocks, associated controllers and connection sets.
*/
class AudioDirector{
  public:
    AudioDirector();
    void activateConnectionGroup(uint16_t group_id);
    bool addAudioStreamObj(erisAudioStream* obj);
    bool connect(char* from,uint8_t from_port,erisAudioStream* to,uint8_t to_port);
  protected:
    erisAudioStream* pAudioStreamObjPool[MAX_AUDIO_STREAM_OBJECTS]; //Generic Object Pool
    erisAudioStream* pAudioStreamInputPort; //ADC Audio Input(s)
    erisAudioStream* pAudioStreamOutputPort; //DAC Audio Output(s)
    void unlinkAll();
    void linkGroup();
    void generateCategoryList();
    erisAudioStream* getAudioStreamObjByName(const char* AudioStreamObjName);
    uint16_t activeConnectionGroup;  
    uint16_t objCount;
    uint16_t categoryCount;
    uint16_t shortNameQueryResultCount;
    char** categoryList[MAX_CATEGORIES];
    char* shortNameQueryResult[MAX_UNIQUE_NAMES_PER_CATEGORY];
    erisAudioConnection* pCord[MAX_CONNECTIONS];
    
    // GUItool: begin automatically generated code
};

AudioDirector::AudioDirector(){
  objCount=0;
  categoryCount=0;
  shortNameQueryResultCount=0;
  activeConnectionGroup=0;
  AudioMemory(MAX_AUDIO_MEMORY_BLOCKS);
  //init pointer arrays
  for (uint16_t i=0; i < MAX_UNIQUE_NAMES_PER_CATEGORY; i++){
    shortNameQueryResult[i]=0;
  }
  for (uint16_t j = 0; j < MAX_CATEGORIES; j++){
    categoryList[j] = (char**)&nullStr;
  }

  pAudioStreamInputPort = new erisAudioInputI2S;
  pAudioStreamOutputPort = new erisAudioOutputI2S;
  addAudioStreamObj(pAudioStreamInputPort);
  addAudioStreamObj(pAudioStreamOutputPort);
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeNoteFrequency);
  //generate audio component pool
  for (int i=0; i < 4; i++){
    addAudioStreamObj(new erisAudioEffectWaveshaper);
    addAudioStreamObj(new erisAudioEffectEnvelope);
    addAudioStreamObj(new erisAudioSynthWaveformModulated);
    addAudioStreamObj(new erisAudioFilterStateVariable);
  }

  for (int i=0; i < 4; i++){
    addAudioStreamObj(new erisAudioMixer4);
    addAudioStreamObj(new erisAudioAnalyzePeak);
    addAudioStreamObj(new erisAudioAnalyzeRMS);
  }

  Serial.print("AudioDirector::AudioDirector() objects: ");
  Serial.println(objCount);

  generateCategoryList();
  Serial.print("AudioDirector::AudioDirector() object categories: ");
  Serial.println(categoryCount);
  for (uint16_t j = 0; j < categoryCount; j++)Serial.println(*categoryList[j]);

  erisAudioStream* test = getAudioStreamObjByName("mixer_2");
  Serial.println("AudioDirector::AudioDirector() getAudioStreamObjByName test: (should be mixer #2)");
  Serial.print(test->shortName);
  Serial.print("_");
  Serial.println(test->instance);


  Serial.println("AudioDirector::AudioDirector() building AudioConnection pool");
  for(uint16_t i=0; i < MAX_CONNECTIONS;i++) pCord[i] = new erisAudioConnection();
  pCord[0]->makeConnection(*test,0,*test,1);
};

bool AudioDirector::addAudioStreamObj(erisAudioStream* obj){
  uint8_t count = 0; 

  if (obj !=0 && objCount < MAX_AUDIO_STREAM_OBJECTS){
    pAudioStreamObjPool[objCount++] = obj;
    Serial.print("AudioDirector::addAudioStreamObj adding obj ");
    Serial.print(obj->category);
    Serial.print("->");
    Serial.println(obj->shortName);  
    if (objCount > 0){
      for(uint16_t i=0; i < objCount;i++){
        if(strcmp(obj->shortName,pAudioStreamObjPool[i]->shortName)==0) count += 1;
      }
    }
    obj->instance = count;
    Serial.print("AudioDirector::addAudioStreamObj obj type instance ");
    Serial.println(obj->instance);
    return true;
  }
  return false;
};

void AudioDirector::generateCategoryList(){
  //called from the constructor
  bool found;
  categoryCount=0;  
  for (uint16_t i = 0; i < objCount; i++){           //for each object
    found = false;
    for (uint16_t j = 0; j < MAX_CATEGORIES; j++){   //check if cat name is already in the list
      if (categoryList[j] != 0){ //dont test unitialized string pointers
        if (strcmp(*categoryList[j],pAudioStreamObjPool[i]->category)==0) found = true;
      } 
    }
    if (found==false) categoryList[categoryCount++] = (char**)&pAudioStreamObjPool[i]->category; //add to the list if not existing
    if (categoryCount==MAX_CATEGORIES) return; //limit the result list to the size of the category buffer 
  }
}

erisAudioStream* AudioDirector::getAudioStreamObjByName(const char* AudioStreamObjName){
  //string name needs to be deccompsed to type and instance
  char *pch;
  char *ptr;
  pch = strstr (AudioStreamObjName,"_");
  uint8_t instance = (uint8_t)strtoul(pch+1,&ptr,10);//instance starts 1 char from '_' and is always base 10

  for (uint16_t i = 0; i < objCount; i++){
    //test the index first, then the string
    if (instance == pAudioStreamObjPool[i]->instance){
        if (0==strncmp(AudioStreamObjName,pAudioStreamObjPool[i]->shortName,strlen(pAudioStreamObjPool[i]->shortName)-1)) return pAudioStreamObjPool[i];
    }
  }
  //not found return null
  return 0;
}

#endif
