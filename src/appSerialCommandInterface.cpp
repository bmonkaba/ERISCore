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
#include "appSerialCommandInterface.h"
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
int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}


uint16_t AppSerialCommandInterface::checksum(const char *msg){
    uint16_t csum;
    csum=0;
    for(uint16_t i = 0;i<strlen(msg);i++){
        csum += msg[i];
    }
    return csum;
}

void AppSerialCommandInterface::streamHandler(){
    char bufferChr;
    char hexBuffer[8];
    uint16_t payload_len;

    if (streamPos == 0) Serial.println(F("FS_START"));
    pSD->chdir(streamPath);

    if(Serial.availableForWrite() < SERIAL_FILESTREAM_PAYLOAD_SIZE){
        //serial tx buffer not available
        //Serial.println(F("GET_WRN throttling"));
        return; 
    }
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
        if(i>0){
            //if i is zero then last chunk was eof
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
        }
        file.close();

        if (payload_len == 0){
            //no data to tx; @ eof
            Serial.println(F("FS_END"));
            Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            return;
        }
        else if (payload_len < SERIAL_FILESTREAM_PAYLOAD_SIZE) {
            //last chunk
            Serial.flush();
            Serial.print(txBuffer);
            Serial.print(",");
            Serial.println(checksum(txBuffer));
            Serial.println(F("FS_END"));
            Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            return;
        } else{
            //send file chunk
            Serial.flush();
            Serial.print(txBuffer);
            Serial.print(",");
            Serial.println(checksum(txBuffer));
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



void AppSerialCommandInterface::updateRT(){
    //tx periodic messages
    if(Serial.availableForWrite() < SERIAL_FILESTREAM_PAYLOAD_SIZE){
        //serial tx buffer not available
        //Serial.println(F("GET_WRN throttling"));
        return; 
    }
    #ifdef SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
    if (sincePeriodic > SERIAL_AUTO_TRANSMIT_DATA_PERIOD){
        sincePeriodic = 0;
        AppManager::getInstance()->data->printDictionary();
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
            if (strncmp(cmd, "LS",sizeof(cmd)) == 0){
                if (total_read < 2){
                    Serial.println(F("DIR"));
                    AppManager::getInstance()->getSD()->chdir();
                    AppManager::getInstance()->getSD()->ls();
                    Serial.println(F("DIR_EOF"));
                    Serial.flush();
                } else{
                    replacechar(param,':',' '); //replace space token used to tx the path
                    Serial.print("M ");
                    Serial.println(param);
                    Serial.println(F("DIR"));
                    AppManager::getInstance()->getSD()->ls(param);
                    Serial.println(F("DIR_EOF"));
                    Serial.flush();
                }
            } else if (strncmp(cmd, "GET",sizeof(cmd)) == 0){
                total_read = sscanf(receivedChars, "%s %s %s" , cmd, param,param2);
                if (total_read < 3){
                    Serial.print(F("GET_ERR WRONG PARAM COUNT"));
                    Serial.println(param);
                    Serial.flush();
                } else{
                    //file streaming request ok
                    //init the transfer
                    Serial.print(F("GET_OK ")); 
                    Serial.print(param);
                    Serial.print(" ");
                    Serial.println(param2);
                    Serial.flush();
                    strcpy(streamPath,param);
                    strcpy(streamFile,param2);
                    isStreamingFile = true;
                }
            } else if (strncmp(cmd, "ACON",sizeof(cmd)) == 0){ //audio connections
                uint16_t i;  
                char csBuffer[128];
                i = 0;
                Serial.println(F("M ACON START"));
                /*
                while(ad->getAudioStreamString(i,csBuffer)){
                    i += 1;
                    Serial.flush();
                    Serial.println(csBuffer);
                    Serial.flush();
                }
                i=0;
                */
                while(ad->getConnectionString(i,csBuffer)){
                    i += 1;
                    
                    Serial.println(csBuffer);
                }
                Serial.println(F("M ACON END"));
                Serial.flush();

            } else if (strncmp(cmd, "CONNECT",sizeof(cmd)) == 0){
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
                    //AudioNoInterrupts();
                    ad->connect(param,source_port,param2,dest_port);
                    //AudioInterrupts();
                    Serial.flush();
                }
            } else if (strncmp(cmd, "DISCONNECT",sizeof(cmd)) == 0){
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
                    //AudioNoInterrupts();
                    ad->disconnect(param,dest_port);
                    //AudioInterrupts();
                    Serial.flush();
                }
            } else if (strncmp(cmd, "AA",sizeof(cmd)) == 0){         //active app message
                if (total_read > 1) AppManager::getInstance()->getActiveApp()->MessageHandler(this,param);
                //Serial.print(F("AA OK "));
                //Serial.println(AppManager::getInstance()->getActiveApp()->name);
                //Serial.flush();
            }else if (strncmp(cmd, "STATS",sizeof(cmd)) == 0){ 
                ad->printStats();
                AppManager::getInstance()->printStats();
            }else if (strncmp(cmd, "CQT_CFG",sizeof(cmd)) == 0){ 
                AppManager::getInstance()->sendMessage(this,"AppCQT","CQT_INFO");
            }else if (strncmp(cmd, "GET_DD",sizeof(cmd)) == 0){ 
                AppManager::getInstance()->data->printDictionary();
            }else if (strncmp(cmd, "GET_RAM2",sizeof(cmd)) == 0){ 
                char* mp = 0;
                char c;
                strcpy(txBuffer," ");
                for(uint32_t i = 0x20200000; i < 0x2027F000; i+=1){ 
                    while(Serial.availableForWrite() < 5048){
                        delay(35);
                    }
                    mp = (char*)i;
                    c = *mp;
                    c = (c & 0xFF);
                    if(i%64==0){
                        Serial.println(txBuffer);
                        strcpy(txBuffer," ");
                        Serial.printf("%08X",i);
                        Serial.print("  ");
                    }

                    Serial.printf("%02X ",(uint8_t)c);

                    if (isprint((int)c)){ 
                        strncat(txBuffer, &c, 1);
                    }
                    else if (c==0){
                        c = '*';
                        strncat(txBuffer, &c, 1);
                    } else{
                        c = '.';
                        strncat(txBuffer, &c, 1);
                    }
                    //Serial.print(c);
                }
                Serial.flush();
            }else if (strncmp(cmd, "GET_RAM1",sizeof(cmd)) == 0){ 
                char* mp = 0;
                char c;
                strcpy(txBuffer," ");
                for(uint32_t i = 0x0007A1E0; i < 0x7F000; i+=1){ 
                    mp = (char*)i;
                    c = *mp;
                    c = (c & 0xFF);
                    if(i%64==0){
                        Serial.println(txBuffer);
                        strcpy(txBuffer," ");
    
                        Serial.printf("%08X",i);
                        Serial.print("  ");
                        Serial.flush();
                        delay(80);
                    }

                    if (isprint((int)c)){ 
                        strncat(txBuffer, &c, 1);
                    }
                    else if (c==0){
                        c = '*';
                        strncat(txBuffer, &c, 1);
                    } else{
                        c = '.';
                        strncat(txBuffer, &c, 1);
                    }
                    

                    Serial.printf("%02X ",c);
                    //Serial.print(c);
                }
                Serial.flush();

            }else if (strncmp(cmd, "AUDIO_NO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioNoInterrupts();
            }else if (strncmp(cmd, "AUDIO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioInterrupts();
            }
            //END
            newRxMsg = false;
        }
    }
    delayNanoseconds(20);
}; //allways called even if app is not active