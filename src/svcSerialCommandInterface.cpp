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
#include <pgmspace.h>
#include "ErisUtils.h"
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
    WREN_SCRIPT_START   indicates the start of a bulk text transfer
    WREN_SCRIPT_END     ends the bulk text transfer
    WREN_SCRIPT_EXECUTE executes the transfered script
    WREN_SCRIPT_COMPILE compiles the transfered script and reports any errors

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


int FASTRUN replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}

uint16_t FASTRUN SvcSerialCommandInterface::checksum(const char *msg){
    uint16_t csum;
    csum=0;
    for(uint16_t i = 0;i<strlen(msg);i++){
        csum += msg[i];
    }
    return csum;
}

/**
 * @brief Calling this function signals the end of a compressed message.\n 
 * The txBuffer contents are lz4 compressed, base64 encoded and finaly sent.\n 
 * The serial message format is "LZ4 [UNCOMPRESSED_SIZE] [BASE64 encoded LZ4 compressed message]" 
 */
void FASTRUN SvcSerialCommandInterface::sendLZ4Message(){
    uint16_t compressed_len,uncompressed_len;
#ifdef USE_EXTMEM
    //do nothing - one time malloc in the constructor
#else
    char workingBuffer[SERIAL_WORKING_BUFFER_SIZE];
#endif
    memset(p_working_buffer,0,SERIAL_WORKING_BUFFER_SIZE);
    uncompressed_len = index_tx_buffer;
    compressed_len = LZ4_compress_fast(tx_Buffer,p_working_buffer,index_tx_buffer,SERIAL_WORKING_BUFFER_SIZE,10);
    empty();
    encode_base64((unsigned char *)p_working_buffer, compressed_len, (unsigned char *)tx_Buffer);
    //while(throttle()){
    //    delay(5);
    //};
    Serial.print(" ");
    Serial.print(uncompressed_len);
    Serial.print(" ");
    Serial.println(tx_Buffer);
    //empty();
    //while(throttle()){
    //    delay(50);
    //};

#ifdef USE_EXTMEM
    //do nothing - one time malloc in the constructor
#else
    //do nothing - local var
#endif
}

/**
 * @brief immediately transmit then clear the txBuffer
 * 
 */
