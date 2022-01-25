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
        updateRT_priority = 0;  //set to max priority
        sincePoll = 0;
        sincePeriodicDataDict = 0;
        sincePeriodicStats = 0;
        indexRxBuffer = 0;
        indexTxBuffer = 0;
        indexCaptureBuffer = 0;
        streamPos = 0;
        isStreamingFile = false;
        isCapturingBulkData = false;
        txBufferOverflowFlag = false;
        periodicMessagesEnabled = true;
#ifdef USE_EXTMEM
        captureBuffer = (char*)extmem_malloc(SERIAL_RX_CAPTURE_BUFFER_SIZE);
        workingBuffer = (char*)extmem_malloc(SERIAL_WORKING_BUFFER_SIZE);
#else
        captureBuffer = 0;
        workingBuffer = 0;
#endif
        strcpy(multiPartHeader,"");
        strcpy(streamPath,"");
        strcpy(streamFile,"");
        streamPos = 0;
        pSD = AppManager::getInstance()->getSD();
        strcpy(name,"SCI");
        memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE+1);
        memset(receivedChars,0,SERIAL_RX_BUFFER_SIZE);
    }; 
    
    void empty(){
      memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      indexTxBuffer = 0;
    }

    bool requestStartLZ4Message(){
      if(isStreamingFile) return false;
      startLZ4Message();
      return true;
    }
    void sendLZ4Message();
    void send();
    bool throttle();
  protected:
    SdFs *pSD;
    FsFile file;
    uint16_t indexRxBuffer;
    uint32_t indexCaptureBuffer;
    uint16_t indexTxBuffer;
    char multiPartHeader[SERIAL_TX_HEADER_BUFFER_SIZE];
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char streamPath[SERIAL_PARAM_BUFFER_SIZE];
    char streamFile[SERIAL_PARAM_BUFFER_SIZE];
    char txBuffer[SERIAL_OUTPUT_BUFFER_SIZE];
    //used for lz4 tx compressor
    char *workingBuffer;
    char *captureBuffer;
    uint64_t streamPos;
    bool isStreamingFile;
    bool isCapturingBulkData;
    bool periodicMessagesEnabled;
    volatile bool txBufferOverflowFlag;
    elapsedMillis sincePoll;
    elapsedMillis sincePeriodicDataDict;
    elapsedMillis sincePeriodicStats;
    uint16_t checksum(const char *msg);
    void streamHandler();
    void update() override{};    //called only when the app is active
    void updateRT() override;
    void txOverflowHandler();
    void startLZ4Message(){ 
      empty();
      Serial.print("LZ4");
    }
    size_t write(uint8_t c){
      txBuffer[indexTxBuffer++] = c;
      if (indexTxBuffer == SERIAL_OUTPUT_BUFFER_SIZE-10){
        txBuffer[indexTxBuffer++] = 0; //make sure the buffer is null terminated
        txOverflowHandler();
      }
      return 1;
    };
    void flush(){
      while(throttle()){
        delay(50);
      }
      if (strlen(txBuffer) > 0 ) Serial.print(txBuffer);
      memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      indexTxBuffer = 0;
      txBufferOverflowFlag = false;
    };
};

#endif