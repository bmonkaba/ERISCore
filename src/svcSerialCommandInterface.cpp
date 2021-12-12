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

#include <ctype.h>
#include "svcSerialCommandInterface.h"
#include "AppManager.h"
#include "lz4.h"
#include <base64.hpp>
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

char* token;
const char newline = '\n';

const char* gWelcomeMessage = 
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
"M .▀▀▀▀▀▀▀▀▀▀▀..▀▀▀▀▀▀▀▀▀▀▀..▀.........▀..▀▀▀▀▀▀▀▀▀▀▀.\n"
"M Connected to the Serial Command Interface           \n";


int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}

uint16_t SvcSerialCommandInterface::checksum(const char *msg){
    uint16_t csum;
    csum=0;
    for(uint16_t i = 0;i<strlen(msg);i++){
        csum += msg[i];
    }
    return csum;
}

void replaceAll(char * str, char oldChar, char newChar)
{
    int i = 0;
    while(str[i] != '\0'){
        if(str[i] == oldChar) str[i] = newChar;
        i++;
    }
}

/**
 * @brief Calling this function signals the end of a compressed message. The current txBuffer is lz4 compressed before being base64 encoded and sent. 
 * The sent message format is "LZ4 [UNCOMPRESSED_SIZE] [BASE64 encoded LZ4 compressed message]" 
 * 
 */
void SvcSerialCommandInterface::endLZ4Message(){
    uint16_t compressed_len,uncompressed_len;
    //compress and transport the message
    workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
    memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
    uncompressed_len = indexTxBuffer;
    compressed_len = LZ4_compress_fast(txBuffer,workingBuffer,indexTxBuffer,SERIAL_WORKING_BUFFER_SIZE,1);
    empty();
    encode_base64((unsigned char *)workingBuffer, compressed_len, (unsigned char *)txBuffer);
    while(throttle()){
        delay(2);
    };
    Serial.print(" ");
    Serial.print(uncompressed_len);
    Serial.print(" ");
    Serial.println(txBuffer);
    empty();
    free(workingBuffer);
}

/**
 * @brief returns true if the available serial buffer falls below SERIAL_THROTTLE_BUFFER_REMAINING_THRESHOLD
 * if the buffer falls below SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD delay for a small period of time
 * if another check finds the buffer isn't moving then flush the buffer.
 * returns false otherwise
 * 
 * @return true 
 * @return false 
 */
bool SvcSerialCommandInterface::throttle(){
    uint16_t avail;
    avail = Serial.availableForWrite();

    if(avail < SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD){
        uint16_t delta_avail;
        //serial tx buffer not available
        delay(SERIAL_THROTTLE_CHECK_CONNECTION_DELAY_MSEC);
        delta_avail = Serial.availableForWrite();
        if(avail == delta_avail){
            Serial.flush();
            Serial.println(F("M WRN flush"));
        } else{
            //Serial.println(F("M WRN throttling"));//use this for debug only!
        }
    }
    if(avail < SERIAL_THROTTLE_BUFFER_THRESHOLD) return true;
    return false;
}

void SvcSerialCommandInterface::txOverflowHandler(){
    char* s;
    bool first;
    bool full;
    workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
    //delay(200);
    s = strrchr(txBuffer,'\n');//find last new line
    indexTxBuffer = (s-txBuffer);//len
    if (indexTxBuffer < SERIAL_OUTPUT_BUFFER_SIZE-1){ 
        full = false;
        memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
        memcpy(workingBuffer,txBuffer,indexTxBuffer);
        s += 1;
        //indexTxBuffer+=1;
        //copy any remaining chars to the begining of the txBuffer
        strcpy(txBuffer,s);
        memset(txBuffer + strlen(s)+1,0,SERIAL_OUTPUT_BUFFER_SIZE - strlen(s));
        indexTxBuffer = strlen(txBuffer);
    } else{
        full = true;
        memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
        memcpy(workingBuffer,txBuffer,SERIAL_OUTPUT_BUFFER_SIZE);
        empty();
    }
    token = strtok(workingBuffer, &newline);
    first = true;
    while( token != NULL ) {                        
        if (first && !txBufferOverflowFlag){
            Serial.print(F("\""));
        }else{
            if (full) Serial.print(F(",\""));
            else Serial.print(F(",\""));
        }
        Serial.print(token);
        Serial.print(F("\""));
        token = strtok(NULL, &newline);
        first = false;
    }
    //Serial.print(",\"dummy_value\"");
    free(workingBuffer);
}

