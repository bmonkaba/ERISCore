#ifndef _GlobalDefines_
#define _GlobalDefines_

//AppManager
#define ENABLE_ASYNC_SCREEN_UPDATES
#define DISPLAY_UPDATE_PERIOD  120
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 50 

//AppBaseClass
#define MAX_NAME_LENGTH 36
#define MAX_TEXT_LENGTH 36

//AppSCI
#define SERIAL_POLLING_RATE_MAX 15
#define SERIAL_RX_BUFFER_SIZE 1024
#define SERIAL_PARAM_BUFFER_SIZE 128
#define SERIAL_OUTPUT_BUFFER_SIZE 4096
#define SERIAL_FILESTREAM_PAYLOAD_SIZE 256

#define SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
#define SERIAL_AUTO_TRANSMIT_DATA_PERIOD 421

//Audio Director
#define MAX_AUDIO_STREAM_OBJECTS 52
#define MAX_AUDIO_MEMORY_BLOCKS 600
#define MAX_CATEGORIES 16
#define MAX_UNIQUE_NAMES_PER_CATEGORY 24
#define MAX_CONNECTIONS 64 
#define MAX_CONNECTION_STRING_LENGTH 96
  
//COLORS
#define ERIS_GREEN 0x5BFF42
#endif