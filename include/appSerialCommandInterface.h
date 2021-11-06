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
#include "SdFat-beta.h"
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

*/
//

using namespace std;



class AppSerialCommandInterface:public AppBaseClass {
  public:
    AppSerialCommandInterface():AppBaseClass(){
        sincePoll = 0;
        sincePeriodic = 0;
        indexRxBuffer = 0;
        isStreamingFile = false;
        strcpy(streamPath,"");
        strcpy(streamFile,"");
        streamPos = 0;
        pSD = AppManager::getInstance()->getSD();
        strcpy(name,"SCI");
        txBuffer[0] = 0;
        receivedChars[0] = 0;
    }; 
    //define event handlers
  protected:
    SdFs *pSD;
    FsFile file;
    uint16_t indexRxBuffer;
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char streamPath[SERIAL_PARAM_BUFFER_SIZE];
    char streamFile[SERIAL_PARAM_BUFFER_SIZE];
    char txBuffer[SERIAL_OUTPUT_BUFFER_SIZE];
    uint64_t streamPos;
    bool isStreamingFile;
    elapsedMillis sincePoll;
    elapsedMillis sincePeriodic;
    uint16_t checksum(const char *msg);
    void streamHandler();
    void update() override{};    //called only when the app is active
    void updateRT() override;
    void onFocus()override{};   //called when given focus
    void onFocusLost()override{}; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y)override{
        //check if touch point is within the application bounding box
       if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t t_x, uint16_t t_y)override{
    };
};

#endif