void SvcSerialCommandInterface::streamHandler(){
    char bufferChr; //only need one char - 512 size is due to a potential bug in the teensy library identified by covintry static analysis
    char hexBuffer[8];
    uint16_t payload_len;
    uint16_t compressed_len,uncompressed_len;
    if(throttle()) return;
    if (streamPos == 0){
        Serial.println(F("FS_START"));
        periodicMessagesEnabled = false;
    }
    pSD->chdir(streamPath);
    if (pSD->exists(streamFile)){
        payload_len = 0;
        Serial.print(F("LZ4 "));
        //Serial.print("FS ");
        empty();
        print("FS ");
        //strcpy(txBuffer,"FS ");

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
                //strcat(txBuffer,hexBuffer);
                print(hexBuffer);
            }
            
            txBuffer[--indexTxBuffer] = '\0'; //remove last comma
            streamPos = file.position();
            delay(20);
        }
        file.close();

        if (payload_len == 0){
            //no data to tx; @ eof
            //Serial.print(",");
            //Serial.println(checksum(txBuffer));
            //println(F("0"));
            workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
            println();
            memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
            uncompressed_len = indexTxBuffer;
            compressed_len = LZ4_compress_fast(txBuffer,workingBuffer,indexTxBuffer,SERIAL_WORKING_BUFFER_SIZE,1);
            empty();
            encode_base64((unsigned char *)workingBuffer, strlen(workingBuffer), (unsigned char *)txBuffer);
            while(throttle()){
                delay(10);
            }
            Serial.print(" ");
            Serial.print(uncompressed_len-2);
            Serial.print(" ");
            Serial.println(txBuffer);
            Serial.println(F("FS_END"));
            empty();
            delay(100);
            //Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            free(workingBuffer);
            return;
        }
        else if (payload_len < SERIAL_FILESTREAM_PAYLOAD_SIZE) {
            //last chunk
            //Serial.flush();
            delayMicroseconds(20);
            //Serial.print(txBuffer);
            println();
            workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
            memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
            uncompressed_len = indexTxBuffer;
            compressed_len = LZ4_compress_fast(txBuffer,workingBuffer,indexTxBuffer,SERIAL_WORKING_BUFFER_SIZE,1);
            empty();
            encode_base64((unsigned char *)workingBuffer, compressed_len, (unsigned char *)txBuffer);
            while(throttle()){
                delay(10);
            }
            Serial.print(uncompressed_len);
            Serial.print(" ");
            Serial.println(txBuffer);
            empty();
            //Serial.print(",");
            //Serial.println(checksum(txBuffer));
            Serial.println(F("FS_END"));
            empty();
            //Serial.flush();
            isStreamingFile = false;
            streamPos = 0;
            delayMicroseconds(3000);
            periodicMessagesEnabled = true;
            free(workingBuffer);
            return;
        } else{
            //send file chunk
            //Serial.print(txBuffer);
            println();
            workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
            memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
            uncompressed_len = indexTxBuffer;
            compressed_len = LZ4_compress_fast(txBuffer,workingBuffer,indexTxBuffer,SERIAL_WORKING_BUFFER_SIZE,1);
            empty();
            encode_base64((unsigned char *)workingBuffer, compressed_len, (unsigned char *)txBuffer);
            Serial.print(uncompressed_len);
            Serial.print(" ");
            Serial.println(txBuffer);
            empty();
            free(workingBuffer);
            //Serial.print(",");
            //Serial.println(checksum(txBuffer));
            //Serial.flush();
            //delayMicroseconds(4000);
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


void SvcSerialCommandInterface::updateRT(){
    //throttle 
    if (throttle()) return;

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
                bool first;
                if (total_read < 2){
                    //send the root directory
                    delay(100);
                    empty();
                    println(F("DIR"));
                    print(F("LS . ["));
                    workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
                    strcpy(workingBuffer,txBuffer);
                    empty();
                    Serial.print(workingBuffer);
                    sd->chdir();
                    txBufferOverflowFlag = false;
                    sd->ls(this);
                    memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
                    strcpy(workingBuffer,txBuffer);
                    empty();
                    first = true;
                    token = strtok(workingBuffer, &newline);
                    while( token != NULL ) {
                        if (first && !txBufferOverflowFlag) Serial.print(F("\""));
                        else Serial.print(F(",\""));
                        Serial.print(token);
                        Serial.print(F("\""));
                        token = strtok(NULL, &newline);
                        first = false;
                    }
                    Serial.println(F("]"));
                    Serial.println(F("DIR_EOF"));
                    free(workingBuffer);
                    delay(200);
                } else{
                    //send the requested path
                    delay(100);
                    workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
                    memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
                    replacechar(param,':',' '); //replace space token used to tx the path
                    empty();
                    print(F("M "));
                    println(param);
                    println(F("DIR"));
                    strcpy(workingBuffer,txBuffer);
                    Serial.print(workingBuffer);
                    memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
                    empty();
                    sd->chdir();
                    Serial.print(F("LS "));
                    replacechar(param,' ',':'); //replace any spaces in the transmitted path to ':'
                    Serial.print(param);
                    replacechar(param,':',' '); // put the spaces back in time for the ls command
                    Serial.print(F(" ["));
                    delay(10);
                    txBufferOverflowFlag = false;
                    sd->ls(this,param,false);
                    memset(workingBuffer,0,SERIAL_WORKING_BUFFER_SIZE);
                    strcpy(workingBuffer,txBuffer);
                    first = true;
                    token = strtok(workingBuffer, &newline);
                    while( token != NULL ) {                        
                        if (first && !txBufferOverflowFlag) Serial.print(F("\""));
                        else Serial.print(F(",\""));
                        Serial.print(token);
                        Serial.print(F("\""));
                        token = strtok(NULL, &newline);
                        first = false;
                        delay(5);
                        while(throttle()){
                            delay(200);
                        }
                    }
                    Serial.println(F("]"));
                    Serial.println(F("DIR_EOF"));
                    empty();
                    free(workingBuffer);
                    delay(250);
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
            } else if (strncmp(cmd, "DISCONNECT",sizeof(cmd)) == 0){
                int dest_port;
                total_read = sscanf(receivedChars, "%s %s %d" , cmd, param,&dest_port);
                if (total_read < 2){
                    Serial.print(F("M DISCONNECT WRONG PARAM COUNT "));
                    Serial.println(receivedChars);
                } else{
                    if (ad->disconnect(param,dest_port)){
                        Serial.printf(F("M DISCONNECTED %s %d\n"),param,dest_port);
                    }else{
                        Serial.printf(F("M FAILED DISCONNECT OF %s %d\n"),param,dest_port);
                    }
                }
            } else if (strncmp(cmd, "AA",sizeof(cmd)) == 0){         //active app message
                if (total_read > 1) am->getActiveApp()->MessageHandler(this,param);
                //Serial.print(F("AA OK "));
                //Serial.println(AppManager::getInstance()->getActiveApp()->name);
                //Serial.flush();
            }else if (strncmp(cmd, "STATS",sizeof(cmd)) == 0){
                am->printStats();
                ad->printStats();
                delay(5);
            }else if (strncmp(cmd, "CQT_CFG",sizeof(cmd)) == 0){ 
                am->sendMessage(this,"AppCQT","CQT_INFO");
            }else if (strncmp(cmd, "GET_DD",sizeof(cmd)) == 0){ 
                am->data->printDictionary(this);
            }else if (strncmp(cmd, "UPDATE_DD",sizeof(cmd)) == 0){
                int32_t val;
                float32_t fval;
                total_read = sscanf(receivedChars, "%s %s %d" , cmd, param,(int*)&val);
                if (total_read < 3){
                    total_read = sscanf(receivedChars, "%s %s %f" , cmd, param,(float32_t*)&fval);
                    if (total_read < 3){
                        Serial.print(F("M WRONG PARAM COUNT"));
                        Serial.println(total_read);
                    } else am->data->update(param,fval); 
                }else if(!am->data->update(param,val)){
                    total_read = sscanf(receivedChars, "%s %s %f" , cmd, param,(float32_t*)&fval);
                    am->data->update(param,fval);
                }
            }else if (strncmp(cmd, "HELO",sizeof(cmd)) == 0){ 
                startLZ4Message();
                println(gWelcomeMessage);
                endLZ4Message();
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
                delayMicroseconds(100000);
                Serial.printf(F("RAM {\"RAM2\":{\"addr\":\"%08X\",\"chunk\":\""),0x20000000);
                strcpy(txBuffer," ");
                //RAM2 always ends at 0x20280000
                for(uint32_t i = 0x20000000; i < 0x20280000; i+=1){
                    if (i == 0x20010000) i = 0x20200000;
                    mp = (char*)i;
                    c = *mp;
                    c = (c & 0xFF);
                    if(i%32==0 && i != 0x20000000){
                        Serial.print(F("\",\"decode\":\""));
                        Serial.print(txBuffer);
                        Serial.println(F("\"}}"));
                        strcpy(txBuffer,"");
                        delay(4);
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
                delay(100);
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
    if(periodicMessagesEnabled){
        if (sincePeriodicStats > SERIAL_AUTO_TRANSMIT_STATS_PERIOD && !throttle()){
            sincePeriodicStats = 0; 
            AppManager::getInstance()->printStats();
            ad->printStats();
        }

        if (sincePeriodicDataDict > SERIAL_AUTO_TRANSMIT_DATA_DICT_PERIOD && !throttle()){
            sincePeriodicDataDict = 0;
            sincePeriodicStats = 0; 
            AppManager::getInstance()->data->printDictionary(this);
        }
    }
    #endif
}; //allways called even if app is not active