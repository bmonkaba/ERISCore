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
#include <Arduino.h>
#include <AppManager.h>

//Wren system interface -- these are standard functions which MUST be provided to the VM
extern "C" {
  int _kill (pid_t pid, int signum){return 0;}
  pid_t _getpid(void){return 1;}
  int _times(void){return 0;}
  int _gettimeofday(struct timeval *tv, struct timezone *tz){return 0;}
}

static void writeFn(WrenVM* vm, const char* text) {
  SerialUSB1.printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
     SerialUSB1.printf("WREN_ERR [%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      SerialUSB1.printf("WREN_ERR [%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      //Serial.printf("M WREN_ERR [Runtime Error] %s\n", msg);
    } break;
  }
}

char * getSourceForModule(const char * name){
  return NULL;
}

static void loadModuleComplete(WrenVM* vm, 
                               const char* module,
                               WrenLoadModuleResult result) 
{
  if(result.source) {
    //for example, if we used malloc to allocate
    //our source string, we use free to release it.
    free((void*)result.source);
  }
}


WrenLoadModuleResult loadModule(WrenVM* vm, const char* name){
  //callback for the VM to request a module be loaded from the filesystem
  SerialUSB1.printf("VM: loadModule %s\n", name);
  WrenLoadModuleResult result = {0};
    result.onComplete = loadModuleComplete;
    result.source = getSourceForModule(name);
  return result;
}


//EXPORTED STATIC FUNCTIONS

void sendMessageCallback(WrenVM* vm){
//callback params can be found in the slots
//return value goes into slot 0
  AppManager* am = AppManager::getInstance();
  wrenSetSlotBool(vm, 0, am->sendMessage(am->getAppByName("AppWren"),wrenGetSlotString(vm, 1),wrenGetSlotString(vm, 2)));
}

void setPositionCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t newOriginX, int16_t newOriginY)
  AppBaseClass* app = am->getAppByName("AppWren");
  app->setPosition(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));

};
void setDimensionCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppBaseClass* app = am->getAppByName("AppWren");
  app->setDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}
void setWidgetPositionCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t newOriginX, int16_t newOriginY)
  AppBaseClass* app = am->getAppByName("AppWren");
  app->setWidgetPosition(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

void setWidgetDimensionCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppBaseClass* app = am->getAppByName("AppWren");
  app->setWidgetDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

void requestPopUpCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->requestPopUp(wrenGetSlotBool(vm, 1));
}
void releasePopUpCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->releasePopUp();
}
void getFocusCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setWidgetDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}
void returnFocusCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //(int16_t new_width, int16_t new_height)
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setWidgetDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

void setPixelCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  //setPixel(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b)
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setPixel(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5));
}



//EXPORTED FUNCTION BINDINGS
WrenForeignMethodFn bindForeignMethod(
    WrenVM* vm,
    const char* module,
    const char* className,
    bool isStatic,
    const char* signature)
{
  //callback for the VM to request a pointer to the c function.
  if (strcmp(module, "main") == 0)
  {
    if (strcmp(className, "App") == 0) //these are static methods... attach them to the class, not an instance
    {
      if (strcmp(signature, "sendMessage(_,_)") == 0){
        return sendMessageCallback;
      }else if (strcmp(signature, "setPosition(_,_)") == 0){
        return setPositionCallback;
      }else if (strcmp(signature, "setDimension(_,_)") == 0){
        return setDimensionCallback;
      }else if (strcmp(signature, "setWidgetPosition(_,_)") == 0){
        return setWidgetPositionCallback;
      }else if (strcmp(signature, "setWidgetDimension(_,_)") == 0){
        return setWidgetDimensionCallback;
      }else if (strcmp(signature, "requestPopUp(_)") == 0){
        return requestPopUpCallback;
      }else if (strcmp(signature, "releasePopUp()") == 0){
        return releasePopUpCallback;
      }else if (strcmp(signature, "getFocus()") == 0){
        return getFocusCallback;
      }else if (strcmp(signature, "returnFocus()") == 0){
        return returnFocusCallback;
      }else if (strcmp(signature, "setPixel(_,_,_,_,_)") == 0){
        return setPixelCallback;
      }
      // Other foreign methods on Math...
    }
    // Other classes in main...
  }
  // Other modules...
  //else return NULL?
  return NULL;
}

//end wren callbacks

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
    config.initialHeapSize = 15534;
    config.minHeapSize = 15534;
    config.heapGrowthPercent = 50;
    config.bindForeignMethodFn = &bindForeignMethod;
    vm = wrenNewVM(&config);
}

