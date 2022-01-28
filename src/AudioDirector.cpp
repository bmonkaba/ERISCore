/**
 * @file AudioDirector.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AudioDirector.h"
#include "PCM1863.h"

#ifdef USE_EXTMEM
#undef AudioMemory
#define AudioMemory(num) ({ \
	static EXTMEM audio_block_t data[num] __attribute__ ((aligned(32))); \
	AudioStream::initialize_memory(data, num); \
})
#endif


const char* nullStr = "NULL";

FLASHMEM AudioDirector::AudioDirector(){
  sci = NULL;
  objCount=0;
  activeConnections = 0;
  categoryCount=0;
  shortNameQueryResultCount=0;
  printStatsSelect = 0;
  AudioMemory(MAX_AUDIO_MEMORY_BLOCKS);
  //init pointer arrays
  for (uint16_t i=0; i < MAX_UNIQUE_NAMES_PER_CATEGORY; i++){
    shortNameQueryResult[i]=0;
  }
  for (uint16_t j = 0; j < MAX_CATEGORIES; j++){
    categoryList[j] = (char**)&nullStr;
  }
  pAudioStreamInputPort = new erisAudioInputI2S();
  heapStart = pAudioStreamInputPort;

  addAudioStreamObj(pAudioStreamInputPort);
  addAudioStreamObj(&AudioStreamOutputPort);

  
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeFFT1024);
  addAudioStreamObj(new erisAudioAnalyzeScope);
  //addAudioStreamObj(new erisAudioAnalyzeNoteFrequency);
  addAudioStreamObj(new erisAudioEffectFreeverb);
  addAudioStreamObj(new erisAudioSynthNoisePink);

  addAudioStreamObj(new erisAudioSynthToneSweep);
  addAudioStreamObj(new erisAudioSynthWaveformSineHires);
  addAudioStreamObj(new erisAudioSynthWaveformSineModulated);
  
  //generate audio component pool
  for (int i=0; i < 17; i++){
    //addAudioStreamObj(new erisAudioEffectEnvelope);
    //addAudioStreamObj(new erisAudioSynthWaveformModulated);
    addAudioStreamObj(new erisAudioSynthWaveform);
  }

  for (int i=0; i < 6; i++){
    addAudioStreamObj(new erisAudioMixer4);
    //addAudioStreamObj(new erisAudioFilterStateVariable);
    addAudioStreamObj(new erisAudioFilterBiquad);
    addAudioStreamObj(new erisAudioAmplifier);
  }

  addAudioStreamObj(new erisAudioMixer8);
  addAudioStreamObj(new erisAudioMixer8);
  addAudioStreamObj(new erisAudioMixer8);

  addAudioStreamObj(new erisAudioEffectDelay);
  
  Serial.print(F("M AudioDirector::AudioDirector() objects: "));
  Serial.println(objCount);

  generateCategoryList();
  Serial.print(F("M AudioDirector::AudioDirector() object categories: "));
  Serial.println(categoryCount);
  for (uint16_t j = 0; j < categoryCount; j++)Serial.println(*categoryList[j]);

  AudioStream* test = getAudioStreamObjByName("mixer_2");
  Serial.println(F("M AudioDirector::AudioDirector() getAudioStreamObjByName test: (should be mixer #2)"));
  Serial.print(test->short_name);
  Serial.print(F("_"));
  Serial.println(test->instance);

  Serial.println(F("M AudioDirector::AudioDirector() building AudioConnection pool"));
  for(uint16_t i=0; i < MAX_CONNECTIONS;i++){
    pCord[i] = new AudioConnection(NULL, (unsigned char)0,NULL, (unsigned char)0);
  }
  heapEnd = pCord[MAX_CONNECTIONS-1] + (pCord[MAX_CONNECTIONS-1] - pCord[MAX_CONNECTIONS-2]);
  Serial.print(F("M AudioDirector::AudioDirector() Estimated Memory Useage: "));
  long s = (uint32_t)heapStart;
  long e = (uint32_t)heapEnd;
  Serial.print(e-s);
  Serial.println(F(" Bytes"));
};

bool AudioDirector::initAudioHardware(){
  I2CReset();
  ExtADCConfig();
  //ExtADCPrintStatus(sci);
  //todo check values
  return true;
}


bool AudioDirector::addAudioStreamObj(AudioStream* obj){
  uint8_t count = 0; 

  if (obj !=0 && objCount < MAX_AUDIO_STREAM_OBJECTS){
    pAudioStreamObjPool[objCount++] = obj;
    Serial.print(F("M AudioDirector::addAudioStreamObj adding obj :"));
    Serial.print(obj->category);
    Serial.print(F("->"));
    Serial.print(obj->short_name);  
    if (objCount > 0){
      for(uint16_t i=0; i < objCount;i++){
        if(strcmp(obj->short_name,pAudioStreamObjPool[i]->short_name)==0) count += 1;
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
    //Serial.flush();
    return true;
  }
  return false;
};

void FLASHMEM AudioDirector::printStats(){

  if (sci==NULL) return;

  if (printStatsSelect == 0){
    if(sci->requestStartLZ4Message()){
      printStatsSelect += 1;
      sci->print(F("STATS {\"AudioDirector\":{"));  
      sci->print(F("\"AudioStreams\":{"));
      
      for(uint16_t i=0; i < objCount;i++){
        sci->print(F("\""));
        sci->print(pAudioStreamObjPool[i]->short_name);sci->print(F("_"));
        sci->print(pAudioStreamObjPool[i]->instance);
        sci->print(F("\":{\"cpu\":"));
        sci->print(pAudioStreamObjPool[i]->processorUsage());
        sci->print(F(",\"cpu_max\":"));
        sci->print(pAudioStreamObjPool[i]->processorUsageMax());
        sci->print(F(",\"category\":\""));
        sci->print(pAudioStreamObjPool[i]->category);sci->print(F("\""));
        sci->print(F(",\"inputs\":"));
        sci->print(pAudioStreamObjPool[i]->unum_inputs);
        sci->print(F(",\"outputs\":"));
        sci->print(pAudioStreamObjPool[i]->unum_outputs);
        sci->print("}"); //close obj
        if ( i < (objCount -1)) sci->print(",");
      }
      sci->print("}}}");
      sci->sendLZ4Message();
    }
  }else if (printStatsSelect == 1){ 
    if(sci->requestStartLZ4Message()){
      printStatsSelect += 1;
      sci->print(F("STATS {\"AudioDirector\":{"));  
      sci->print(F("\"AudioConnectionPool\":{"));
      sci->print(F("\"activeConnections\":"));
      sci->print(activeConnections);
      for(uint16_t i=0; i < MAX_CONNECTIONS;i++){ //for each...
        sci->print(F(",\""));
        sci->print(i); //connection index used as a container
        sci->print(F("\":{"));
        sci->print(F("\"inUse\":"));
        sci->print(pCord[i]->isConnected);
        if (pCord[i]->isConnected == true){ //assigned connections
            sci->print(F(",\"srcType\":\""));
            sci->print(pCord[i]->pSrc->short_name);
            sci->print(F("\",\"srcInstance\":"));
            sci->print(pCord[i]->pSrc->instance);
            sci->print(F(",\"srcPort\":"));
            sci->print(pCord[i]->src_index);

            sci->print(F(",\"destType\":\""));
            sci->print(pCord[i]->pDst->short_name);
            sci->print(F("\",\"destInstance\":"));
            sci->print(pCord[i]->pDst->instance);
            sci->print(F(",\"destPort\":"));
            sci->print(pCord[i]->dest_index);
        }else{
            //unassigned connections
        }
        sci->print(F("}")); //close the connection container
      }
      sci->println(F("}}}")); //close the connection container
      sci->sendLZ4Message();
    }
  }else{
    if(sci->requestStartLZ4Message()){
      printStatsSelect = 0;
      sci->print(F("STATS {"));  
      ExtADCPrintStatus(sci);
      sci->println(F("}"));
      sci->sendLZ4Message();
    }
  }
  //sci->startLZ4Message();
  //sci->print(F("}")); // close the coonection pool container
  //throw in some audio director stats
  //sci->print(F(",\"AudioStreamObjCount\":")); sci->print(objCount);
  //sci->print(F(",\"memory\":"));
  //sci->print(e-s);
  //sci->println(F("}}"));
  //sci->endLZ4Message();
  
}

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
        if (0==strncmp(AudioStreamObjName,pAudioStreamObjPool[i]->short_name,strlen(pAudioStreamObjPool[i]->short_name))) return pAudioStreamObjPool[i];
    }
  }
  //not found return null
  Serial.print(F("M AudioDirector::getAudioStreamObjByName Not Found: "));
  Serial.println(AudioStreamObjName);
  return 0;
}

bool AudioDirector::getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer){
  if (streamIndex >= objCount){
    strcpy(streamStringBuffer,"");
    return false;
  }
  sprintf(streamStringBuffer,"%s %s_%d",pAudioStreamObjPool[streamIndex]->category,pAudioStreamObjPool[streamIndex]->short_name,pAudioStreamObjPool[streamIndex]->instance);
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
          pCord[i]->pSrc->short_name,
          pCord[i]->pSrc->instance,
          pCord[i]->src_index,
          pCord[i]->pDst->short_name,
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
      //Serial.print(F("M AudioDirector::connect() found existing connection; reconnecting at index  "));
      //Serial.println(i);
      //Serial.flush();
      if(pCord[i]->reconnect()) activeConnections++;
      return true;
    }
  }
  
  //find a free connection from the pool
  for(i=0; i < MAX_CONNECTIONS;i++){
    //find any already existing but unused connections
    if (NULL!=pCord[i]){
      if (pCord[i]->isConnected == false){
        //Serial.printf(F("M \tAudioDirector::connect() connection index: %d\t"),i);
        //Serial.printf("%s_%d:%d -> %s_%d:%d  ",source->shortName,source->instance,sourceOutput,destination->shortName,destination->instance,destinationInput);
        //Serial.flush();
        if(pCord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput)) activeConnections++;
        return true;
      }
    }
    if (NULL==pCord[i]){
      Serial.print(F("M AudioDirector::connect() making a new AudioConnection at index "));
      Serial.println(i);
      /*
      Serial.print(source->shortName);Serial.print(F(":"));
      Serial.print(sourceOutput);
      Serial.print(F(" -> "));
      Serial.print(destination->shortName);Serial.print(F(":"));
      Serial.println(destinationInput);
      */
      //Serial.flush();
      //need to rewire after creation as the reference based approach was bypassed 
      //in favor of pointers in order to facilitate the extention of the audio connection base class
      pCord[i] = new AudioConnection(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
      pCord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
      return true; 
    }
  }
  Serial.print(F("M AudioDirector::connect() Error: No free audio connection available"));
  return false; //no empty connection slots  
}

