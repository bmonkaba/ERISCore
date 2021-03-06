#ifndef __AudioDirector
#define  __AudioDirector
#include <string.h>
#include "erisAudio.h"
#include "eris_analyze_fft1024.h"
#include "eris_analyze_scope.h"

#define MAX_AUDIO_STREAM_OBJECTS 150
#define MAX_AUDIO_MEMORY_BLOCKS 38
#define MAX_CATEGORIES 16
#define MAX_UNIQUE_NAMES_PER_CATEGORY 16
#define MAX_CONNECTIONS 64 
#define MAX_CONNECTION_STRING_LENGTH 64

const char* nullStr = "NULL";

typedef struct{
  char src[MAX_NAME_LENGTH];
  char dst[MAX_NAME_LENGTH];
  uint8_t src_port;
  uint8_t dst_port;
} ParsedConnection;

/*
Audio director class is responsible for proving an interface for the managment 
of the audio blocks, associated controllers and connection sets.
*/
class AudioDirector{
  public:
    AudioDirector();
    AudioDirector(const AudioDirector &) = delete;	//delete the copy constructor
    void activateConnectionGroup(uint16_t group_id);
    bool addAudioStreamObj(AudioStream* obj);
    bool connect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput);
    bool connect(char* from,uint8_t from_port,char* to,uint8_t to_port);
    bool connect(const char* connectionString);
    bool getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer);
    bool getConnectionString(uint16_t connectionIndex, char* connectionStringBuffer);
    bool disconnect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput);
    bool disconnect(char* from,uint8_t from_port,char* to,uint8_t to_port);
    bool disconnect(const char* connectionString);
    int16_t connectionCount(){return activeConnections;};
    AudioStream* getAudioStreamObjByName(const char* AudioStreamObjName);
  protected:
    void*  heapStart;  //used to estamate total heap allocation size
    void*  heapEnd;    //
    AudioStream* pAudioStreamObjPool[MAX_AUDIO_STREAM_OBJECTS]; //Generic Object Pool
    erisAudioInputI2S AudioStreamInputPort; //ADC Audio Input(s)
    AudioStream* pAudioStreamOutputPort; //DAC Audio Output(s)
    void unlinkAll();
    void linkGroup();
    void generateCategoryList();
    uint16_t activeConnectionGroup; 
    uint16_t activeConnections; 
    uint16_t objCount;
    uint16_t categoryCount;
    uint16_t shortNameQueryResultCount;
    char** categoryList[MAX_CATEGORIES];
    char* shortNameQueryResult[MAX_UNIQUE_NAMES_PER_CATEGORY];
    AudioConnection* pCord[MAX_CONNECTIONS];
    void ParseConnectString(const char* connectionString,ParsedConnection *p);
    
    // GUItool: begin automatically generated code
};

AudioDirector::AudioDirector(){
  objCount=0;
  activeConnections = 0;
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

  pAudioStreamOutputPort = new erisAudioOutputI2S();
  heapStart = pAudioStreamOutputPort;
  addAudioStreamObj(&AudioStreamInputPort);
  addAudioStreamObj(pAudioStreamOutputPort);
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeScope);
  addAudioStreamObj(new erisAudioAnalyzeNoteFrequency);
  addAudioStreamObj(new erisAudioEffectFreeverb);


  //generate audio component pool
  for (int i=0; i < 16; i++){
    addAudioStreamObj(new erisAudioEffectEnvelope);
    addAudioStreamObj(new erisAudioSynthWaveformModulated);
    addAudioStreamObj(new erisAudioSynthWaveform);
  }

  for (int i=0; i < 6; i++){
    addAudioStreamObj(new erisAudioMixer4);
    addAudioStreamObj(new erisAudioFilterStateVariable);
    addAudioStreamObj(new erisAudioFilterBiquad);
  }

  Serial.print(F("AudioDirector::AudioDirector() objects: "));
  Serial.println(objCount);

  generateCategoryList();
  Serial.print(F("AudioDirector::AudioDirector() object categories: "));
  Serial.println(categoryCount);
  for (uint16_t j = 0; j < categoryCount; j++)Serial.println(*categoryList[j]);

  AudioStream* test = getAudioStreamObjByName("mixer_2");
  Serial.println(F("AudioDirector::AudioDirector() getAudioStreamObjByName test: (should be mixer #2)"));
  Serial.print(test->shortName);
  Serial.print(F("_"));
  Serial.println(test->instance);

  Serial.println(F("AudioDirector::AudioDirector() building AudioConnection pool"));
  for(uint16_t i=0; i < MAX_CONNECTIONS;i++){
    pCord[i] = new AudioConnection(NULL, (unsigned char)0,NULL, (unsigned char)0);
  }
  heapEnd = pCord[MAX_CONNECTIONS-1] + (pCord[MAX_CONNECTIONS-1] - pCord[MAX_CONNECTIONS-2]);
  Serial.print(F("AudioDirector::AudioDirector() Estimated Memory Useage: "));
  long s = (uint32_t)heapStart;
  long e = (uint32_t)heapEnd;
  Serial.print(e-s);
  Serial.println(" Bytes");

};