void AppWren::MessageHandler(AppBaseClass *sender, const char *message){   
    if(sender->isName("SCI")){
        if(0 == strncmp(message,"WREN_SCRIPT_EXECUTE",strlen("WREN_SCRIPT_EXECUTE"))) {
            Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_COMPILE -> compileOnly = false"));
            compileOnly = false;
            return;
        }else if(0 == strncmp(message,"WREN_SCRIPT_COMPILE",strlen("WREN_SCRIPT_COMPILE"))) {
            Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_EXECUTE -> compileOnly = true"));
            compileOnly = true;
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


 void AppWren::update(){
        if (h_update==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (!isWrenResultOK(wrenCall(vm,h_update))){

            }else{
                if(isPressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                        show_active = false;
                        time_active = 0;
                }
                if(usingImage){
                    if(!surface_cache){                        
                        surface_cache = new Surface(am->fastImgCacheSurfaceP, widget_width, widget_height);
                        if(!surface_cache){ 
                            Serial.println(F("M AppWren::update() VM ERROR: Surface not available"));
                            return;
                        }else Serial.println(F("M AppWren::update() Surface created"));
                    } else{
                        draw->bltMem(am->displaySurfaceP,surface_cache,x,y,AT_NONE);
                    }
                }else{
                    draw->fillRoundRect(x,y,w,h/2+3,3,am->data->read("UI_BUTTON_FILL_COLOR"));
                    draw->fillRoundRect(x,y+h/2,w,h/2,3,am->data->read("UI_BUTTON_SHADE_COLOR"));
                }
                if (show_active){
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_ACTIVE_BORDER_COLOR")); 
                } else{
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_INACTIVE_BORDER_COLOR"));
                }
                
                if(!usingImage){
                    draw->setTextColor(am->data->read("UI_BUTTON_TEXT_COLOR"));
                    draw->setCursor(x+(w/2),y+(h/2),true);
                    draw->setFont(Arial_9);
                    //draw->print(text);
                }
            }
        }
        wrenCollectGarbage(vm);
    };    //called only when the app is active


void AppWren::releaseWrenHandles(){
  //release any existing handles
  if (h_slot0!=NULL) wrenReleaseHandle(vm, h_slot0);
  if (h_update!=NULL) wrenReleaseHandle(vm, h_update);
  if (h_updateRT!=NULL) wrenReleaseHandle(vm, h_updateRT);
  if (h_onFocus!=NULL) wrenReleaseHandle(vm, h_onFocus);
  if (h_onFocusLost!=NULL) wrenReleaseHandle(vm, h_onFocusLost);
  if (h_onTouch!=NULL) wrenReleaseHandle(vm, h_onTouch);
  if (h_onTouchDrag!=NULL) wrenReleaseHandle(vm, h_onTouchDrag);
  if (h_onTouchRelease!=NULL) wrenReleaseHandle(vm, h_onTouchRelease);
  if (h_onAnalog1!=NULL) wrenReleaseHandle(vm, h_onAnalog1);
  if (h_onAnalog2!=NULL) wrenReleaseHandle(vm, h_onAnalog2);
  if (h_onAnalog3!=NULL) wrenReleaseHandle(vm, h_onAnalog3);
  if (h_onAnalog4!=NULL) wrenReleaseHandle(vm, h_onAnalog4);
  if (h_messageHandler!=NULL) wrenReleaseHandle(vm, h_messageHandler);
}

void AppWren::getWrenHandles(){
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, "main", "ErisApp", 0); //get the instance to call the methods on
  //get the handles
  h_slot0 = wrenGetSlotHandle(vm, 0);
  h_update = wrenMakeCallHandle(vm, "update()");
  h_updateRT = wrenMakeCallHandle(vm, "updateRT()");
  
  h_onFocus = wrenMakeCallHandle(vm, "onFocus()");
  h_onFocusLost = wrenMakeCallHandle(vm, "onFocusLost()");
  h_onTouch = wrenMakeCallHandle(vm, "onTouch()");
  h_onTouchDrag = wrenMakeCallHandle(vm, "onTouchDrag()");
  h_onTouchRelease = wrenMakeCallHandle(vm, "onTouchRelease()");
  h_onAnalog1 = wrenMakeCallHandle(vm, "onAnalog1()");
  h_onAnalog2 = wrenMakeCallHandle(vm, "onAnalog2()");
  h_onAnalog3 = wrenMakeCallHandle(vm, "onAnalog3()");
  h_onAnalog4 = wrenMakeCallHandle(vm, "onAnalog4()");
  h_messageHandler = wrenMakeCallHandle(vm, "messageHandler()");
}