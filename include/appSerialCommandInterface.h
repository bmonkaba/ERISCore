/**
 * @file appSerialCommandInterface.h
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

class AppSerialCommandInterface:public AppBaseClass, Print {
  public:
    AppSerialCommandInterface():AppBaseClass(){
        sincePoll = 0;
        sincePeriodic = 0;
        indexRxBuffer = 0;
        indexTxBuffer = 0;
        isStreamingFile = false;
        txBufferOverflowFlag = false;
        strcpy(streamPath,"");
        strcpy(streamFile,"");
        streamPos = 0;
        pSD = AppManager::getInstance()->getSD();
        strcpy(name,"SCI");
        memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
        memset(receivedChars,0,SERIAL_RX_BUFFER_SIZE);
    }; 
    //define event handlers
  protected:
    SdFs *pSD;
    FsFile file;
    uint16_t indexRxBuffer;
    uint16_t indexTxBuffer;
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char streamPath[SERIAL_PARAM_BUFFER_SIZE];
    char streamFile[SERIAL_PARAM_BUFFER_SIZE];
    char txBuffer[SERIAL_OUTPUT_BUFFER_SIZE + 1];
    char workingBuffer[SERIAL_OUTPUT_BUFFER_SIZE + 1];
    uint64_t streamPos;
    bool isStreamingFile;
    volatile bool txBufferOverflowFlag;
    elapsedMillis sincePoll;
    elapsedMillis sincePeriodic;
    uint16_t checksum(const char *msg);
    void throttle();
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
        //2x transmit buffer size
        txBufferOverflowFlag = true;
      }
      return 1;
    };
    void flush(){
      while(Serial.availableForWrite() < 6000){
        delay(5);
      }
      if (strlen(txBuffer) > 0 ) Serial.print(txBuffer);
      memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      indexTxBuffer = 0;
    };
    void empty(){
      memset(txBuffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
      indexTxBuffer = 0;
    }
};

#endif