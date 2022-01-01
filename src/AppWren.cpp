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
#include <boarddefs.h>


uint16_t FASTRUN wrenFastRam[WREN_FRAME_BUFFER_SIZE]__attribute__ ((aligned (16)));
char DMAMEM wrenFastVMRam[1]__attribute__ ((aligned (16)));

/**
 * @brief Wren system interface 
 * 
 */
//-- these are standard functions which MUST be provided to the VM
extern "C" {
  int _kill (pid_t pid, int signum){return 0;}
  pid_t _getpid(void){return 1;}
  int _times(void){return 0;}
  int _gettimeofday(struct timeval *tv, struct timezone *tz){return 0;}
}

/**
 * @brief write function system callback for the Wren VM. Stdout/Stderr is SerialUSB1
 * 
 * @param vm 
 * @param text 
 */
static void writeFn(WrenVM* vm, const char* text) {
  SerialUSB1.printf("VM %s", text);
}

/**
 * @brief error function system callback for the Wren VM. Stdout/Stderr is SerialUSB1
 * 
 * @param vm 
 * @param errorType 
 * @param module 
 * @param line 
 * @param msg 
 */
void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      SerialUSB1.printf("VM WREN_ERR [%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      SerialUSB1.printf("VM WREN_ERR [%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      //Serial.printf("VM M WREN_ERR [Runtime Error] %s\n", msg);
    } break;
  }
}

/**
 * @brief Get the Source For Module object requested by the Wren VM.
 * 
 * @param name 
 * @return const char* 
 */
const char * getSourceForModule(const char * name){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  return app->loadModuleSource(name);
}

/**
 * @brief callback function for the VM to indicate to the system that it's ok to release any memory allocated during the module load request
 * 
 * @param vm 
 * @param module 
 * @param result 
 */
static void loadModuleComplete(WrenVM* vm, 
                               const char* module,
                               WrenLoadModuleResult result) 
{
  if(result.source) {
    //for example, if we used malloc to allocate
    //our source string, we use free to release it.
    //free((void*)result.source);
    AppManager* am = AppManager::getInstance();
    AppWren* app = (AppWren*)am->getAppByName("AppWren");
    return app->freeModuleSource();
  }
}


/**
 * @brief callback for the VM to request a module be loaded from the filesystem
 * 
 * @param vm 
 * @param name 
 * @return WrenLoadModuleResult 
 */
WrenLoadModuleResult loadModule(WrenVM* vm, const char* name){
  //callback for the VM to request a module be loaded from the filesystem
  Serial.printf("M VM: loadModule %s\n", name);
  WrenLoadModuleResult result = {0};
    result.onComplete = loadModuleComplete;
    result.source = getSourceForModule(name);
  return result;
}


static void* wrenFastMemoryAllocator(void* ptr, size_t newSize, void* _)
{
  if (newSize == 0) return NULL;
  if(64000 < newSize) return NULL;
  //realloc(ptr, newSize);
  //ram is fixed in this integration. 
  //therfore all memory allocation requests will return the fast ram vm buffer
  //unless the request size is too large. 
  return (void*)&wrenFastVMRam[0];
}



//EXPORTED STATIC FUNCTIONS

/**
 * @brief AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void sendMessageCallback(WrenVM* vm){
//callback params can be found in the slots
//return value goes into slot 0
  AppManager* am = AppManager::getInstance();
  wrenSetSlotBool(vm, 0, am->sendMessage(am->getAppByName("AppWren"),wrenGetSlotString(vm, 1),wrenGetSlotString(vm, 2)));
}

/**
 * @brief AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void setPositionCallback(WrenVM* vm){
  //(int16_t newOriginX, int16_t newOriginY)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setPosition(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
};

/**
 * @brief Set the Dimension Callback object. AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void setDimensionCallback(WrenVM* vm){
  //(int16_t new_width, int16_t new_height)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

/**
 * @brief Set the Widget Position Callback object. AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void setWidgetPositionCallback(WrenVM* vm){
  //(int16_t newOriginX, int16_t newOriginY)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setWidgetPosition(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

/**
 * @brief Set the Widget Dimension Callback object. AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void setWidgetDimensionCallback(WrenVM* vm){
  //(int16_t new_width, int16_t new_height)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setWidgetDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}
/**
 * @brief AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void requestPopUpCallback(WrenVM* vm){
  //(void)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->requestPopUp(wrenGetSlotBool(vm, 1));
}
/**
 * @brief AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void releasePopUpCallback(WrenVM* vm){
  //(void)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->releasePopUp();
}
/**
 * @brief Get the Focus Callback object.AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void getFocusCallback(WrenVM* vm){
  //(void)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->getFocus();
}
/**
 * @brief AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void returnFocusCallback(WrenVM* vm){
  //(void)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setWidgetDimension(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2));
}

extern "C" uint32_t set_arm_clock(uint32_t frequency);

/**
 * @brief Set the CPU Clock Speed Callback object
 * 
 * @param vm 
 */
