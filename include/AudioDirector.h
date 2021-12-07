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

#include "globaldefs.h"
#include <string.h>
#include "erisAudio.h"
#include "svcSerialCommandInterface.h"

class SvcSerialCommandInterface;

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
    bool addAudioStreamObj(AudioStream* obj);
    bool connect(AudioStream* source, int sourceOutput, AudioStream* destination,int destinationInput);
    bool connect(char* from,uint8_t from_port,char* to,uint8_t to_port);
    bool connect(const char* connectionString);
    bool getAudioStreamString(uint16_t streamIndex, char* streamStringBuffer);
    bool getConnectionString(uint16_t connectionIndex, char* connectionStringBuffer);
    bool disconnectAll();
    bool disconnect(AudioStream* destination,int destinationInput);
    bool disconnect(char* to,uint8_t to_port);
    bool disconnect(const char* connectionString);
    int16_t connectionCount(){return activeConnections;};
    AudioStream* getAudioStreamObjByName(const char* AudioStreamObjName);
    void setSCI(SvcSerialCommandInterface *serialCommandInterface){sci = serialCommandInterface;}
    void printStats();
  protected:
    SvcSerialCommandInterface* sci;
    void*  heapStart;  //used to estamate total heap allocation size
    void*  heapEnd;    //
    AudioStream* pAudioStreamObjPool[MAX_AUDIO_STREAM_OBJECTS]; //Generic Object Pool
    AudioStream* pAudioStreamInputPort; //ADC Audio Input(s)
    erisAudioOutputI2S AudioStreamOutputPort; //DAC Audio Output(s)
    void unlinkAll();
    void linkGroup();
    void generateCategoryList();
    uint16_t activeConnections; 
    uint16_t objCount;
    uint16_t categoryCount;
    uint16_t shortNameQueryResultCount;
    char** categoryList[MAX_CATEGORIES];
    char* shortNameQueryResult[MAX_UNIQUE_NAMES_PER_CATEGORY];
    AudioConnection* pCord[MAX_CONNECTIONS];
    void ParseConnectString(const char* connectionString,ParsedConnection *p);
};



#endif