bool AudioDirector::addAudioStreamObj(AudioStream* obj){
  uint8_t count = 0; 

  if (obj !=0 && objCount < MAX_AUDIO_STREAM_OBJECTS){
    pAudioStreamObjPool[objCount++] = obj;
    Serial.print(F("AudioDirector::addAudioStreamObj adding obj :"));
    Serial.print(obj->category);
    Serial.print(F("->"));
    Serial.print(obj->shortName);  
    if (objCount > 0){
      for(uint16_t i=0; i < objCount;i++){
        if(strcmp(obj->shortName,pAudioStreamObjPool[i]->shortName)==0) count += 1;
      }
    } else{
      heapStart = (void *)obj;  
    }
    obj->instance = count;
    Serial.print(F(" instance: "));
    Serial.print(obj->instance);
    Serial.print(F(" ptr: "));
    Serial.println((uint32_t)obj);
    heapEnd = (void *)obj;
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

AudioStream* AudioDirector::getAudioStreamObjByName(const char* AudioStreamObjName){
  //string name needs to be deccompsed to type and instance
  char *pch;
  char *ptr;
  pch = strstr (AudioStreamObjName,"_");
  uint8_t instance = (uint8_t)strtoul(pch+1,&ptr,10);//instance starts 1 char from '_' and is always base 10

  for (uint16_t i = 0; i < objCount; i++){
    //test the index first, then the string
    if (instance == pAudioStreamObjPool[i]->instance){
        if (0==strncmp(AudioStreamObjName,pAudioStreamObjPool[i]->shortName,strlen(pAudioStreamObjPool[i]->shortName))) return pAudioStreamObjPool[i];
    }
  }
  //not found return null
  Serial.print(F("M  AudioDirector::getAudioStreamObjByName Not Found: "));
  Serial.println(AudioStreamObjName);
  return 0;
}

bool AudioDirector::getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer){
  if (streamIndex >= objCount){
    strcpy(streamStringBuffer,"");
    return false;
  }
  sprintf(streamStringBuffer,"%s %s_%d",pAudioStreamObjPool[streamIndex]->category,pAudioStreamObjPool[streamIndex]->shortName,pAudioStreamObjPool[streamIndex]->instance);
  return true;
}

bool AudioDirector::getConnectionString(uint16_t connectionIndex, char* connectionStringBuffer){
  //STUB
  uint16_t i;
  uint16_t count = 0;
  if (connectionIndex >= activeConnections){
    strcpy(connectionStringBuffer,"");
    return false;
  }
  for(i=0; i < MAX_CONNECTIONS;i++){
    if (pCord[i]->isConnected == true){
      if(count == connectionIndex){
        //build connection string into the buffer
        sprintf(connectionStringBuffer,"connect(%s_%d,%d,%s_%d,%d);",
          pCord[i]->pSrc->shortName,
          pCord[i]->pSrc->instance,
          pCord[i]->src_index,
          pCord[i]->pDst->shortName,
          pCord[i]->pDst->instance,
          pCord[i]->dest_index);
        if (connectionIndex < activeConnections) return true;
      }
      count++;
    }
  }
  strcpy(connectionStringBuffer,"");
  return false;
}
bool AudioDirector::connect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput){
  uint16_t i;
  if (NULL==source||NULL==destination) return false;
  //check if already existing
  for(i=0; i < MAX_CONNECTIONS;i++){
    if(source==pCord[i]->pSrc && destination==pCord[i]->pDst && sourceOutput==pCord[i]->src_index && destinationInput==pCord[i]->dest_index){
      Serial.print(F("AudioDirector::connect() found existing connection; reconnecting at index  "));
      Serial.println(i);
      if(pCord[i]->reconnect()) activeConnections++;
      return true;
    }
  }
  
  //find a free connection from the pool
  for(i=0; i < MAX_CONNECTIONS;i++){
    //find any already existing but unused connections
    if (NULL!=pCord[i]){
      if (pCord[i]->isConnected == false){
        Serial.print(F("AudioDirector::connect() recycling AudioConnection at index "));
        Serial.println(i);
        Serial.print(source->shortName);Serial.print(":");
        Serial.print(sourceOutput);
        Serial.print(F(" -> "));
        Serial.print(destination->shortName);Serial.print(":");
        Serial.println(destinationInput);
        if(pCord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput)) activeConnections++;
        return true;
      }
    }
    if (NULL==pCord[i]){
      Serial.print(F("AudioDirector::connect() making a new AudioConnection at index "));
      Serial.println(i);
      Serial.print(source->shortName);Serial.print(F(":"));
      Serial.print(sourceOutput);
      Serial.print(F(" -> "));
      Serial.print(destination->shortName);Serial.print(F(":"));
      Serial.println(destinationInput);
      //need to rewire after creation as the reference based approach was bypassed 
      //in favor of pointers in order to facilitate the extention of the audio connection base class
      pCord[i] = new AudioConnection(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
      pCord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
      return true; 
    }
  }
  Serial.print(F("AudioDirector::connect() Error: No free audio connection available"));
  return false; //no empty connection slots  
}

