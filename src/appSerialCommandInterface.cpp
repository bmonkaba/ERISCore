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
#include <ctype.h>
#include "appSerialCommandInterface.h"
#include "AppManager.h"
// AppSerialCommandInterface
//
/*

INCOMMING MESSAGES:

    HELO
    LS [PATH]
    GET [PATH]  (get file is responded with FS messages)
    ACON        (request current audio block connections)
    CONNECT     (make an audio block connection)
    DISCONNECT  (break an audio block connection)
    AA          (broadcast message to active app )
    STATS
    CQT_CFG     request a dump of the cqt bin configs
    GET_DD      request a dump of the data dictionary   
    GET_RAM2    reqquest a dump of ram2   

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
    RAM {JSON}


*/
//
extern SdFs sd;

const char* gWelcomeMessage = 
"M \n" 
"M .▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄.\n"
"M ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌\n"
"M ▐░█▀▀▀▀▀▀▀▀▀.▐░█▀▀▀▀▀▀▀█░▌.▀▀▀▀█░█▀▀▀▀.▐░█▀▀▀▀▀▀▀▀▀.\n"
"M ▐░▌..........▐░▌.......▐░▌.....▐░▌.....▐░▌..........\n"
"M ▐░█▄▄▄▄▄▄▄▄▄.▐░█▄▄▄▄▄▄▄█░▌.....▐░▌.....▐░█▄▄▄▄▄▄▄▄▄.\n"
"M ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌.....▐░▌.....▐░░░░░░░░░░░▌\n"
"M ▐░█▀▀▀▀▀▀▀▀▀.▐░█▀▀▀▀█░█▀▀......▐░▌......▀▀▀▀▀▀▀▀▀█░▌\n"
"M ▐░▌..........▐░▌.....▐░▌.......▐░▌...............▐░▌\n"
"M ▐░█▄▄▄▄▄▄▄▄▄.▐░▌......▐░▌..▄▄▄▄█░█▄▄▄▄..▄▄▄▄▄▄▄▄▄█░▌\n"
"M ▐░░░░░░░░░░░▌▐░▌.......▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌\n"
"M .▀▀▀▀▀▀▀▀▀▀▀..▀.........▀..▀▀▀▀▀▀▀▀▀▀▀..▀▀▀▀▀▀▀▀▀▀▀.\n"
"M ....................................................\n"
"M .▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄..▄▄▄▄▄▄▄▄▄▄▄.\n"
"M ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌\n"
"M ▐░█▀▀▀▀▀▀▀▀▀.▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀.\n"
"M ▐░▌..........▐░▌.......▐░▌▐░▌.......▐░▌▐░▌..........\n"
"M ▐░▌..........▐░▌.......▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄.\n"
"M ▐░▌..........▐░▌.......▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌\n"
"M ▐░▌..........▐░▌.......▐░▌▐░█▀▀▀▀█░█▀▀.▐░█▀▀▀▀▀▀▀▀▀.\n"
"M ▐░▌..........▐░▌.......▐░▌▐░▌.....▐░▌..▐░▌..........\n"
"M ▐░█▄▄▄▄▄▄▄▄▄.▐░█▄▄▄▄▄▄▄█░▌▐░▌......▐░▌.▐░█▄▄▄▄▄▄▄▄▄.\n"
"M ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌.......▐░▌▐░░░░░░░░░░░▌\n"
"M .▀▀▀▀▀▀▀▀▀▀▀..▀▀▀▀▀▀▀▀▀▀▀..▀.........▀..▀▀▀▀▀▀▀▀▀▀▀.\n";


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

void AppSerialCommandInterface::throttle(){
    uint16_t avail;
    avail = Serial.availableForWrite();

    if(avail < 1000){
        uint16_t delta_avail;
        //serial tx buffer not available
        delayMicroseconds(3000);
        delta_avail = Serial.availableForWrite();
        if(avail == delta_avail){
            Serial.flush();
            Serial.println(F("M WRN flush"));
        } else Serial.println(F("M WRN throttling"));
    }
    return;
}

