/**
 * @file AudioDirector.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __AudioDirector
#define  __AudioDirector

#include "Eris.h"
#include <string.h>
#include "ErisAudio.h"
#include "SvcSerialCommandInterface.h"
#include "SvcErisAudioParameterController.h"

class SvcSerialCommandInterface;
class SvcErisAudioParameterController;

typedef struct{
  char src[MAX_NAME_LENGTH];
  char dst[MAX_NAME_LENGTH];
  uint8_t src_port;
  uint8_t dst_port;
} ParsedConnection;

/*
Audio director class is responsible for proving an interface for the management 
of the audio blocks, associated controllers and connection sets.
*/
class AudioDirector{
  public:
    AudioDirector();
    AudioDirector(const AudioDirector &) = delete;	//delete the copy constructor
    bool initAudioHardware();
    bool addAudioStreamObj(AudioStream* obj);
    bool connect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput);
    bool connect(const char* from,uint8_t from_port,const char* to,uint8_t to_port);
    bool connect(const char* connectionString);
    bool getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer);
    bool getConnectionString(uint16_t connectionIndex, char* connectionStringBuffer);
    bool disconnectAll();
    bool disconnect(AudioStream* destination,int destinationInput);
    bool disconnect(const char* to,uint8_t to_port);
    bool disconnect(const char* connectionString);
    int16_t connectionCount(){return active_connections;};
    AudioStream* getAudioStreamObjByName(const char* AudioStreamObjName);
    void setSCI(SvcSerialCommandInterface *serialCommandInterface){sci = serialCommandInterface;}
    void setAPC(SvcErisAudioParameterController *audioParameterController){parameter_controller = audioParameterController;}
    void printStats();
    uint16_t getFunctionCount(){generateFunctionList();return category_count;}
    
    char* getFunctionListItem(uint16_t i){
      generateFunctionList();
      char* p;
      p = *functionsList[i];
      return p;
    }

    uint16_t getTypeCountByFunction(const char * function){queryTypesByFunction(function);return query_result_count;}
    char* getTypeListItem(const char* function,uint16_t i);
    uint16_t getTypeInstanceCount(const char* type);
   protected:
    SvcSerialCommandInterface* sci;
    SvcErisAudioParameterController* parameter_controller;
    void*  p_heap_start;  //used to estimate total heap allocation size
    void*  p_heap_end;    //
    AudioStream* p_audiostream_obj_pool[MAX_AUDIO_STREAM_OBJECTS]; //Generic Object Pool
    erisAudioInputI2S audiostream_input_port; //ADC Audio Input(s)
    erisAudioOutputI2S audiostream_output_port; //DAC Audio Output(s)
    uint8_t printstats_select;
    uint16_t printstats_block;
    uint16_t active_connections; 
    uint16_t obj_count;
    uint16_t category_count; 
      
      
    uint16_t query_result_count;
    char** functionsList[MAX_AUDIO_FUNCTION_CATEGORIES];
    char** query_result[MAX_AUDIO_TYPES_BY_FUNCTION_QUERY_RESULT];
    AudioConnection* p_cord[MAX_CONNECTIONS];
    void unlinkAll();
    void linkGroup();
    void generateFunctionList();
    uint16_t queryTypesByFunction(const char * function);
    void ParseConnectString(const char* connectionString,ParsedConnection *p);
};



#endif