void FASTRUN SvcSerialCommandInterface::send(){
    while(throttle()){
        delay(50);
    }
    if (strlen(tx_Buffer) > 0 ) Serial.print(tx_Buffer);
    memset(tx_Buffer,0,SERIAL_OUTPUT_BUFFER_SIZE);
    index_tx_buffer = 0;
    tx_buffer_overflow_flag = false;
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
bool FASTRUN SvcSerialCommandInterface::throttle(){
    uint16_t avail;
    avail = Serial.availableForWrite();

    if(avail < SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD){
        uint16_t delta_avail;
        uint16_t msec_delay_counter = 0;
        //serial tx buffer not available
        do{
            delay(0.5);
            msec_delay_counter += 2;
            delta_avail = Serial.availableForWrite();
            if (delta_avail > SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD) break;
        } while (msec_delay_counter < SERIAL_THROTTLE_CHECK_CONNECTION_MAX_DELAY);
        
        if(avail == delta_avail){
            return true;
        } else{
            //Serial.println(F("M WRN throttling"));//use this for debug only!
            am->data->increment("SERIAL_THROTTLE_EVENTS");
        }
    }
    if(avail < SERIAL_THROTTLE_BUFFER_THRESHOLD) return true;
    return false;
}

void FASTRUN SvcSerialCommandInterface::txOverflowHandler(){
    char* s;
    s = strrchr(tx_Buffer,'\r');//find last new line
    s[0] = 0;
    token = strtok(tx_Buffer, &newline);
    while( token != NULL ) {     
        while(throttle()){
            delay(50);
        }                
        if (tx_buffer_overflow_flag==false){
            //txBufferOverflowFlag=true;
            Serial.print(F("\""));
        }else{
            Serial.print(F(",\""));
        }
        //replaceAll(token,'\r',0);
        Serial.print(token);
        Serial.print(F("\""));
        token = strtok(NULL, &newline);
    }
    //close the tag
    Serial.println("]");
    //start the next part of the multipart message
    Serial.print(&multipart_header[0]);
    s = strrchr(tx_Buffer,'\r');//find last new line
    //s = strrchr(s,'\r');//find last new line
    s+=2;
    Serial.printf("VM %s %s\n",&multipart_header[0],s);
    //Serial.flush();
    index_tx_buffer = strlen(s);
    safer_strncpy(tx_Buffer,s,sizeof(tx_Buffer));
    //Serial.printf("VM remainder %s",txBuffer);
}


void FASTRUN SvcSerialCommandInterface::streamReceiveHandler(){
    Serial.println(F("M SvcSerialCommandInterface::streamReceiveHandler isCapturingBulkData"));
    char* f;
    do{
        do{
            p_capture_buffer[index_capture_buffer] = Serial.read();
            if(p_capture_buffer[index_capture_buffer] != 0xFF) index_capture_buffer++;
        }while (Serial.available() && index_capture_buffer < SERIAL_RX_CAPTURE_BUFFER_SIZE-1);//in case were asking for the streaming data faster than it's being sent
        if(index_capture_buffer == SERIAL_RX_CAPTURE_BUFFER_SIZE-1){
            Serial.println(F("M SvcSerialCommandInterface::streamReceiveHandler Wren Script ERROR: Input buffer full"));
            Serial.flush();
            is_capturing_bulk_data = false;
            index_capture_buffer = 0;
        }
        f = strstr(p_capture_buffer,"WREN_SCRIPT_END");
        if (f > 0){
            Serial.println(F("M SvcSerialCommandInterface::streamReceiveHandler Wren Script Received"));
            memset(f,0,strlen("WREN_SCRIPT_END"));//remove the EOF marker
            //stream complete
            is_capturing_bulk_data = false;
        }
    }while(is_capturing_bulk_data);
}


void FASTRUN SvcSerialCommandInterface::streamTransmitHandler(){
    char bufferChr; //only need one char - 512 size is due to a potential bug in the teensy library identified by covintry static analysis
    char hexBuffer[8];
    uint16_t payload_len;
    uint16_t compressed_len,uncompressed_len;
    if(Serial.availableForWrite() < 6000) return;
    p_SD->chdir(stream_path);
    if (p_SD->exists(stream_file)){
        payload_len = 0;
        if(!file.open(stream_file, O_READ)){ 
            Serial.println(F("M GET_ERR FILE OPEN ERROR"));
            is_streaming_file = false;
            is_periodic_messages_enabled = true;
            stream_pos = 0;
            return;
        }
        startLZ4Message();
        print("FS ");
        file.seek(stream_pos);
        //send the next chunked message until eof
        uint64_t i;
        i = file.available();
        if(i>0){
            //if i is zero then last chunk was eof
            if(i > SERIAL_FILESTREAM_PAYLOAD_SIZE) i = SERIAL_FILESTREAM_PAYLOAD_SIZE;
            for(;i > 0; i--){
                payload_len += 1;
                if (file.read(&bufferChr,1) < 0){
                    Serial.println(F("\nM GET_ERR FILE READ ERROR"));
                    is_streaming_file = false;
                    is_periodic_messages_enabled = true;
                    stream_pos = 0;
                    file.close();
                    return;
                }
                sprintf(hexBuffer,"%02X,",(unsigned int)bufferChr);
                print(hexBuffer);
            }
            if(index_tx_buffer > 0) tx_Buffer[--index_tx_buffer] = '\0'; //remove last comma
            stream_pos = file.position();
            println("");
        }
        file.close();
        if (payload_len == 0){
            //no data to tx; @ eof
            sendLZ4Message();
            Serial.println("FS_END");
            is_streaming_file = false;
            is_periodic_messages_enabled = true;
            stream_pos = 0;
            delayMicroseconds(3000);
            return;
        }
        else if (payload_len < SERIAL_FILESTREAM_PAYLOAD_SIZE) {
            sendLZ4Message();
            Serial.println("FS_END");
            empty();
            is_streaming_file = false;
            is_periodic_messages_enabled = true;
            stream_pos = 0;
            delayMicroseconds(3000);
            return;
        } else{
            //send file chunk
            sendLZ4Message();
            return;
        }             
    } 
    else{
        Serial.print(F("M GET_ERR FILE NOT FOUND "));
        Serial.println(stream_file);
        is_streaming_file = false;
        stream_pos = 0;
        return;
    }
};


void FLASHMEM SvcSerialCommandInterface::messageHandler_GET_RAM1(){
    char* mp = 0;
    char c;
#ifdef USE_EXTMEM
    char tmp[1000];
    //tmp = (char*)extmem_malloc(1000);
#else
    char* tmp;
    tmp = (char*)malloc(1000);
#endif
    while(throttle()){delay(100);}
    if(!requestStartLZ4Message()) return;
    strcpy(tmp,"");
    printf(F("RAM {\"RAM1\":{\"addr\":\"%08X\",\"chunk\":\""),0x000000);
    
    for(uint32_t i = 0x00070000; i < 0x0007F000; i+=1){
        //if (i == 0x20010000) i = 0x20200000;
        mp = (char*)i;
        c = *mp;
        c = (c & 0xFF);
        if(i%32==0 && i != 0x00070000){
            print(F("\",\"decode\":\""));
            print(tmp);
            println(F("\"}}"));
            sendLZ4Message();
            strcpy(tmp,"");
            while(throttle()){delay(150);}
            if(i%1024==0){
                float32_t pct;
                pct = 100.0 * ((float)(i-0x00070000)/(float)(0x0007F000));
                Serial.printf(F("CLS\nM GET_RAM1 %08X %.0f pct "),i,pct);
                for(uint16_t div = 0; div < (uint16_t)pct; div += 5){
                    Serial.printf("*");
                }
                Serial.println("");
            }
            startLZ4Message();
            printf(F("RAM {\"RAM1\":{\"addr\":\"%08X\",\"chunk\":\""),i);
        }

        printf("%02X ",(uint8_t)c);
        const char* escape = "\\";
        if (c=='"'){                        
            strncat(tmp, escape, 1);
            strncat(tmp, &c, 1);
        }else if (c=='\''){
            strncat(tmp, escape, 1);
            strncat(tmp, &c, 1);
        }else if (isprint((int)c)){ 
            strncat(tmp, &c, 1);
        }else if (iscntrl((int)c)){
            c = '.';
            strncat(tmp, &c, 1);
        }else{
            c = '?';
            strncat(tmp, &c, 1);
        }
    }
    println("\n");
    println(F("RAM END"));
    sendLZ4Message();
#ifdef USE_EXTMEM
    //do nothing - local var
#else       
    free(tmp);
#endif
}

void FLASHMEM SvcSerialCommandInterface::messageHandler_GET_RAM2(){
    char* mp = 0;
    char c;
#ifdef USE_EXTMEM
    char tmp[1000];
    //tmp = (char*)extmem_malloc(1000);
#else
    char* tmp;
    tmp = (char*)malloc(1000);
#endif
    while(throttle()){delay(100);}
    if(!requestStartLZ4Message()) return;
    strcpy(tmp,"");
    printf(F("RAM {\"RAM2\":{\"addr\":\"%08X\",\"chunk\":\""),0x20000000);
    //RAM2 always ends at 0x20280000
    for(uint32_t i = 0x20000000; i < 0x20280000; i+=1){
        if (i == 0x20010000) i = 0x20200000;
        mp = (char*)i;
        c = *mp;
        c = (c & 0xFF);
        if(i%32==0 && i != 0x20000000){
            print(F("\",\"decode\":\""));
            print(tmp);
            println(F("\"}}"));
            sendLZ4Message();
            strcpy(tmp,"");
            while(throttle()){delay(50);}
            if(i%1024==0){
                float32_t pct;
                pct = 100.0 * ((float)(i-0x20000000)/(float)(0x20280000-0x20000000));
                Serial.printf(F("CLS\nM GET_RAM2 %08X %.0f pct "),i,pct);
                for(uint16_t div = 0; div < (uint16_t)pct; div += 5){
                    Serial.printf("*");
                }
                Serial.println("");
            }
            startLZ4Message();
            printf(F("RAM {\"RAM2\":{\"addr\":\"%08X\",\"chunk\":\""),i);
        }

        printf("%02X ",(uint8_t)c);
        const char* escape = "\\";
        if (c=='"'){                        
            strncat(tmp, escape, 1);
            strncat(tmp, &c, 1);
        }else if (c=='\''){
            strncat(tmp, escape, 1);
            strncat(tmp, &c, 1);
        }else if (isprint((int)c)){ 
            strncat(tmp, &c, 1);
        }else if (iscntrl((int)c)){
            c = '.';
            strncat(tmp, &c, 1);
        }else{
            c = '?';
            strncat(tmp, &c, 1);
        }
    }
    println("\n");
    println(F("RAM END"));
    sendLZ4Message();
#ifdef USE_EXTMEM
    //do nothing - local var
#else       
    free(tmp);
#endif
}

void FLASHMEM SvcSerialCommandInterface::messageHandler_UPDATE_DD(){
    int total_read;
    char cmd[128], param[128],param2[128];
    int32_t val;
    float32_t fval;
    total_read = sscanf(received_chars, "%127s %127s %d" , cmd, param,(int*)&val);
    if (total_read < 3){
        total_read = sscanf(received_chars, "%127s %127s %f" , cmd, param,(float32_t*)&fval);
        if (total_read < 3){
            Serial.print(F("M SvcSerialCommandInterface::messageHandler_UPDATE_DD ERROR WRONG PARAM COUNT"));
            Serial.println(total_read);
        } else am->data->update(param,fval); 
    }else if(!am->data->update(param,val)){
        total_read = sscanf(received_chars, "%127s %127s %f" , cmd, param,(float32_t*)&fval);
        am->data->update(param,fval);
    }
}


void FLASHMEM SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_SAVE(){
    if(p_capture_buffer != NULL){
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_SAVE: script save request"));
        am->sendMessage(this,"AppWren",received_chars);//"WREN_SCRIPT_SAVE [modulename]");
        am->sendMessage(this,"AppWren",p_capture_buffer);
    #ifndef USE_EXTMEM
        captureBuffer = (char*)realloc(captureBuffer,0);
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_SAVE: captureBuffer released"));
    #endif
    } else Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_SAVE: captureBuffer is NULL"));
}

void FLASHMEM SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_EXECUTE(){
    if(p_capture_buffer != NULL){
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_EXECUTE: script execute request"));
        am->sendMessage(this,"AppWren","WREN_SCRIPT_EXECUTE");
        am->sendMessage(this,"AppWren",p_capture_buffer);
#ifndef USE_EXTMEM
        captureBuffer = (char*)realloc(captureBuffer,0);
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_EXECUTE: captureBuffer released"));
#endif
    } else Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_EXECUTE: captureBuffer is NULL"));
}


void FLASHMEM SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_COMPILE(){
    if(p_capture_buffer != NULL){
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_COMPILE: script compile request"));
        am->sendMessage(this,"AppWren","WREN_SCRIPT_COMPILE");
        am->sendMessage(this,"AppWren",p_capture_buffer);
#ifndef USE_EXTMEM
        captureBuffer = (char*)realloc(captureBuffer,0);
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_COMPILE: captureBuffer released"));
#endif
    } else Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_COMPILE: captureBuffer is NULL"));
}

void FLASHMEM SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_START(){
    is_capturing_bulk_data = true;
#ifndef USE_EXTMEM
    char* tmp;
    tmp = (char*)realloc(captureBuffer,SERIAL_RX_CAPTURE_BUFFER_SIZE);
    if(tmp==NULL){
        //realloc failed; release the pointer and try again
        free(captureBuffer);
        captureBuffer = (char*)malloc(SERIAL_RX_CAPTURE_BUFFER_SIZE);
    } else captureBuffer = tmp;
#endif
    if(p_capture_buffer!=NULL){
        memset(p_capture_buffer,0,SERIAL_RX_CAPTURE_BUFFER_SIZE);
        index_capture_buffer = 0;
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_START"));
    } else{
        is_capturing_bulk_data = false;
        Serial.println(F("M SvcSerialCommandInterface::messageHandler_WREN_SCRIPT_START ERROR: realloc(SERIAL_RX_CAPTURE_BUFFER_SIZE) failed"));
        Serial.flush();
    }
}

void FLASHMEM SvcSerialCommandInterface::messageHandler_GET(){
    int total_read;
    char cmd[128], param[128],param2[128];
    total_read = sscanf(received_chars, "%127s %127s %127s" , cmd, param,param2);
    if (total_read < 3){
        Serial.print(F("M GET_ERR WRONG PARAM COUNT"));
        Serial.println(param);
    } else{
        while(throttle()){ delay(1);}
        //file streaming request ok
        //init the transfer
        Serial.print(F("M GET_OK ")); 
        Serial.print(param);
        Serial.print(" ");
        Serial.println(param2);
        strcpy(stream_path,param);
        strcpy(stream_file,param2);
        Serial.println(F("FS_START"));
        is_streaming_file = true;
        is_periodic_messages_enabled = false;
        stream_pos = 0;
    }
}
void FLASHMEM SvcSerialCommandInterface::messageHandler_LS(){
    bool first;
    int total_read;
    char cmd[128], param[128],param2[128];
    total_read = sscanf(received_chars, "%127s %127s %127s" , cmd, param,param2);
    if (total_read < 2){
        //send the root directory
        //delay(100);
        empty();
        println(F("DIR"));
        print(F("LS . ["));
#ifdef USE_EXTMEM
        //do nothing - one time malloc in the constructor
#else
        workingBuffer = (char*)malloc(SERIAL_WORKING_BUFFER_SIZE);
#endif
        strcpy(p_working_buffer,tx_Buffer);
        empty();
        Serial.print(p_working_buffer);
        sd->chdir();
        tx_buffer_overflow_flag = false;
        sd->ls(this);
        memset(p_working_buffer,0,SERIAL_WORKING_BUFFER_SIZE);
        strcpy(p_working_buffer,tx_Buffer);
        empty();
        first = true;
        token = strtok(p_working_buffer, &newline);
        while( token != NULL ) {
            if (first && !tx_buffer_overflow_flag) Serial.print(F("\""));
            else Serial.print(F(",\""));
            Serial.print(token);
            Serial.print(F("\""));
            token = strtok(NULL, &newline);
            first = false;
        }
        Serial.println(F("]"));
        Serial.println(F("DIR_EOF"));
#ifdef USE_EXTMEM                    
        //do nothing - one time malloc in the constructor
#else
        free(workingBuffer);
#endif
        //delay(200);
    } else{
        //send the requested path
        replacechar(param,':',' '); //replace space token used to tx the path
        empty();
        print(F("M "));
        println(param);
        println(F("DIR"));
        send();
        sd->chdir();
        replacechar(param,' ',':'); //replace any spaces in the transmitted path to ':'
        sprintf(multipart_header, "LS %s [", param);
        Serial.print(multipart_header);
        replacechar(param,':',' '); // put the spaces back in time for the ls command
        sd->ls(this,param,false);
        first = true;
        token = strtok(tx_Buffer, &newline);
        while( token != NULL && index_tx_buffer > 1) {                        
            if (first && !tx_buffer_overflow_flag) Serial.print(F("\""));
            else Serial.print(F(",\""));
            replacechar(token,'\r',0);
            Serial.print(token);
            Serial.print(F("\""));
            token = strtok(NULL, &newline);
            first = false;
            while(throttle()){
                delay(50);
            }
        }
        Serial.println(F("]"));
        Serial.println(F("DIR_EOF"));
    }
}

void FASTRUN SvcSerialCommandInterface::update(){ 
    char endMarker = '\n';
    boolean newRxMsg = false;
    char bufferChr;
    am->requestArmSetClock(CPU_BOOST_MAX_FREQ);
    
    
    if (is_streaming_file){ //streaming file handler
        streamTransmitHandler();
    }
    
    if(is_capturing_bulk_data){ //bulk data state handler
        streamReceiveHandler();
    }else { //check for incomming data
        //This is the incomming message handler - it collects the message one byte at a time until 
        //the end of message marker. If a full message is received the newRxMsg flag is set true.
        //if a partial message is received the flag remains false.
        //if the receiving buffer reaches an overflow condition, reset the buffer and clear in remaining serial input
        if (Serial.available() > 0 && false == newRxMsg ){
            do{
                bufferChr = Serial.read();
                received_chars[index_rx_buffer++] = bufferChr;

                if(index_rx_buffer >= (SERIAL_RX_BUFFER_SIZE-2)){
                    //input overflow - clear serial input and reset the index
                    index_rx_buffer = 0;
                    Serial.clear();
                    Serial.clearReadError();
                    am->requestArmSetClock(CPU_LOW_POWER_MODE_FREQ);
                    return;
                }

                if (bufferChr == endMarker){
                    received_chars[--index_rx_buffer] = '\0'; //remove the end marker and null terminate the string
                    index_rx_buffer = 0; //reset the input write index
                    newRxMsg = true;
                }
            } while (Serial.available() > 0 && false == newRxMsg);
        }
        if (newRxMsg){
            //Once a new message is received, first the command is parsed out from the message
            // this first split into cmd and param is for the purpose of identifying the requested command
            // each individual message handler is then responsible for parsing out thier own parameters if required.
            char cmd[128], param[128],param2[128];
            int total_read;
            total_read = sscanf(received_chars, "%127s %127s" , cmd, param);
            while(throttle()){delay(1);}; //let the transmitt buffer clear out
            if (strncmp(cmd, "LS",sizeof(cmd)) == 0){
                messageHandler_LS();
            }else if (strncmp(cmd, "GET",sizeof(cmd)) == 0){
                messageHandler_GET();
            }else if (strncmp(cmd, "ACON",sizeof(cmd)) == 0){ //audio connections
                uint16_t i;  
                char csBuffer[128];
                i = 0;
                Serial.println(F("M ACON START"));
                
                while(ad->getConnectionString(i,csBuffer)){
                    i += 1;
                    Serial.print("M ");
                    Serial.println(csBuffer);
                }
                Serial.println(F("M ACON END"));

            }else if (strncmp(cmd, "CONNECT",sizeof(cmd)) == 0){
                int source_port;
                int dest_port;
                
                total_read = sscanf(received_chars, "%127s %127s %d %127s %d" , cmd, param,&source_port,param2,&dest_port);
                if (total_read < 3){
                    Serial.print(F("M CONNECT WRONG PARAM COUNT "));
                    Serial.println(received_chars);
                } else{
                    ad->connect(param,source_port,param2,dest_port);
                }
            }else if (strncmp(cmd, "DISCONNECT",sizeof(cmd)) == 0){
                int dest_port;
                total_read = sscanf(received_chars, "%127s %127s %d" , cmd, param,&dest_port);
                if (total_read < 2){
                    Serial.print(F("M DISCONNECT WRONG PARAM COUNT "));
                    Serial.println(received_chars);
                } else{
                    if (ad->disconnect(param,dest_port)){
                        Serial.printf(F("M DISCONNECTED %s %d\n"),param,dest_port);
                    }else{
                        Serial.printf(F("M FAILED DISCONNECT OF %s %d\n"),param,dest_port);
                    }
                }
            }else if (strncmp(cmd, "AA",sizeof(cmd)) == 0){         //active app message
                if (total_read > 1) am->getActiveApp()->messageHandler(this,param);
            }else if (strncmp(cmd, "APC",sizeof(cmd)) == 0){
                //forward the message to the SvcErisAudioParameterController
                am->getAppByName("APC")->messageHandler(this,received_chars + 4);
            }else if (strncmp(cmd, "STATS",sizeof(cmd)) == 0){
                ad->printStats();
                while(throttle()){delay(1);}
                am->printStats();
            }else if (strncmp(cmd, "CQT_CFG",sizeof(cmd)) == 0){ 
                am->sendMessage(this,"AppCQT","CQT_INFO");
            }else if (strncmp(cmd, "GET_DD",sizeof(cmd)) == 0){ 
                am->data->printDictionary(this);
            }else if (strncmp(cmd, "GET_WREN_SCRIPT",sizeof(cmd)) == 0){ 
                while(throttle()){delay(100);}
                if(requestStartLZ4Message()){
                    println("#WREN_START!"); 
                    println(g_wrenScript);
                    println("#WREN_EOF!");
                    sendLZ4Message();
                }
            }else if (strncmp(cmd, "WREN_SCRIPT_START",sizeof(cmd)) == 0){
                messageHandler_WREN_SCRIPT_START();
            }else if (strncmp(cmd, "WREN_SCRIPT_COMPILE",sizeof(cmd)) == 0){
                messageHandler_WREN_SCRIPT_COMPILE();  
            }else if (strncmp(cmd, "WREN_SCRIPT_EXECUTE",sizeof(cmd)) == 0){
                messageHandler_WREN_SCRIPT_EXECUTE();
            }else if (strncmp(cmd, "WREN_SCRIPT_SAVE",sizeof(cmd)) == 0){
                messageHandler_WREN_SCRIPT_SAVE();
            }else if (strncmp(cmd, "UPDATE_DD",sizeof(cmd)) == 0){
                messageHandler_UPDATE_DD();
            }else if (strncmp(cmd, "HELO",sizeof(cmd)) == 0){ 
                if(requestStartLZ4Message()){
                    println(gWelcomeMessage);
                    sendLZ4Message();
                }
            }else if (strncmp(cmd, "GET_RAM",sizeof(cmd)) == 0){ 
                char* mp;
                long num;
                num = strtol(param,NULL,10);
                mp = (char*)num;
                Serial.printf("M GET_RAM {\"addr\":\"%08X\",\"data\":\"%02X\"}\n",num,(uint8_t)*mp);
            }else if (strncmp(cmd, "GET_RAM2",sizeof(cmd)) == 0){ 
                messageHandler_GET_RAM2();
            }else if (strncmp(cmd, "GET_RAM1",sizeof(cmd)) == 0){ 
                messageHandler_GET_RAM1();
            }else if (strncmp(cmd, "AUDIO_NO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioNoInterrupts();
            }else if (strncmp(cmd, "AUDIO_INTERRUPTS",sizeof(cmd)) == 0){
                AudioInterrupts();
            }
            //END
            newRxMsg = false;
        }
    }
    et_since_poll = 0;
    
    //tx periodic messages
    #ifdef SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
    if(is_periodic_messages_enabled && !is_streaming_file){
        if (et_since_periodic_stats_tx > SERIAL_AUTO_TRANSMIT_STATS_PERIOD){
            et_since_periodic_stats_tx = 0; 
            AppManager::getInstance()->printStats();
            ad->printStats();            
        }else if (et_since_periodic_data_dict_tx > SERIAL_AUTO_TRANSMIT_DATA_DICT_PERIOD){
                et_since_periodic_data_dict_tx = 0;
                et_since_periodic_stats_tx = 0; 
                AppManager::getInstance()->data->printDictionary(this);
        }
    }
    #endif
    am->requestArmSetClock(CPU_BASE_FREQ);
}; //allways called even if app is not active