void AppSerialCommandInterface::streamHandler(){
    char bufferChr;
    char hexBuffer[8];
    uint16_t payload_len;
    
    throttle();

    if (streamPos == 0) Serial.println(F("FS_START"));
    pSD->chdir(streamPath);
    if (pSD->exists(streamFile)){
        payload_len = 0;

        strcpy(txBuffer,"FS ");

        if(!file.open(streamFile, O_READ)){ 
            Serial.println(F("M GET_ERR FILE OPEN ERROR"));
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
                    Serial.println(F("M GET_ERR FILE READ ERROR"));
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
            delayMicroseconds(2000);
            //Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            return;
        }
        else if (payload_len < SERIAL_FILESTREAM_PAYLOAD_SIZE) {
            //last chunk
            //Serial.flush();
            Serial.print(txBuffer);
            Serial.print(",");
            Serial.println(checksum(txBuffer));
            Serial.println(F("FS_END"));
            //Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            return;
        } else{
            //send file chunk
            Serial.print(txBuffer);
            Serial.print(",");
            Serial.println(checksum(txBuffer));
            Serial.flush();
            delayMicroseconds(5500);
            return;
        }             
    } 
    else{
        Serial.print(F("M GET_ERR FILE NOT FOUND "));
        Serial.println(streamFile);
        isStreamingFile = false;
        streamPos = 0;
        return;
    }
};