void setClockSpeedCallback(WrenVM* vm){
  //(uint16_t cpu_speed)
  set_arm_clock(wrenGetSlotDouble(vm, 1));
}



/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorConnectCallback(WrenVM* vm){
  //(char* source, uint16_t source_port,char* dest, uint16_t dest_port)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  const char* s = wrenGetSlotString(vm, 1);
  bool b = app->getAudioDirector()->connect((char*)wrenGetSlotString(vm, 1),wrenGetSlotDouble(vm, 2),(char*)wrenGetSlotString(vm, 3),wrenGetSlotDouble(vm, 4));
  wrenSetSlotBool(vm, 0,b);
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorDisconnectCallback(WrenVM* vm){
  //(char* dest, uint16_t dest_port)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  const char* s = wrenGetSlotString(vm, 1);
  bool b = app->getAudioDirector()->disconnect((char*)wrenGetSlotString(vm, 1),wrenGetSlotDouble(vm, 2));
  wrenSetSlotBool(vm, 0,b);
}
/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorDisconnectAllCallback(WrenVM* vm){
  //()
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  const char* s = wrenGetSlotString(vm, 1);
  bool b = app->getAudioDirector()->disconnectAll();
  wrenSetSlotBool(vm, 0,b);
}


// DATA DICT CALLBACKS
/**
 * @brief SvcDataDictionary exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void dataDictUpdateCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  bool b = am->data->update(wrenGetSlotString(vm, 1),(int32_t)wrenGetSlotDouble(vm, 2));
  wrenSetSlotBool(vm, 0,b);
}
/**
 * @brief SvcDataDictionary exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void dataDictReadCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  int32_t d = am->data->read(wrenGetSlotString(vm, 1));
  wrenSetSlotDouble(vm, 0,d);
}

/**
 * @brief SvcDataDictionary exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void dataDictUpdateFloatCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  bool b = am->data->update(wrenGetSlotString(vm, 1),(float)wrenGetSlotDouble(vm, 2));
  wrenSetSlotBool(vm, 0,b);
}
/**
 * @brief SvcDataDictionary exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void dataDictReadFloatCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  float32_t f = am->data->readf(wrenGetSlotString(vm, 1));
  wrenSetSlotDouble(vm, 0,f);
}


//Draw Callbacks
/**
 * @brief Set the Text Color Callback object
 * 
 * @param vm 
 */
void setTextColorCallback(WrenVM* vm){
  //(int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->getDraw()->setTextColor(app->getDraw()->color565(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2),wrenGetSlotDouble(vm, 3)));
}
/**
 * @brief Set the Cursor Callback object
 * 
 * @param vm 
 */
void setCursorCallback(WrenVM* vm){
  //(int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->getDraw()->setCursor(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2),false);
}
/**
 * @brief Set the Print Callback object
 * 
 * @param vm 
 */
void printCallback(WrenVM* vm){
  //(char* string)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->getDraw()->print(wrenGetSlotString(vm,1));
}
/**
 * @brief Set the Font Size Callback object
 * 
 * @param vm 
 */
void setFontSizeCallback(WrenVM* vm){
  //TODO
}

void loadImageCallback(WrenVM* vm){
  //(char* path,char* filename,int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->getDraw()->bltSD(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),AT_NONE);
}

/**
 * @brief Set the Pixel Callback object. AppWren exported method which can be called from within a Wren VM instance
 * 
 * @param vm 
 */
void setPixelCallback(WrenVM* vm){
  //(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setPixel(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5));
}

void drawLineCallback(WrenVM* vm){
  //(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->drawLine(wrenGetSlotDouble(vm,1),wrenGetSlotDouble(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5), wrenGetSlotDouble(vm, 6), wrenGetSlotDouble(vm, 7));
}


//EXPORTED FUNCTION BINDINGS
/**
 * @brief callback function for Wren to request an exported function bindings
 * 
 * @param vm 
 * @param module 
 * @param className 
 * @param isStatic 
 * @param signature 
 * @return WrenForeignMethodFn 
 */
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
      }else if (strcmp(signature, "setClockSpeed(_)") == 0){
        return setClockSpeedCallback;
      }
    }else if (strcmp(className, "Data") == 0){ //these are static methods... attach them to the class, not an instance
      if (strcmp(signature, "update(_,_)") == 0){
        return dataDictUpdateCallback;
      }else if (strcmp(signature, "read(_)") == 0){
        return dataDictReadCallback;
      }
      if (strcmp(signature, "updatef(_,_)") == 0){
        return dataDictUpdateFloatCallback;
      }else if (strcmp(signature, "readf(_)") == 0){
        return dataDictReadFloatCallback;
      }
    }else if (strcmp(className, "AudioDirector") == 0){ //these are static methods... attach them to the class, not an instance
      if (strcmp(signature, "connect(_,_,_,_)") == 0){
        return audioDirectorConnectCallback;
      } else if (strcmp(signature, "disconnect(_,_)") == 0){
        return audioDirectorDisconnectCallback;
      }else if (strcmp(signature, "disconnectAll()") == 0){
        return audioDirectorDisconnectAllCallback;
      }
    }else if (strcmp(className, "Draw") == 0){ //these are static methods... attach them to the class, not an instance
      if (strcmp(signature, "setPixel(_,_,_,_,_)") == 0){
        return setPixelCallback;
      }else if (strcmp(signature, "setTextColor(_,_,_)") == 0){
        return setTextColorCallback;
      }else if (strcmp(signature, "setCursor(_,_)") == 0){
        return setCursorCallback;
      }else if (strcmp(signature, "print(_)") == 0){
        return printCallback;
      }else if (strcmp(signature, "setFontSize(_)") == 0){
        return setFontSizeCallback;
      }else if (strcmp(signature, "loadImage(_,_,_,_)") == 0){
        return loadImageCallback;
      }else if (strcmp(signature, "line(_,_,_,_,_,_,_)") == 0){
        return drawLineCallback;
      }
    }
    // Other classes in main...
  }
  // Other modules...
  //else return NULL?
  return NULL;
}

