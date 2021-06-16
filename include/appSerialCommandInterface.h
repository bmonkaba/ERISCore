#include "AppManager.h"
// AppSerialCommandInterface
//
/*





INCOMMING MESSAGES:

    LS [PATH]
    LDI [PATH]



OUTPUT MESSAGES:

    CQT_H  (CQT BINS - High Range)
    CQT_L  (CQT  "     Low Range")
    CQT_EOF (End of analysis frame)
    DIR  (directory contents - aka the result of the ls command)
    DIR_EOF
    S   (audio data stream)
    I   (img data stream)

*/
//
#define SERIAL_POLLING_RATE_MAX 100
#define SERIAL_RX_BUFFER_SIZE 128

class AppSerialCommandInterface:public AppBaseClass {
  public:
    AppSerialCommandInterface():AppBaseClass(){
        sincePoll = 0;
        indexRxBuffer = 0;
    }; 
    //define event handlers
  protected:
    byte indexRxBuffer;
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    elapsedMillis sincePoll;
    void update(){};    //called only when the app is active
    void updateRT(){
        if (sincePoll < SERIAL_POLLING_RATE_MAX) return;
        sincePoll = 0;
        char endMarker = '\n';
        char rxChr;
        boolean newRxMsg = false;
    
        while (Serial.available() > 0 && false == newRxMsg) {
            rxChr = Serial.read();
            receivedChars[indexRxBuffer++] = rxChr;
            if (rxChr == endMarker){
                receivedChars[--indexRxBuffer] = '\0'; //remove the end marker and null terminate the string
                indexRxBuffer = 0; //reset the write index
                newRxMsg = true;
            }
            if (newRxMsg){
                char cmd[128], param[128];
                int total_read;
                total_read = sscanf(receivedChars, "%s %s" , cmd, param);
                if (total_read < 2){
                    Serial.println(F("DIR"));
                    AppManager::getInstance()->getSD()->chdir();
                    AppManager::getInstance()->getSD()->ls();
                    Serial.println(F("DIR_EOF"));
                } else{
                    Serial.println(F("DIR"));
                    AppManager::getInstance()->getSD()->ls(param);
                    Serial.println(F("DIR_EOF"));
                }
                newRxMsg = false;
            }
        }
    }; //allways called even if app is not active
    void onFocus(){};   //called when given focus
    void onFocusLost(){}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y){
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
    };
};