#include "AppManager.h"
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
#define SERIAL_POLLING_RATE_MAX 5
#define SERIAL_RX_BUFFER_SIZE 128
#define SERIAL_PARAM_BUFFER_SIZE 128
#define SERIAL_OUTPUT_BUFFER_SIZE 1024
#define SERIAL_FILESTREAM_PAYLOAD_SIZE 128

#define SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
#define SERIAL_AUTO_TRANSMIT_DATA_PERIOD 40

int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}

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
    byte indexRxBuffer;
    char receivedChars[SERIAL_RX_BUFFER_SIZE];   // an array to store the received data
    char streamPath[SERIAL_PARAM_BUFFER_SIZE];
    char streamFile[SERIAL_PARAM_BUFFER_SIZE];
    char txBuffer[SERIAL_OUTPUT_BUFFER_SIZE];
    uint64_t streamPos;
    bool isStreamingFile;
    elapsedMillis sincePoll;
    elapsedMillis sincePeriodic;

    void streamHandler(){
        char bufferChr;
        char hexBuffer[8];
        uint16_t payload_len;
        
        if (streamPos == 0) Serial.println(F("FS_START"));
        pSD->chdir(streamPath);
        if (pSD->exists(streamFile)){
            payload_len = 0;

            strcpy(txBuffer,"FS ");

            if(!file.open(streamFile, O_READ)){ 
                Serial.println(F("GET_ERR FILE OPEN ERROR"));
                isStreamingFile = false;
                streamPos = 0;
                return;
            }
            file.seek(streamPos);
            //send the next chunked message until eof
            uint64_t i;
            i = file.available();
            if(i > SERIAL_FILESTREAM_PAYLOAD_SIZE) i = SERIAL_FILESTREAM_PAYLOAD_SIZE;
            for(;i > 0; i--){
                payload_len += 1;
                if (file.read(&bufferChr,1) < 0){
                    Serial.println(F("GET_ERR FILE READ ERROR "));
                    isStreamingFile = false;
                    streamPos = 0;
                    file.close();
                    return;
                }
                sprintf(hexBuffer,"%02X,",(unsigned int)bufferChr);
                strcat(txBuffer,hexBuffer);
                //strcat(txBuffer,",");
            }
            
            txBuffer[strlen(txBuffer)-1] = '\0'; //remove last comma
            streamPos = file.position();
            file.close();

            if (payload_len < SERIAL_FILESTREAM_PAYLOAD_SIZE) {
                //last chunk
                //file.close();
                Serial.flush();
                Serial.println(txBuffer);
                Serial.println(F("FS_END"));
                Serial.flush();
                isStreamingFile = false;
                streamPos = 0;
                return;
            } else{
                //send file chunk
                Serial.flush();
                Serial.println(txBuffer);
                Serial.flush();
                return;
            }             
        } 
        else{
            Serial.print(F("GET_ERR FILE NOT FOUND "));
            Serial.println(streamFile);
            isStreamingFile = false;
            streamPos = 0;
            return;
        }
    };


    void update() override{};    //called only when the app is active
    void updateRT() override{
        //tx periodic messages
        #ifdef SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
        if (sincePeriodic > SERIAL_AUTO_TRANSMIT_DATA_PERIOD){
            sincePeriodic = 0;
            AppManager::getInstance()->data.printDictionary();
        }
        #endif
        //poll for rx data
        if (sincePoll < SERIAL_POLLING_RATE_MAX) return;
        sincePoll = 0;
        char endMarker = '\n';
        boolean newRxMsg = false;
        char bufferChr;
    
        if (isStreamingFile){
            streamHandler();
        }else while (Serial.available() > 0 && false == newRxMsg) {
            bufferChr = Serial.read();
            receivedChars[indexRxBuffer++] = bufferChr;
            if (bufferChr == endMarker){
                receivedChars[--indexRxBuffer] = '\0'; //remove the end marker and null terminate the string
                indexRxBuffer = 0; //reset the write index
                newRxMsg = true;
            }
            if (newRxMsg){
                char cmd[128], param[128],param2[128];
                int total_read;
                total_read = sscanf(receivedChars, "%s %s" , cmd, param);
                if (strcmp(cmd, "LS") == 0){
                    if (total_read < 2){
                        Serial.println(F("DIR"));
                        AppManager::getInstance()->getSD()->chdir();
                        AppManager::getInstance()->getSD()->ls();
                        Serial.println(F("DIR_EOF"));
                    } else{
                        replacechar(param,':',' '); //replace space token used to tx the path
                        Serial.print("M ");
                        Serial.println(param);
                        Serial.println(F("DIR"));
                        AppManager::getInstance()->getSD()->ls(param);
                        Serial.println(F("DIR_EOF"));
                    }
                } else if (strcmp(cmd, "GET") == 0){
                    total_read = sscanf(receivedChars, "%s %s %s" , cmd, param,param2);
                    if (total_read < 3){
                        Serial.print(F("GET_ERR WRONG PARAM COUNT"));
                        Serial.println(param);
                    } else{
                        //file streaming request ok
                        //init the transfer
                        Serial.print(F("GET_OK ")); 
                        Serial.print(param);
                        Serial.print(" ");
                        Serial.println(param2);
                        strcpy(streamPath,param);
                        strcpy(streamFile,param2);
                        isStreamingFile = true;
                    }
                } else if (strcmp(cmd, "ACON") == 0){ //audio connections
                    uint16_t i;  
                    char csBuffer[128];
                    i = 0;
                    Serial.println(F("M ACON START"));
                    /*
                    while(ad.getAudioStreamString(i,csBuffer)){
                        i += 1;
                        Serial.flush();
                        Serial.println(csBuffer);
                        Serial.flush();
                    }
                    i=0;
                    */
                    while(ad.getConnectionString(i,csBuffer)){
                        i += 1;
                        
                        Serial.println(csBuffer);
                        Serial.flush();
                    }
                    Serial.println(F("M ACON END"));

                } else if (strcmp(cmd, "CONNECT") == 0){
                    int source_port;
                    int dest_port;
                    
                    total_read = sscanf(receivedChars, "%s %s %d %s %d" , cmd, param,&source_port,param2,&dest_port);
                    if (total_read < 3){
                        Serial.flush();
                        Serial.print(F("CONNECT WRONG PARAM COUNT "));
                        Serial.println(receivedChars);
                        Serial.flush();
                    } else{
                        Serial.flush();
                        //Serial.println(F("CONNECT OK"));
                        AudioNoInterrupts();
                        ad.connect(param,source_port,param2,dest_port);
                        AudioInterrupts();
                        Serial.flush();
                    }
                } else if (strcmp(cmd, "DISCONNECT") == 0){
                    int dest_port;
                    total_read = sscanf(receivedChars, "%s %s %d" , cmd, param,&dest_port);
                    if (total_read < 2){
                        Serial.flush();
                        Serial.print(F("DISCONNECT WRONG PARAM COUNT "));
                        Serial.println(receivedChars);
                        Serial.flush();
                    } else{
                        Serial.flush();
                        //Serial.println(F("DISCONNECT OK"));
                        AudioNoInterrupts();
                        ad.disconnect(param,dest_port);
                        AudioInterrupts();
                        Serial.flush();
                    }
                } else if (strcmp(cmd, "AA") == 0){         //active app message
                    if (total_read > 1) AppManager::getInstance()->getActiveApp()->MessageHandler(this,param);
                    //Serial.print(F("AA OK "));
                    //Serial.println(AppManager::getInstance()->getActiveApp()->name);
                    //Serial.flush();
                }else if (strcmp(cmd, "STATS") == 0){ 
                    ad.printStats();
                    AppManager::getInstance()->printStats();
                }else if (strcmp(cmd, "CQT_CFG") == 0){ 
                    AppManager::getInstance()->sendMessage(this,"AppCQT","CQT_INFO");
                }else if (strcmp(cmd, "GET_DD") == 0){ 
                    AppManager::getInstance()->data.printDictionary();
                }

                newRxMsg = false;
            }
        }
    }; //allways called even if app is not active
    void onFocus()override{};   //called when given focus
    void onFocusLost()override{}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y)override{
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y)override{
    };
};