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


FLASHMEM AudioDirector::AudioDirector(){
  sci = NULL;
  obj_count=0;
  active_connections = 0;
  category_count=0;
  query_result_count=0;
  printstats_select = 0;
  AudioMemory(MAX_AUDIO_MEMORY_BLOCKS);
  //init pointer arrays
  for (uint16_t i=0; i < MAX_AUDIO_TYPES_BY_FUNCTION_QUERY_RESULT; i++){
    query_result[i]=0;
  }
  for (uint16_t j = 0; j < MAX_AUDIO_FUNCTION_CATEGORIES; j++){
    functionsList[j] =0;
  }
  p_audiostream_input_port = new erisAudioInputI2S();
  p_heap_start = p_audiostream_input_port;

  addAudioStreamObj(p_audiostream_input_port);
  addAudioStreamObj(&p_audiostream_output_port);

  
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
  Serial.println(obj_count);

  generateFunctionList();
  Serial.print(F("M AudioDirector::AudioDirector() object categories: "));
  Serial.println(category_count);
  for (uint16_t j = 0; j < category_count; j++)Serial.printf("M %s\n",*functionsList[j]);

  AudioStream* test = getAudioStreamObjByName("mixer:2");
  Serial.println(F("M AudioDirector::AudioDirector() getAudioStreamObjByName test: (should be mixer #2)"));
  Serial.print(test->short_name);
  Serial.print(F(":"));
  Serial.println(test->instance);

  Serial.println(F("M AudioDirector::AudioDirector() building AudioConnection pool"));
  for(uint16_t i=0; i < MAX_CONNECTIONS;i++){
    p_cord[i] = new AudioConnection(NULL, (unsigned char)0,NULL, (unsigned char)0);
  }
  p_heap_end = p_cord[MAX_CONNECTIONS-1] + (p_cord[MAX_CONNECTIONS-1] - p_cord[MAX_CONNECTIONS-2]);
  Serial.print(F("M AudioDirector::AudioDirector() Estimated Memory Useage: "));
  long s = (uint32_t)p_heap_start;
  long e = (uint32_t)p_heap_end;
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

  if (obj !=0 && obj_count < MAX_AUDIO_STREAM_OBJECTS){
    p_audiostream_obj_pool[obj_count++] = obj;
    Serial.print(F("M AudioDirector::addAudioStreamObj adding obj :"));
    Serial.print(obj->category);
    Serial.print(F("->"));
    Serial.print(obj->short_name);  
    if (obj_count > 0){
      for(uint16_t i=0; i < obj_count;i++){
        if(strcmp(obj->short_name,p_audiostream_obj_pool[i]->short_name)==0) count += 1;
      }
    } else{
      p_heap_start = (void *)obj;  
    }
    obj->instance = count;
    Serial.print(F(" instance: "));
    Serial.print(obj->instance);
    Serial.print(F(" ptr: "));
    Serial.println((uint32_t)obj);
    p_heap_end = (void *)obj;
    //Serial.flush();
    return true;
  }
  return false;
};