bool AudioDirector::connect(char* from,uint8_t from_port,char* to,uint8_t to_port){
  return connect(getAudioStreamObjByName((char* )from),from_port,getAudioStreamObjByName((char* )to),to_port);
}

void AudioDirector::ParseConnectString(const char* connectionString,ParsedConnection *p){
//parse connectionString 
  //format: "SRCTYPE(CHAR)_INSTANCE(INT) SRCPORT(INT) DESTTYPE(CHAR)_INSTANCE(INT) DESTPORT(INT)"
  //example: "MIXER_2 0 CHORUS_1 1"
  //the example above connects mixer instance 2 output 0 with chorus instance 1 input 1
  char buffer[MAX_CONNECTION_STRING_LENGTH];
  char *token;

  if(strlen(connectionString)>MAX_CONNECTION_STRING_LENGTH-1) return;

  memset(buffer, '\0', sizeof(buffer));
  memset(p->src, '\0', sizeof(p->src));
  memset(p->dst, '\0', sizeof(p->dst));
  strcpy(buffer,connectionString);
   
  token = strtok(buffer, " ");
  if(strlen(token)>MAX_NAME_LENGTH-1) return;
  strcpy(p->src,token);
  
  token = strtok(NULL, " ");
  if(strlen(token)>MAX_NAME_LENGTH-1) return;
  p->src_port = atoi(token);
  
  token = strtok(NULL, " ");
  if(strlen(token)>MAX_NAME_LENGTH-1) return;
  strcpy(p->dst,token);
  token = strtok(NULL, " ");
  p->dst_port = atoi(token);
  
  Serial.print(F("AudioDirector::ParseConnectString "));
  Serial.println(connectionString);
  Serial.print(F("Source: "));Serial.print(p->src);Serial.print(F(" Port:"));Serial.println(p->src_port);
  Serial.print(F("Dest: "));Serial.print(p->dst);Serial.print(F(" Port:"));Serial.println(p->dst_port);

  return;
}

bool AudioDirector::connect(const char* connectionString){
  ParsedConnection p;
  ParseConnectString(connectionString,&p);
  connect(p.src,p.src_port,p.dst,p.dst_port);
  return false;
}

