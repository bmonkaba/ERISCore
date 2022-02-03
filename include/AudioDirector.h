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

#include "ErisGlobals.h"
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
Audio director class is responsible for proving an interface for the managment 
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
  protected:
    SvcSerialCommandInterface* sci;
    SvcErisAudioParameterController* parameter_controller;
    void*  p_heap_start;  //used to estamate total heap allocation size
    void*  p_heap_end;    //
    AudioStream* p_audiostream_obj_pool[MAX_AUDIO_STREAM_OBJECTS]; //Generic Object Pool
    AudioStream* p_audiostream_input_port; //ADC Audio Input(s)
    erisAudioOutputI2S p_audiostream_output_port; //DAC Audio Output(s)
    void unlinkAll();
    void linkGroup();
    void generateCategoryList();
    uint8_t printstats_select; 
    uint16_t active_connections; 
    uint16_t obj_count;
    uint16_t category_count;
    uint16_t short_name_query_result_count;
    char** categoryList[MAX_CATEGORIES];
    char* short_name_query_result[MAX_UNIQUE_NAMES_PER_CATEGORY];
    AudioConnection* p_cord[MAX_CONNECTIONS];
    void ParseConnectString(const char* connectionString,ParsedConnection *p);
};



#endif