//end wren callbacks

/**
 * @brief start, load and configure the handles of the VM
 * 
 */
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
    Serial.println(F("CLS\nM AppWren::startVM()"));
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
    config.initialHeapSize = WREN_VM_HEAP_SIZE;
    config.minHeapSize = WREN_VM_HEAP_SIZE;
    config.heapGrowthPercent = 1;
    config.bindForeignMethodFn = &bindForeignMethod;
    config.loadModuleFn = &loadModule;
    //config.reallocateFn = &wrenFastMemoryAllocator; 
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
        }else if(0 == strncmp(message,"WREN_SCRIPT_SAVE",strlen("WREN_SCRIPT_SAVE"))){
            char cmd[128], file_name[128];
            int total_read;
            total_read = sscanf(message, "%s %s\n" , cmd, save_module_as);
            if (total_read==2){
              //save the file
              //strcpy(file_name,"/wren/");
              strcat(save_module_as,".wren");
              //strcat(file_name,save_module_as);
              //strcpy(save_module_as,file_name);
              Serial.print(F("M AppWren::MessageHandler: WREN_SCRIPT_SAVE as: "));
              Serial.println(save_module_as);
              save_module = true;
            }
            return;
        }else{ //if the message payload is not a command then assume its the data
            if(save_module){
              set_arm_clock(600000000);
              FsFile file;
              sd->chdir("/wren");
              sd->remove(save_module_as);
              sd->remove("temp.wren");
              sd->remove("test.wren");
              sd->remove("test2.wren");
              sd->remove(save_module_as);
              file = sd->open(save_module_as, O_WRONLY|O_CREAT);
              file.write(message,strlen(message));
              //file.write("//DEBUG\n\n",strlen("//DEBUG\n\n"));
              file.close();
              save_module = false;
              Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_SAVE complete"));
            }else if(compileOnly){
              Serial.println(F("M AppWren::MessageHandler: Compiling the received script"));
              return;
            }else{ //execute the script
                Serial.println(F("M AppWren::MessageHandler: Restarting the VM"));
                restartVM();
                Serial.println(F("M AppWren::MessageHandler: Loading the received script"));
                loadScript(message);
                Serial.println(F("M AppWren::MessageHandler: cache the wren handles"));
                getWrenHandles();
                wrenEnsureSlots(vm, 1);
                wrenSetSlotHandle(vm, 0, h_slot0);//App
                Serial.println(F("M AppWren::MessageHandler: request complete"));
                return;
            }
        }
    } else{
      //All other messages from senders other than the SCI will be forwarded to the VM
      //SerialUSB1 is dedicated to serial comms with the VM for stdio/stderr
      wrenEnsureSlots(vm, 3);
      wrenSetSlotHandle(vm, 0, h_slot0);//App
      wrenSetSlotString(vm, 1, sender->name);//sender
      wrenSetSlotString(vm, 2, message);//message
      isWrenResultOK(wrenCall(vm,h_messageHandler));
    }
}

