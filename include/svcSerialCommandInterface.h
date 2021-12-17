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

#include "globaldefs.h"
#include <string.h>
#include "AppManager.h"
#include "AppBaseClass.h"
#include <SdFat.h>
#include <Print.h>
//#include <SdFat.h>
// AppSerialCommandInterface
//
/*

INCOMMING MESSAGES:

    LS [PATH]
    GET [PATH]  (get file is responded with FS messages)
    ACON        (request current audio block connections)
    CONNECT     (make an audio block connection)
    DISCONNECT  (break an audio block connection)
    AA          (broadcast message to active app )
    STATS
    CQT_CFG     request a dump of the cqt bin configs
    GET_DD      request a dump of the data dictionary       
    UPDATE_DD key val   set a key value 
OUTPUT MESSAGES:

    CQT_H  (CQT BINS - High Range)
    CQT_L  (CQT  "     Low Range")
    CQT_EOF (End of analysis frame)
    DIR  (directory contents - aka the result of the ls command)
    DIR_EOF
    S   (FFT window stream)
    FS   (file stream)
    FS_END (file stream end, indicate to client eof)
    GET_ERR (error response )
    ACON START (begin audio connection list)
    ACON END (end audio connection list)
    RAM   JSON container
*/
//

class SvcSerialCommandInterface:public AppBaseClass, public Print {
  public:
    SvcSerialCommandInterface():AppBaseClass(){
        sincePoll = 0;
        sincePeriodicDataDict = 0;
        sincePeriodicStats = 0;
        indexRxBuffer = 0;
        indexTxBuffer = 0;
        isStreamingFile = false;
        isCapturingBulkData = false;
        txBufferOverflowFlag = false;
        periodicMessagesEnabled = true;
        captureBuffer = 0;
        workingBuffer = 0;
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

    void startLZ4Message(){
      empty();
      Serial.print("LZ4");
    }

    void endLZ4Message();
  protected:
    SdFs *pSD;
    FsFile file;
    uint16_t indexRxBuffer;
    uint32_t indexCaptureBuffer;
    uint16_t indexTxBuffer;
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char streamPath[SERIAL_PARAM_BUFFER_SIZE];
    char streamFile[SERIAL_PARAM_BUFFER_SIZE];
    char txBuffer[SERIAL_OUTPUT_BUFFER_SIZE + 1];
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
    bool throttle();
    void streamHandler();
    void update() override{};    //called only when the app is active
    void updateRT() override;
    void txOverflowHandler();
    size_t write(uint8_t c){
      txBuffer[indexTxBuffer++] = c;
      if (indexTxBuffer == SERIAL_OUTPUT_BUFFER_SIZE){
        txOverflowHandler();
        //set the flag after the handler 
        //this allows the handler to detect multiple overflows
        //which is expected when streaming data larger than
        //the transmit buffer size
        txBufferOverflowFlag = true;
      }
      return 1;
    };
    void flush(){
      while(Serial.availableForWrite() < 6000){
        delay(15);
      }
      if (strlen(txBuffer) > 0 ) Serial.print(txBuffer);
      memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      indexTxBuffer = 0;
    };
};

#endif