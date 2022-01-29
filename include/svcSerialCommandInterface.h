/**
 * @file svcSerialCommandInterface.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _AppSCI_
#define _AppSCI_

#include "ErisGlobals.h"
#include <string.h>
#include "AppManager.h"
#include "AppBaseClass.h"
#include <SdFat.h>
#include <Print.h>
//#include <SdFat.h>

/**
 * @brief  Serial communication service and interface
 *  Serial commands (rx): \n
 *
 *   LS [PATH] 
 *   GET [PATH]  get file is responded with FS messages \n
 *   ACON        request current audio block connections \n
 *   CONNECT     make an audio block connection \n
 *   DISCONNECT  break an audio block connection \n
 *   APC         audio block parameter control message \n
 *   AA          broadcast message to active app \n
 *   STATS \n
 *   CQT_CFG     request a dump of the cqt bin configs \n
 *   GET_DD      request a dump of the data dictionary \n    
 *   UPDATE_DD   key val   set a key value \n
 *\n
 * Serial messages (tx):\n
 *\n
 *   CQT_H  (CQT BINS - High Range) \n
 *   CQT_L  (CQT  "     Low Range") \n
 *   CQT_EOF (End of analysis frame) \n
 *   DIR  (directory contents - aka the result of the ls command) \n
 *   DIR_EOF \n
 *   S   (FFT window stream) \n
 *   FS   (file stream) \n
 *   FS_END (file stream end, indicate to client eof) \n
 *   GET_ERR (file request error response ) \n
 *   ACON START (begin audio connection list) \n
 *   ACON END (end audio connection list) \n
 *   RAM1   JSON container \n
 *   RAM2   JSON container \n
 */
class SvcSerialCommandInterface:public AppBaseClass, public Print {
  public:
    SvcSerialCommandInterface():AppBaseClass(){
        update_priority = 0;  //set to max priority
        et_since_poll = 0;
        et_since_periodic_data_dict_tx = 0;
        et_since_periodic_stats_tx = 0;
        index_rx_buffer = 0;
        index_tx_buffer = 0;
        index_capture_buffer = 0;
        stream_pos = 0;
        is_streaming_file = false;
        is_capturing_bulk_data = false;
        tx_buffer_overflow_flag = false;
        is_periodic_messages_enabled = true;
#ifdef USE_EXTMEM
        p_capture_buffer = (char*)extmem_malloc(SERIAL_RX_CAPTURE_BUFFER_SIZE);
        p_working_buffer = (char*)extmem_malloc(SERIAL_WORKING_BUFFER_SIZE);
#else
        captureBuffer = 0;
        workingBuffer = 0;
#endif
        strcpy(multipart_header,"");
        strcpy(stream_path,"");
        strcpy(stream_file,"");
        stream_pos = 0;
        p_SD = AppManager::getInstance()->getSD();
        strcpy(name,"SCI");
        memset(tx_Buffer,0,SERIAL_OUTPUT_BUFFER_SIZE+1);
        memset(received_chars,0,SERIAL_RX_BUFFER_SIZE);
    }; 
    
    void empty(){
      memset(tx_Buffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      index_tx_buffer = 0;
    }

    bool requestStartLZ4Message(){
      if(is_streaming_file) return false;
      startLZ4Message();
      return true;
    }
    void sendLZ4Message();
    void send();
    bool throttle();
  protected:
    SdFs *p_SD;
    FsFile file;
    uint16_t index_rx_buffer;
    uint32_t index_capture_buffer;
    uint16_t index_tx_buffer;
    char multipart_header[SERIAL_TX_HEADER_BUFFER_SIZE];
    char received_chars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char stream_path[SERIAL_PARAM_BUFFER_SIZE];
    char stream_file[SERIAL_PARAM_BUFFER_SIZE];
    char tx_Buffer[SERIAL_OUTPUT_BUFFER_SIZE];
    //used for lz4 tx compressor
    char *p_working_buffer;
    char *p_capture_buffer;
    uint64_t stream_pos;
    bool is_streaming_file;
    bool is_capturing_bulk_data;
    bool is_periodic_messages_enabled;
    volatile bool tx_buffer_overflow_flag;
    elapsedMillis et_since_poll;
    elapsedMillis et_since_periodic_data_dict_tx;
    elapsedMillis et_since_periodic_stats_tx;
    uint16_t checksum(const char *msg);
    void streamHandler();
    void render() override{};    //called only when the app is active
    void update() override;
    void txOverflowHandler();
    void startLZ4Message(){ 
      empty();
      Serial.print("LZ4");
    }
    size_t write(uint8_t c){
      tx_Buffer[index_tx_buffer++] = c;
      if (index_tx_buffer == SERIAL_OUTPUT_BUFFER_SIZE-10){
        tx_Buffer[index_tx_buffer++] = 0; //make sure the buffer is null terminated
        txOverflowHandler();
      }
      return 1;
    };
    void flush(){
      while(throttle()){
        delay(50);
      }
      if (strlen(tx_Buffer) > 0 ) Serial.print(tx_Buffer);
      memset(tx_Buffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      index_tx_buffer = 0;
      tx_buffer_overflow_flag = false;
    };
};

#endif