void FLASHMEM AudioDirector::printStats(){

  if (sci==NULL) return;

  if (printstats_select == 0){
    if(sci->requestStartLZ4Message()){
      printstats_select += 1;
      sci->print(F("STATS {\"AudioDirector\":{"));  
      sci->print(F("\"AudioStreams\":{"));
      
      for(uint16_t i=0; i < obj_count;i++){
        sci->print(F("\""));
        sci->print(p_audiostream_obj_pool[i]->short_name);sci->print(F(":"));
        sci->print(p_audiostream_obj_pool[i]->instance);
        sci->print(F("\":{\"cpu\":"));
        sci->print(p_audiostream_obj_pool[i]->processorUsage());
        sci->print(F(",\"cpu_max\":"));
        sci->print(p_audiostream_obj_pool[i]->processorUsageMax());
        sci->print(F(",\"category\":\""));
        sci->print(p_audiostream_obj_pool[i]->category);sci->print(F("\""));
        sci->print(F(",\"inputs\":"));
        sci->print(p_audiostream_obj_pool[i]->unum_inputs);
        sci->print(F(",\"outputs\":"));
        sci->print(p_audiostream_obj_pool[i]->unum_outputs);
        sci->print("}"); //close obj
        if ( i < (obj_count -1)) sci->print(",");
      }
      sci->print("}}}");
      sci->sendLZ4Message();
    }
  }else if (printstats_select == 1){ 
    if(sci->requestStartLZ4Message()){
      printstats_select += 1;
      sci->print(F("STATS {\"AudioDirector\":{"));  
      sci->print(F("\"AudioConnectionPool\":{"));
      sci->print(F("\"active_connections\":"));
      sci->print(active_connections);
      for(uint16_t i=0; i < MAX_CONNECTIONS;i++){ //for each...
        sci->print(F(",\""));
        sci->print(i); //connection index used as a container
        sci->print(F("\":{"));
        sci->print(F("\"inUse\":"));
        sci->print(p_cord[i]->isConnected);
        if (p_cord[i]->isConnected == true){ //assigned connections
            sci->print(F(",\"srcType\":\""));
            sci->print(p_cord[i]->pSrc->short_name);
            sci->print(F("\",\"srcInstance\":"));
            sci->print(p_cord[i]->pSrc->instance);
            sci->print(F(",\"srcPort\":"));
            sci->print(p_cord[i]->src_index);

            sci->print(F(",\"destType\":\""));
            sci->print(p_cord[i]->pDst->short_name);
            sci->print(F("\",\"destInstance\":"));
            sci->print(p_cord[i]->pDst->instance);
            sci->print(F(",\"destPort\":"));
            sci->print(p_cord[i]->dest_index);
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
      printstats_select = 0;
      sci->print(F("STATS {"));  
      ExtADCPrintStatus(sci);
      sci->println(F("}"));
      sci->sendLZ4Message();
    }
  }
  //sci->startLZ4Message();
  //sci->print(F("}")); // close the coonection pool container
  //throw in some audio director stats
  //sci->print(F(",\"AudioStreamObjCount\":")); sci->print(obj_count);
  //sci->print(F(",\"memory\":"));
  //sci->print(e-s);
  //sci->println(F("}}"));
  //sci->endLZ4Message();
  
}


/**
 * @brief get the list of function types in the obj pool
 * 
 */
void AudioDirector::generateFunctionList(){
  //init the count and the container
  category_count=0;
  for (uint16_t j = 0; j < MAX_AUDIO_FUNCTION_CATEGORIES; j++){
    functionsList[j] = 0;
  }

  for (uint16_t i = 0; i < obj_count; i++){           //for each object
    bool found;
    found = false;
    for (uint16_t j = 0; j < MAX_AUDIO_FUNCTION_CATEGORIES; j++){   //check if cat name is already in the list
      if (functionsList[j] != 0){ //dont test unitialized string pointers
        if (strcmp(*functionsList[j],p_audiostream_obj_pool[i]->category)==0) found = true;
      } 
    }
    if (found==false) functionsList[category_count++] = (char**)&p_audiostream_obj_pool[i]->category; //add to the list if not existing
    if (category_count==MAX_AUDIO_FUNCTION_CATEGORIES) return; //limit the result list to the size of the category buffer 
  }
}

/**
 * @brief query the obj pool for the list of audio blocks belonging to the given function type
 * 
 * @param function 
 * @return uint16_t 
 */
uint16_t AudioDirector::queryTypesByFunction(const char * function){
  query_result_count=0;
  for (uint16_t j = 0; j < MAX_AUDIO_TYPES_BY_FUNCTION_QUERY_RESULT; j++){
    query_result[j] = 0;
  }

  for (uint16_t i = 0; i < obj_count; i++){           //for each object
    bool found;
    found = false;
    for (uint16_t j = 0; j < MAX_AUDIO_TYPES_BY_FUNCTION_QUERY_RESULT; j++){   //check if cat name is already in the list
      if (query_result[j] != 0){ //dont test unitialized string pointers
        if ((strcmp(*query_result[j],p_audiostream_obj_pool[i]->short_name)==0)) found = true;
      } 
    }
    if (found==false && (strcmp(function,p_audiostream_obj_pool[i]->category)==0)) query_result[query_result_count++] = (char**)&p_audiostream_obj_pool[i]->short_name; //add to the list if not existing
    if (query_result_count==MAX_AUDIO_TYPES_BY_FUNCTION_QUERY_RESULT) return query_result_count; //limit the result list to the size of the category buffer 
  }

  return query_result_count;
}

uint16_t AudioDirector::getTypeInstanceCount(const char* type){
  query_result_count=0;
  for (uint16_t i = 0; i < obj_count; i++){           //for each object
    if ((strcmp(type,p_audiostream_obj_pool[i]->short_name)==0)) query_result_count++;
  }
  return query_result_count;
}

/**
 * @brief returns the requested index of the type list\n 
 * if the requested index is out of bounds the string "NULL" is returned
 * 
 * @param function 
 * @param i 
 * @return char* 
 */
char* AudioDirector::getTypeListItem(const char * function,uint16_t i){
  uint16_t count = queryTypesByFunction(function);
  if (i > count) return (char*)nullStr; //out of bounds index return "NULL"
  char* p;
  p = *query_result[i];
  return p;
}

AudioStream* AudioDirector::getAudioStreamObjByName(const char* AudioStreamObjName){
  //string name needs to be deccompsed to type and instance
  char *pch;
  char *ptr;
  pch = strstr (AudioStreamObjName,":");
  uint8_t instance = (uint8_t)strtoul(pch+1,&ptr,10);//instance starts 1 char from ':' and is always base 10

  for (uint16_t i = 0; i < obj_count; i++){
    //test the index first, then the string
    if (instance == p_audiostream_obj_pool[i]->instance){
        if (0==strncmp(AudioStreamObjName,p_audiostream_obj_pool[i]->short_name,strlen(p_audiostream_obj_pool[i]->short_name))) return p_audiostream_obj_pool[i];
    }
  }
  //not found return null
  Serial.print(F("M AudioDirector::getAudioStreamObjByName Not Found: "));
  Serial.println(AudioStreamObjName);
  return 0;
}

bool AudioDirector::getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer){
  if (streamIndex >= obj_count){
    strcpy(streamStringBuffer,"");
    return false;
  }
  sprintf(streamStringBuffer,"%s %s:%d",p_audiostream_obj_pool[streamIndex]->category,p_audiostream_obj_pool[streamIndex]->short_name,p_audiostream_obj_pool[streamIndex]->instance);
  return true;
}

bool AudioDirector::getConnectionString(uint16_t connectionIndex, char* connectionStringBuffer){
  //STUB
  uint16_t i;
  uint16_t count = 0;
  if (connectionIndex >= active_connections){
    strcpy(connectionStringBuffer,"");
    return false;
  }
  for(i=0; i < MAX_CONNECTIONS;i++){
    if (p_cord[i]->isConnected == true){
      if(count == connectionIndex){
        //build connection string into the buffer
        sprintf(connectionStringBuffer,"connect(%s:%d,%d,%s:%d,%d);",
          p_cord[i]->pSrc->short_name,
          p_cord[i]->pSrc->instance,
          p_cord[i]->src_index,
          p_cord[i]->pDst->short_name,
          p_cord[i]->pDst->instance,
          p_cord[i]->dest_index);
        if (connectionIndex < active_connections) return true;
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
    if(source==p_cord[i]->pSrc && destination==p_cord[i]->pDst && sourceOutput==p_cord[i]->src_index && destinationInput==p_cord[i]->dest_index){
      Serial.printf(F("M AudioDirector::connect() found existing connection; reconnecting at index %hu\n"),i);
      //Serial.println(i);
      //Serial.flush();
      if(p_cord[i]->reconnect()) active_connections++;
      return true;
    }
  }
  
  //find a free connection from the pool
  for(i=0; i < MAX_CONNECTIONS;i++){
    //find any already existing but unused connections
    if (NULL!=p_cord[i]){
      if (p_cord[i]->isConnected == false){
        Serial.printf(F("M AudioDirector::connect() connection index: %hu\n"),i);
        //Serial.printf("%s:%d:%d -> %s:%d:%d  ",source->shortName,source->instance,sourceOutput,destination->shortName,destination->instance,destinationInput);
        //Serial.flush();
        if(p_cord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput)) active_connections++;
        return true;
      }
    }
    if (NULL==p_cord[i]){
      Serial.printf(F("M AudioDirector::connect() making a new AudioConnection at index %hu\n"),i);
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
      p_cord[i] = new AudioConnection(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
      p_cord[i]->rewire(source, (unsigned char)sourceOutput,destination, (unsigned char)destinationInput);
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
  //format: "SRCTYPE(CHAR):INSTANCE(INT) SRCPORT(INT) DESTTYPE(CHAR):INSTANCE(INT) DESTPORT(INT)"
  //example: "MIXER:2 0 CHORUS:2 1"
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
  
  
  Serial.print(F("M AudioDirector::ParseConnectString "));
  Serial.println(connectionString);
  Serial.print(F("M Source: "));Serial.print(p->src);Serial.print(F(" Port:"));Serial.print(p->src_port);
  Serial.print(F("\tDest: "));Serial.print(p->dst);Serial.print(F(" Port:"));Serial.println(p->dst_port);
  //Serial.flush();
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
    if (p_cord[i]->pDst == destination && p_cord[i]->dest_index == destinationInput){
      Serial.print(F("M AudioDirector::disconnect() found AudioConnection at index "));
      Serial.println(i);
      //disconnect the audio connection
      if(p_cord[i]->disconnect()) active_connections--;
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
    if(p_cord[i]->disconnect()) active_connections--;
  }
  return false;
};