bool AudioDirector::connect(const char* from,uint8_t from_port,const char* to,uint8_t to_port){
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
  
  /*
  Serial.print(F("AudioDirector::ParseConnectString "));
  //Serial.println(connectionString);
  Serial.print(F("Source: "));Serial.print(p->src);Serial.print(F(" Port:"));Serial.print(p->src_port);
  Serial.print(F("\tDest: "));Serial.print(p->dst);Serial.print(F(" Port:"));Serial.println(p->dst_port);
  //Serial.flush();
  */
 
  return;
}

bool AudioDirector::connect(const char* connectionString){
  ParsedConnection p;
  ParseConnectString(connectionString,&p);
  connect(p.src,p.src_port,p.dst,p.dst_port);
  return false;
}

bool AudioDirector::disconnect(const char* to,uint8_t to_port){
  return disconnect(getAudioStreamObjByName((char* )to),to_port);
}

bool AudioDirector::disconnect(AudioStream* destination,int destinationInput){
  if (NULL==destination){
    Serial.println(F("M AudioDirector::disconnect: ERROR destination is NULL"));
    return false;
  }
  //find the connection within the pool
  uint16_t i;
  for(i=0; i < MAX_CONNECTIONS;i++){
    if (pCord[i]->pDst == destination && pCord[i]->dest_index == destinationInput){
      Serial.print(F("M AudioDirector::disconnect() found AudioConnection at index "));
      Serial.println(i);
      //disconnect the audio connection
      if(pCord[i]->disconnect()) activeConnections--;
      Serial.println(F("M AudioDirector::disconnect() disconnect complete"));
      //Serial.flush();
      return true; 
    }
  }
  Serial.println(F("M AudioDirector::disconnect() Warning: AudioConnection not found"));
  Serial.println(destination->short_name);
  Serial.println(destination->instance);
  //Serial.flush();
  return false; 
}

bool AudioDirector::disconnect(const char* connectionString){
  return false;
};

bool AudioDirector::disconnectAll(){
  for(uint16_t i=0; i < MAX_CONNECTIONS; i++){
    if(pCord[i]->disconnect()) activeConnections--;
  }
  return false;
};