bool AudioDirector::disconnect(char* from,uint8_t from_port,char* to,uint8_t to_port){
  return disconnect(getAudioStreamObjByName((char* )from),from_port,getAudioStreamObjByName((char* )to),to_port);
}

bool AudioDirector::disconnect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput){
  if (NULL==source||NULL==destination) return false;
  //find the connection within the pool
  uint16_t i;
  for(i=0; i < MAX_CONNECTIONS;i++){
    if (pCord[i]->pSrc == source && pCord[i]->pDst == destination && pCord[i]->src_index == sourceOutput && pCord[i]->dest_index == destinationInput){
      Serial.print(F("M AudioDirector::disconnect() found AudioConnection at index "));
      Serial.println(i);
      //disconnect the audio connection
      if(pCord[i]->disconnect()) activeConnections--;
      Serial.println(F("M AudioDirector::disconnect() disconnect complete"));
      return true; 
    }
  }
  Serial.println(F("M AudioDirector::disconnect() Warning: AudioConnection not found"));
  Serial.println(source->shortName);
  Serial.println(source->instance);
  Serial.println(destination->shortName);
  Serial.println(destination->instance);
  return false; //no empty connection slots
}

bool AudioDirector::disconnect(const char* connectionString){
  return false;
};

void AudioDirector::activateConnectionGroup(uint16_t group_id){
//testing
  Serial.println(F("AudioDirector::activateConnectionGroup() connecting AudioStreamInputPort to pAudioStreamOutputPort"));
  AudioNoInterrupts();

  //connect(pAudioStreamInputPort,0,pAudioStreamOutputPort,0);

  //connect a waveform object to the output and fft
  //connect("waveform_1 0 waveformMod_1 0");
  

  //connect("waveformMod_1 0 filter_1 0");
  //connect("waveformMod_1 0 filter_3 0");
  connect("i2s-in_1 1 biquad_1 0");
  connect("biquad_1 0 fft1024_1 0"); //lp filter
  
  connect("i2s-in_1 1 biquad_2 0");
  connect("biquad_2 0 fft1024_2 0"); //lp filter

  connect("i2s-in_1 1 scope_1 0");
  connect("mixer_1 0 scope_1 1");
  //connect("i2s-in_1 1 i2s-out_1 1");

  //16 voice oscillator bank
  connect("mixer_2 0 mixer_6 0");
  connect("mixer_3 0 mixer_6 1");
  connect("mixer_4 0 mixer_6 2");

  connect("mixer_6 0 freeverb_1 0");
  connect("freeverb_1 0 mixer_1 0");
  
  connect("mixer_6 0 mixer_1 1");

  connect("mixer_5 0 filter_3 0");
  connect("filter_3 0 mixer_1 2");

  connect("waveform_1 0 mixer_2 0");
  connect("waveform_2 0 mixer_2 1");
  connect("waveform_3 0 mixer_2 2");
  connect("waveform_4 0 mixer_2 3");
  
  connect("waveform_5 0 mixer_3 0");
  connect("waveform_6 0 mixer_3 1");
  connect("waveform_7 0 mixer_3 2");
  connect("waveform_8 0 mixer_3 3");
  
  connect("waveform_9 0 mixer_4 0");
  connect("waveform_10 0 mixer_4 1");
  connect("waveform_11 0 mixer_4 2");
  connect("waveform_12 0 mixer_4 3");
  
  connect("waveform_13 0 mixer_5 0");
  connect("waveform_14 0 mixer_5 1");
  connect("waveform_15 0 mixer_5 2");
  connect("i2s-in_1 1 mixer_5 3");

  connect("mixer_1 0 i2s-out_1 0");
  AudioInterrupts();
  
  //to use the objects they must be downcast
  //erisAudioSynthWaveform* mod = (erisAudioSynthWaveform*) (getAudioStreamObjByName("waveform_1"));
  //mod->begin(0.40, 200, WAVEFORM_SINE);

/*
  erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*) (getAudioStreamObjByName("waveformMod_1"));
  wav->frequencyModulation(2);
  wav->begin(0.6, 1200, WAVEFORM_SAWTOOTH);
*/
}

#endif
