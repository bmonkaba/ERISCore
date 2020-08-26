#ifndef __AudioDirector
#define  __AudioDirector
#include <String.h>
#include "erisAudio.h"

#define MAX_AUDIO_STREAM_OBJECTS 50
#define MAX_AUDIO_MEMORY_BLOCKS 5
#define MAX_CATEGORIES 16
#define MAX_UNIQUE_NAMES_PER_CATEGORY 16
#define MAX_CONNECTIONS 20

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
    bool connect(erisAudioStream* source, int sourceOutput, erisAudioStream* destination,int destinationInput);
    bool connect(char* from,uint8_t from_port,erisAudioStream* to,uint8_t to_port);
    bool connect(const char* connectionString);
    void disconnect();
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
  pAudioStreamOutputPort = new erisAudioOutputI2S2;
  addAudioStreamObj(pAudioStreamInputPort);
  addAudioStreamObj(pAudioStreamOutputPort);
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeNoteFrequency);
  //generate audio component pool
  for (int i=0; i < 2; i++){
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

  delay(1000);

  Serial.println("AudioDirector::AudioDirector() building AudioConnection pool");
  for(uint16_t i=0; i < MAX_CONNECTIONS;i++) pCord[i] = NULL;
  Serial.println("AudioDirector::AudioDirector() connecting AudioStreamInputPort to pAudioStreamOutputPort");
  connect((erisAudioStream*)pAudioStreamInputPort,0,(erisAudioStream*)pAudioStreamOutputPort,0);
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


bool AudioDirector::connect(erisAudioStream* source, int sourceOutput, erisAudioStream* destination,int destinationInput){
  if (NULL==source||NULL==destination) return false;
  //find free connection
  uint16_t i;
  for(i=0; i < MAX_CONNECTIONS;i++){
    if (NULL==pCord[i]){
      Serial.print("AudioDirector::connect() making a new AudioConnection at index ");
      Serial.println(i);
      Serial.print(source->shortName);
      Serial.print(" -> ");
      Serial.println(destination->shortName);
      //pCord[i] = new erisAudioConnection(*source, (unsigned char)sourceOutput,*destination, (unsigned char)destinationInput);
      return true; 
    }
  }
  return false; //no empty connection slots  
}

void AudioDirector::disconnect(){
  
}

bool AudioDirector::connect(char* from,uint8_t from_port,erisAudioStream* to,uint8_t to_port){
  return connect(getAudioStreamObjByName((char* )from),from_port,getAudioStreamObjByName((char* )from),to_port);
}
bool AudioDirector::connect(const char* connectionString){
  //parse connectionString 
  //format: "SRCTYPE(CHAR)_INSTANCE(INT) SRCPORT(INT) DESTTYPE(CHAR)_INSTANCE(INT) DESTPORT(INT)"
  //example: "MIXER_2 0 CHORUS_1 1"
  //the example above connects mixer instance 2 output 0 with chorus instance 1 input 1
  return true;
}

#endif