void AppSerialCommandInterface::updateRT(){
    //throttle 
    throttle();

    char endMarker = '\n';
    boolean newRxMsg = false;
    char bufferChr;

    if (isStreamingFile){
        streamHandler();
    }else if (Serial.available() > 0 && false == newRxMsg ) {
        bufferChr = Serial.read();
        receivedChars[indexRxBuffer++] = bufferChr;
        if(indexRxBuffer >= (SERIAL_RX_BUFFER_SIZE-16)){
            //input overflow - clear serial input and reset the index
            indexRxBuffer = 0;
            Serial.clear();
            Serial.clearReadError();
            return;
        }
        if (bufferChr == endMarker){
            receivedChars[--indexRxBuffer] = '\0'; //remove the end marker and null terminate the string
            indexRxBuffer = 0; //reset the input write index
            newRxMsg = true;
        }

        if (newRxMsg){
            char cmd[128], param[128],param2[128];
            int total_read;
            total_read = sscanf(receivedChars, "%s %s" , cmd, param);
            if (strncmp(cmd, "LS",sizeof(cmd)) == 0){
                if (total_read < 2){
                    Serial.println(F("DIR"));
                    sd->chdir();
                    sd->ls();
                    //AppManager::getInstance()->getSD()->chdir();
                    //AppManager::getInstance()->getSD()->ls();
                    Serial.println(F("DIR_EOF"));
                    Serial.flush();
                } else{
                    replacechar(param,':',' '); //replace space token used to tx the path
                    Serial.print("M ");
                    Serial.println(param);
                    Serial.println(F("DIR"));
                    pSD->ls(param);
                    Serial.println(F("DIR_EOF"));
                    Serial.flush();
                }
            } else if (strncmp(cmd, "GET",sizeof(cmd)) == 0){
                total_read = sscanf(receivedChars, "%s %s %s" , cmd, param,param2);
                if (total_read < 3){
                    Serial.print(F("M GET_ERR WRONG PARAM COUNT"));
                    Serial.println(param);
                    //Serial.flush();
                } else{
                    //file streaming request ok
                    //init the transfer
                    Serial.print(F("M GET_OK ")); 
                    Serial.print(param);
                    Serial.print(" ");
                    Serial.println(param2);
                    //Serial.flush();
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
                    //Serial.flush();
                    Serial.println(csBuffer);
                    //Serial.flush();
                }
                i=0;
                */
                while(ad->getConnectionString(i,csBuffer)){
                    i += 1;
                    Serial.print("M ");
                    Serial.println(csBuffer);
                }
                Serial.println(F("M ACON END"));
                //Serial.flush();

            } else if (strncmp(cmd, "CONNECT",sizeof(cmd)) == 0){
                int source_port;
                int dest_port;
                
                total_read = sscanf(receivedChars, "%s %s %d %s %d" , cmd, param,&source_port,param2,&dest_port);
                if (total_read < 3){
                    Serial.print(F("M CONNECT WRONG PARAM COUNT "));
                    Serial.println(receivedChars);
                } else{
                    ad->connect(param,source_port,param2,dest_port);
                }
            } else if (strncmp(cmd, "M DISCONNECT",sizeof(cmd)) == 0){
                int dest_port;
                total_read = sscanf(receivedChars, "%s %s %d" , cmd, param,&dest_port);
                if (total_read < 2){
                    Serial.print(F("M DISCONNECT WRONG PARAM COUNT "));
                    Serial.println(receivedChars);
                } else{
                    ad->disconnect(param,dest_port);
                }
            } else if (strncmp(cmd, "AA",sizeof(cmd)) == 0){         //active app message
                if (total_read > 1) AppManager::getInstance()->getActiveApp()->MessageHandler(this,param);
                //Serial.print(F("AA OK "));
                //Serial.println(AppManager::getInstance()->getActiveApp()->name);
                //Serial.flush();
            }else if (strncmp(cmd, "STATS",sizeof(cmd)) == 0){
                AppManager::getInstance()->printStats();
                ad->printStats();
            }else if (strncmp(cmd, "CQT_CFG",sizeof(cmd)) == 0){ 
                AppManager::getInstance()->sendMessage(this,"AppCQT","CQT_INFO");
            }else if (strncmp(cmd, "GET_DD",sizeof(cmd)) == 0){ 
                AppManager::getInstance()->data->printDictionary();
            }else if (strncmp(cmd, "UPDATE_DD",sizeof(cmd)) == 0){
                int32_t val;
                total_read = sscanf(receivedChars, "%s %s %d" , cmd, param,(int*)&val);
                if (total_read < 3){
                    Serial.print(F("M WRONG PARAM COUNT"));
                    Serial.println(total_read);
                }else{
                    am->data->update(param,val);
                }
            }else if (strncmp(cmd, "HELO",sizeof(cmd)) == 0){ 
                Serial.println(gWelcomeMessage);
            }else if (strncmp(cmd, "GET_RAM",sizeof(cmd)) == 0){ 
                char* mp;
                long num;
                num = strtol(param,NULL,10);
                mp = (char*)num;
                Serial.printf("M GET_RAM {\"addr\":\"%08X\",\"data\":\"%02X\"}\n",num,(uint8_t)*mp);
                //Serial.printf("M DEBUG {\"addr\":\"%08X\",\"data\":\"%02X\"}\n",&g_octave_down_shift,g_octave_down_shift);
            }else if (strncmp(cmd, "GET_RAM2",sizeof(cmd)) == 0){ 
                char* mp = 0;
                char c;
                delayMicroseconds(230000);
                Serial.printf(F("RAM {\"RAM2\":{\"addr\":\"%08X\",\"chunk\":\""),0x20000000);
                strcpy(txBuffer," ");
                //RAM2 always ends at 0x20280000
                for(uint32_t i = 0x20000000; i < 0x20280000; i+=1){
                    if (i == 0x20016000) i = 0x20200000;
                    mp = (char*)i;
                    c = *mp;
                    c = (c & 0xFF);
                    if(i%64==0 && i != 0x20000000){
                        Serial.print(F("\",\"decode\":\""));
                        Serial.print(txBuffer);
                        Serial.println(F("\"}}"));
                        strcpy(txBuffer,"");
                        delayMicroseconds(8000);
                        throttle();
                        if(i%1024==0){
                            float32_t pct;
                            pct = 100.0 * ((float)(i-0x20000000)/(float)(0x20280000-0x20000000));
                            Serial.printf(F("CLS\nM GET_RAM2 %08X %.0f pct "),i,pct);
                            for(uint16_t div = 0; div < (uint16_t)pct; div += 5){
                                Serial.printf("*");
                            }
                            Serial.println("");
                        }
                        Serial.printf(F("RAM {\"RAM2\":{\"addr\":\"%08X\",\"chunk\":\""),i);
                    }

                    Serial.printf("%02X ",(uint8_t)c);
                    const char* escape = "\\";
                    if (c=='"'){                        
                        strncat(txBuffer, escape, 1);
                        strncat(txBuffer, &c, 1);
                    }else if (c=='\''){
                        strncat(txBuffer, escape, 1);
                        strncat(txBuffer, &c, 1);
                    }else if (isprint((int)c)){ 
                        strncat(txBuffer, &c, 1);
                    }else if (iscntrl((int)c)){
                        c = '.';
                        strncat(txBuffer, &c, 1);
                    }else{
                        c = '?';
                        strncat(txBuffer, &c, 1);
                    }
                    //Serial.print(c);
                }
                Serial.println("\n");
                Serial.println(F("RAM END"));
                delayMicroseconds(1800000);
                //flush out serial input buffer
                Serial.clear();
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
            }else if (strncmp(cmd, "AUDIO_NO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioNoInterrupts();
            }else if (strncmp(cmd, "AUDIO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioInterrupts();
            }
            //END
            newRxMsg = false;
        }
    }
    sincePoll = 0;
    
    //tx periodic messages
    #ifdef SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
    if (sincePeriodic > SERIAL_AUTO_TRANSMIT_DATA_PERIOD){
        sincePeriodic = 0;
        AppManager::getInstance()->data->printDictionary();
    }
    #endif
    //Serial.flush();
}; //allways called even if app is not active