/**
 * @file AppWren.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppWren.h"
#include "globaldefs.h"

//Wren system interface -- these are standard functions which MUST be provided to the VM
extern "C" {
  int _kill (pid_t pid, int signum){return 0;}
  pid_t _getpid(void){return 1;}
  int _times(void){return 0;}
  int _gettimeofday(struct timeval *tv, struct timezone *tz){return 0;}
}

static void writeFn(WrenVM* vm, const char* text) {Serial.printf("%s", text);}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      Serial.printf("M WREN_ERR [%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      Serial.printf("M WREN_ERR [%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      //Serial.printf("M WREN_ERR [Runtime Error] %s\n", msg);
    } break;
  }
}

void AppWren::vmConstructor(){
    Serial.println(F("M AppWren: Starting the VM"));
    startVM();
    loadScript(g_wrenScript);
    getWrenHandles();
    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, h_slot0);//App
    isWrenResultOK(wrenCall(vm,h_updateRT));
}

void AppWren::startVM(){
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
    config.initialHeapSize = 16000;
    config.minHeapSize = 16000;
    config.heapGrowthPercent = 50;
    vm = wrenNewVM(&config);
}

void AppWren::MessageHandler(AppBaseClass *sender, const char *message){   
    if(sender->isName("SCI")){
        if(strcmp(message,"WREN_SCRIPT_COMPILE")) {
            Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_COMPILE -> compileOnly = true"));
            compileOnly = true;
            return;
        }else if(strcmp(message,"WREN_SCRIPT_EXECUTE")) {
            Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_EXECUTE -> compileOnly = false"));
            compileOnly = false;
            return;
        }else{ //if the message payload is not a command then assume its the data
            if(compileOnly){
              Serial.println(F("M AppWren::MessageHandler: Compiling the received script"));
              return;
            }else{ //execute the script
                Serial.println(F("M AppWren::MessageHandler: Restarting the VM"));
                restartVM();
                Serial.println(F("M AppWren::MessageHandler: Loading the received script"));
                loadScript(message);
                Serial.println(F("M AppWren::MessageHandler: cache the wren handles"));
                getWrenHandles();
                wrenSetSlotHandle(vm, 0, h_slot0);//App
                Serial.println(F("M AppWren::MessageHandler: request complete"));
            }
        }
    }
}