/**
 * @brief responsible for managing the surface buffer memory allocation
 * 
 * @return true 
 * @return false 
 */
  bool AppWren::dynamicSurfaceManager(){
    if(!surface_cache){
      surface_mempool = wrenFastRam;
      if (has_pop || has_focus){
        surface_cache = new Surface(surface_mempool,width,height);
        memset(surface_mempool,0,sizeof(uint16_t)*width*height);
      }else{
        surface_cache = new Surface(surface_mempool, widget_width, widget_height);
        memset(surface_mempool,0,sizeof(uint16_t)*widget_width*widget_height);
      }  
    }else{
      //check for state changes and adjust the buffer accordingly
      if (has_pop || has_focus){
        if( (width != surface_cache->getWidth()) || (height != surface_cache->getHeight()) ){
          delete(surface_cache);
          surface_cache = new Surface(surface_mempool, width, height);
          memset(surface_mempool,0,sizeof(uint16_t)*width*height);
        }
      }else{
        if( (widget_width != surface_cache->getWidth()) || (widget_height != surface_cache->getHeight()) ){
          delete(surface_cache);
          surface_cache = new Surface(surface_mempool, widget_width, widget_height);
          memset(surface_mempool,0,sizeof(uint16_t)*widget_width*widget_height);
        }
      }  
    }
    if (surface_cache==NULL)return false;
    wrenCollectGarbage(vm);
    return true;
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
                        //surface_cache = new Surface(am->fastImgCacheSurfaceP, widget_width, widget_height);
                        if(!dynamicSurfaceManager()){ 
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
        set_arm_clock(600000000);
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
  h_onTouch = wrenMakeCallHandle(vm, "onTouch(x,y)");
  h_onTouchDrag = wrenMakeCallHandle(vm, "onTouchDrag(x,y)");
  h_onTouchRelease = wrenMakeCallHandle(vm, "onTouchRelease(x,y)");
  h_onAnalog1 = wrenMakeCallHandle(vm, "onAnalog1(_)");
  h_onAnalog2 = wrenMakeCallHandle(vm, "onAnalog2(_)");
  h_onAnalog3 = wrenMakeCallHandle(vm, "onAnalog3(_)");
  h_onAnalog4 = wrenMakeCallHandle(vm, "onAnalog4(_)");
  h_messageHandler = wrenMakeCallHandle(vm, "messageHandler(_,_)");
}

const char * AppWren::loadModuleSource(const char * name){
  char file_name[128];
  FsFile file;
  int16_t file_size; //max file size 32K
  strcpy(file_name,name);
  strcat(file_name,".wren");
  sd->chdir("/wren");
  file = sd->open(file_name, O_RDONLY);
  file_size = file.available();
  module_load_buffer = (char*)malloc(file_size);
  if (module_load_buffer == NULL){
    Serial.println(F("M AppWren::loadModuleSource ERROR: Not Enough Memory"));
    return NULL;
  }
  file.read(module_load_buffer,file_size);
  file.close();
  return module_load_buffer;
}

void AppWren::freeModuleSource(){
  if (module_load_buffer != NULL) free(module_load_buffer);
  module_load_buffer = NULL;
  return;
}
void AppWren::setPosition(int16_t newOriginX, int16_t newOriginY){
  AppBaseClass::setPosition(newOriginX, newOriginY);
}
void AppWren::setDimension(int16_t new_width, int16_t new_height){
  width = new_width;
  height = new_height;
}

void AppWren::setWidgetPosition(int16_t newOriginX, int16_t newOriginY){
  AppBaseClass::setWidgetPosition(newOriginX, newOriginY);
}
void AppWren::setWidgetDimension(int16_t new_width, int16_t new_height){
  widget_width = new_width;
  widget_height = new_height;
}
