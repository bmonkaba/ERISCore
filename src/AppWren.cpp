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
#include "Eris.h"
#include "AppManager.h"
#include "ErisUtils.h"


#ifdef USE_EXTMEM
uint16_t EXTMEM wrenFastRam[WREN_FRAME_BUFFER_SIZE]__attribute__ ((aligned (16)));
//char wrenFastVMRam[WREN_VM_HEAP_SIZE]__attribute__ ((aligned (16)));
#else
uint16_t DMAMEM wrenFastRam[WREN_FRAME_BUFFER_SIZE]__attribute__ ((aligned (16)));
//char wrenFastVMRam[WREN_VM_HEAP_SIZE]__attribute__ ((aligned (16)));
#endif
//char wrenFastVMRam[1]__attribute__ ((aligned (16)));

/**
 * @brief Wren system interface 
 * 
 */
//-- these are standard functions which MUST be provided to the VM
extern "C" {
  int _kill (pid_t pid, int signum){return 0;}
  pid_t _getpid(void){return 1;}
  int _times(void){return 0;}
  int _gettimeofday(struct timeval *tv, struct timezone *tz){return millis()/1000;}
  
}

/**
 * @brief write function system callback for the Wren VM. Stdout/Stderr is Serial
 * 
 * @param vm 
 * @param text 
 */
static void writeFn(WrenVM* vm, const char* text) {
  //Serial.printf("VM %s", text);
  AppManager* am = AppManager::getInstance();
  SvcSerialCommandInterface* sci = (SvcSerialCommandInterface*)am->getAppByName("SCI");
  if(sci->requestStartLZ4Message()){
    sci->printf("VM %s", text);
    sci->sendLZ4Message();
  };
}

/**
 * @brief error function system callback for the Wren VM. Stdout/Stderr is Serial
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
      Serial.printf("VM WREN_ERR [%s line %d] [Error] %s\n", module, line, msg);
      break;
    } 
    case WREN_ERROR_STACK_TRACE:
    {
      Serial.printf("VM WREN_ERR [%s line %d] in %s\n", module, line, msg);
      break;
    } 
    case WREN_ERROR_RUNTIME:
    {
      Serial.printf("VM WREN_ERR [Runtime Error] %s\n", msg);
      break;
    } 
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
    Serial.printf("M VM: loadModule complete %s freeing memory\n", module);
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

//
//
// APP EXTENTIONS FOR WREN
//
//
//

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
  app->returnFocus();
}

/**
 * @brief Set the CPU Clock Speed Callback object
 * 
 * @param vm 
 */
void setClockSpeedCallback(WrenVM* vm){
  //(uint16_t cpu_speed)
  AppManager* am = AppManager::getInstance();
  am->requestArmSetClock(wrenGetSlotDouble(vm, 1));
}

/**
 * @brief Set the Random Callback object
 * 
 * @param vm 
 */
void setRandomCallback(WrenVM* vm){
  //(uint16_t how_big)
  wrenSetSlotDouble(vm,0,random(wrenGetSlotDouble(vm, 1)));
}


/**
 * @brief restartVM callback for wren
 * 
 * @param vm 
 */
void restartVMCallback(WrenVM* vm){
  //(char* script)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  char* script = (char*)wrenGetSlotString(vm, 1);
  app->rebootRequest(script);
}

//
//
// AUDIO DIRECTOR EXTENTIONS FOR WREN
//
//
//

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorConnectCallback(WrenVM* vm){
  //(char* source, uint16_t source_port,char* dest, uint16_t dest_port)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
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
  bool b = app->getAudioDirector()->disconnectAll();
  wrenSetSlotBool(vm, 0,b);
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorGetFunctionListCallback(WrenVM* vm){
  //()
  int num_slots;
	int slot = 0;
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  AudioDirector* ad = app->getAudioDirector();
  num_slots = ad->getFunctionCount()+1;
  wrenEnsureSlots(vm, num_slots);
  wrenSetSlotNewList(vm, slot);
  while(++slot < num_slots) {
    char* cat_list;
    cat_list = ad->getFunctionListItem(slot-1);
		wrenSetSlotString(vm, slot, cat_list);
		wrenInsertInList(vm, 0, -1, slot);
	}
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorGetTypeListCallback(WrenVM* vm){
  //(char* function)
  int num_slots;
	int slot = 0;
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  AudioDirector* ad = app->getAudioDirector();
  const char * function = wrenGetSlotString(vm, 1);
  num_slots = ad->getTypeCountByFunction(function)+1;
  wrenEnsureSlots(vm, num_slots);
  wrenSetSlotNewList(vm, slot);
  while(++slot < num_slots) {
    char* cat_list;
    cat_list = ad->getTypeListItem(function,slot-1);
		wrenSetSlotString(vm, slot, cat_list);
		wrenInsertInList(vm, 0, -1, slot);
	}
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorGetTypeInstanceCountCallback(WrenVM* vm){
  //(char* type)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  AudioDirector* ad = app->getAudioDirector();
  const char * type = wrenGetSlotString(vm, 1);
  wrenSetSlotDouble(vm, 0,ad->getTypeInstanceCount(type));
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorDisableAudioInterrupts(WrenVM* vm){
  //()
  AudioNoInterrupts();
}

/**
 * @brief AudioDirector callback for wren
 * 
 * @param vm 
 */
void audioDirectorEnableAudioInterrupts(WrenVM* vm){
  //()
  AudioInterrupts();
}

//
//
// DATA DICTIONARY EXTENTIONS FOR WREN
//
//
//

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

//
//
// DRAW EXTENTIONS FOR WREN
//
//
//

/**
 * @brief VM callback for extention method
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
 * @brief VM callback for extention method
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
 * @brief VM callback for extention method
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
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void printWithFontCallback(WrenVM* vm){
  //(char* string)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  const char *string_buffer = wrenGetSlotString(vm,1);
  uint16_t x = wrenGetSlotDouble(vm,2);
  uint16_t y = wrenGetSlotDouble(vm,3);
  const char *font_name = wrenGetSlotString(vm,4);
  uint16_t pt = wrenGetSlotDouble(vm,5); //font size
  if(app->useNativeFS){
    app->getDraw()->printWithFont(string_buffer,x,y,font_name,pt,0);
  }else{
    app->getDraw()->printWithFont(string_buffer,x,y,font_name,pt,&app->wren_file_system);
  }
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void setFontSizeCallback(WrenVM* vm){
  //TODO
}



void getImageSizeCallback(WrenVM* vm){
  //(char* path,char* filename,int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  int32_t width, height;
  if(app->useNativeFS == true){
    app->getDraw()->getImageSize(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),&width,&height,0);
  }else{
    app->getDraw()->getImageSize(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),&width,&height,&app->wren_file_system);
  }
  wrenEnsureSlots(vm, 3);
  wrenSetSlotNewList(vm, 0);
  wrenSetSlotDouble(vm,1,width);
  wrenInsertInList(vm, 0, -1, 1);//insert the width from slot 1 
  wrenSetSlotDouble(vm,2,height);
  wrenInsertInList(vm, 0, -1, 2);//insert the height from slot 1
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void loadImageCallback(WrenVM* vm){
  //(char* path,char* filename,int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  bltMode bm;
  int32_t blt_mode_request = wrenGetSlotDouble(vm, 5);
  if (blt_mode_request == 0){ bm = BLT_COPY;
  }else if (blt_mode_request == 1){ bm = BLT_BLK_COLOR_KEY;
  }else if (blt_mode_request == 2){ bm = BLT_HATCH_BLK;
  }else if (blt_mode_request == 3){ bm = BLT_HATCH_XOR;
  }else if (blt_mode_request == 4){ bm = BLT_ADD;
  }else if (blt_mode_request == 5){ bm = BLT_SUB;
  }else if (blt_mode_request == 6){ bm = BLT_MULT;
  }else if (blt_mode_request == 7){ bm = BLT_DIV;
  }else if (blt_mode_request == 8){ bm = BLT_AND;
  }else if (blt_mode_request == 9){ bm = BLT_OR;
  }else if (blt_mode_request == 10){bm = BLT_XOR;
  }else if (blt_mode_request == 11){bm = BLT_MEAN;
  }else if (blt_mode_request == 12){bm = BLT_1ST_PIXEL_COLOR_KEY;
  }else return;
  if(app->useNativeFS){
    app->getDraw()->bltSD(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),bm);
  }else{
    app->bltRAMDrive2FrameBuffer(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),bm);
  }
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void loadImageSurfaceCallback(WrenVM* vm){
  //(char* path,char* filename,int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  bltMode bm;
  int32_t blt_mode_request = wrenGetSlotDouble(vm, 5);
  if (blt_mode_request == 0){ bm = BLT_COPY;
  }else if (blt_mode_request == 1){ bm = BLT_BLK_COLOR_KEY;
  }else if (blt_mode_request == 2){ bm = BLT_HATCH_BLK;
  }else if (blt_mode_request == 3){ bm = BLT_HATCH_XOR;
  }else if (blt_mode_request == 4){ bm = BLT_ADD;
  }else if (blt_mode_request == 5){ bm = BLT_SUB;
  }else if (blt_mode_request == 6){ bm = BLT_MULT;
  }else if (blt_mode_request == 7){ bm = BLT_DIV;
  }else if (blt_mode_request == 8){ bm = BLT_AND;
  }else if (blt_mode_request == 9){ bm = BLT_OR;
  }else if (blt_mode_request == 10){bm = BLT_XOR;
  }else if (blt_mode_request == 11){bm = BLT_MEAN;
  }else if (blt_mode_request == 12){bm = BLT_1ST_PIXEL_COLOR_KEY;
  }else return;
  if(app->useNativeFS){
    app->bltSD2Surface(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),bm);
  }else{
    app->bltRAMDrive2Surface(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),bm);
  }
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void bltCallback(WrenVM* vm){
  //(int16_t from_x, int16_t from_y, int16_t width, int16_t height, int16_t dest_x, int16_t dest_y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  bltMode bm;
  int32_t blt_mode_request = wrenGetSlotDouble(vm, 7);
  if (blt_mode_request == 0){ bm = BLT_COPY;
  }else if (blt_mode_request == 1){ bm = BLT_BLK_COLOR_KEY;
  }else if (blt_mode_request == 2){ bm = BLT_HATCH_BLK;
  }else if (blt_mode_request == 3){ bm = BLT_HATCH_XOR;
  }else if (blt_mode_request == 4){ bm = BLT_ADD;
  }else if (blt_mode_request == 5){ bm = BLT_SUB;
  }else if (blt_mode_request == 6){ bm = BLT_MULT;
  }else if (blt_mode_request == 7){ bm = BLT_DIV;
  }else if (blt_mode_request == 8){ bm = BLT_AND;
  }else if (blt_mode_request == 9){ bm = BLT_OR;
  }else if (blt_mode_request == 10){bm = BLT_XOR;
  }else if (blt_mode_request == 11){bm = BLT_MEAN;
  }else if (blt_mode_request == 12){bm = BLT_1ST_PIXEL_COLOR_KEY;
  }else return;
  
  app->bltSurface2FrameBuffer(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5), wrenGetSlotDouble(vm, 6),bm);
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void setPixelCallback(WrenVM* vm){
  //(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->setPixel(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void getPixelCallback(WrenVM* vm){
  //(int16_t x, int16_t y)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->getPixel(wrenGetSlotDouble(vm, 1),wrenGetSlotDouble(vm, 2)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void drawLineCallback(WrenVM* vm){
  //(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->drawLine(wrenGetSlotDouble(vm,1),wrenGetSlotDouble(vm,2),wrenGetSlotDouble(vm, 3),wrenGetSlotDouble(vm, 4),wrenGetSlotDouble(vm, 5), wrenGetSlotDouble(vm, 6), wrenGetSlotDouble(vm, 7));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void drawFillCallback(WrenVM* vm){
  //(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->drawFill(wrenGetSlotDouble(vm,1),wrenGetSlotDouble(vm,2),wrenGetSlotDouble(vm, 3));
}


void drawEnablePixelOp(WrenVM* vm){
  //(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  pixelOPMode pom;
  int32_t pixel_mode_request = wrenGetSlotDouble(vm, 2);
  if(pixel_mode_request==0) pom = PXOP_COPY;
  else if(pixel_mode_request==1) pom =  PXOP_BLK_COLOR_KEY;
  else if(pixel_mode_request==2) pom = PXOP_HATCH_BLK;
  else if(pixel_mode_request==3) pom = PXOP_HATCH_XOR;
  else if(pixel_mode_request==4) pom = PXOP_ADD;
  else if(pixel_mode_request==5) pom = PXOP_SUB;
  else if(pixel_mode_request==6) pom = PXOP_MULT;
  else if(pixel_mode_request==7) pom = PXOP_DIV;
  else if(pixel_mode_request==8) pom = PXOP_AND;
  else if(pixel_mode_request==9) pom = PXOP_OR;
  else if(pixel_mode_request==10) pom = PXOP_XOR;
  else if(pixel_mode_request==11) pom = PXOP_MEAN;
  else if(pixel_mode_request==12) pom = PXOP_1ST_PIXEL_COLOR_KEY;  
  app->enablePixelOP(wrenGetSlotDouble(vm, 1),pom);
}

void drawDisablePixelOp(WrenVM* vm){
  //(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->disablePixelOP();
}


/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void drawUseWrenFileSystem(WrenVM* vm){
  //()
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->useNativeFS = false;
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void drawUseSDFileSystem(WrenVM* vm){
  //()
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->useNativeFS = true;
}

//
//
// FILE SYSTEM EXTENTIONS FOR WREN
//
//
//


/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsOpenCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  uint8_t mode = 0;
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  if(strcmp("r",wrenGetSlotString(vm,2))==0){
    mode = 0;
  }else if(strcmp("w",wrenGetSlotString(vm,2))==0){
    mode = 1;
  }else if(strcmp("a",wrenGetSlotString(vm,2))==0){
    mode = 2;
  }
  app->wren_file = app->wren_file_system.open(wrenGetSlotString(vm,1),mode);
  if (app->wren_file){
    wrenSetSlotBool(vm, 0,true);
  }else wrenSetSlotBool(vm, 0,false);
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsExistsCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file_system.exists(wrenGetSlotString(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsMkdirCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file_system.mkdir(wrenGetSlotString(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsRenameCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file_system.rename(wrenGetSlotString(vm,1),wrenGetSlotString(vm,2)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsRemoveCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file_system.remove(wrenGetSlotString(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsRmdirCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file_system.rmdir(wrenGetSlotString(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsUsedSizeCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->wren_file_system.usedSize());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsTotalSizeCallback(WrenVM* vm){
  //(char* file, uint8_t mode)
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->wren_file_system.totalSize());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsimportFromSD(WrenVM* vm){
  //(src_path,src_file,dest_path,dst_file)
  char c[64];
  FsFile source; //The source is the SD card
  File dest; //The destination is the VM file system located in ext_ram
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  
  const char* src_path;
  const char* src_filename;
  const char* dst_path;
  const char* dst_filename;
  src_path = wrenGetSlotString(vm,1);
  src_filename = wrenGetSlotString(vm,2);
  dst_path = wrenGetSlotString(vm,3);
  dst_filename = wrenGetSlotString(vm,4);
  //am->requestArmSetClock(600000000);
  am->getSD()->chdir(src_path);
  source = am->getSD()->open(src_filename,O_RDONLY);
  strcpy(c,dst_path);
  strcat(c,dst_filename);
  dest = app->wren_file_system.open(c,O_RDWR);
  //c[2]=0; //null termination
  
  for(int32_t i= source.available(); i > 0; i--){
    source.readBytes(c,1);//read char
    dest.write(c,1);
  }
  source.close();
  dest.close();
  wrenSetSlotDouble(vm,0,app->wren_file_system.totalSize());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fsFormat(WrenVM* vm){
  //()
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->wren_file_system.quickFormat();
  app->wren_file_system.mkdir("system"); //add the system folder
}


//
//
// FILE EXTENTIONS FOR WREN
//
//
//

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileReadCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  uint16_t size = wrenGetSlotDouble(vm,1);
  char* buffer;
  buffer = (char*)extmem_malloc(size+1);
  memset(buffer,0,size+1);
  app->wren_file.read(buffer,size);
  wrenSetSlotString(vm,0,buffer);
  extmem_free(buffer);
}

void fileReadBytesCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  uint16_t size = wrenGetSlotDouble(vm,1);
  char* buffer;
  buffer = (char*)extmem_malloc(size);
  size = app->wren_file.readBytes(buffer,size);
  wrenSetSlotBytes(vm,0,buffer,size);
  extmem_free(buffer);
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileWriteCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->wren_file.write(wrenGetSlotString(vm,1));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileAvailableCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file.available());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void filePeekCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->wren_file.peek());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileFlushCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->wren_file.flush();
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileTruncateCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file.truncate(wrenGetSlotDouble(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileSeekCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file.seek(wrenGetSlotDouble(vm,1)));
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void filePositionCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->wren_file.position());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileSizeCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotDouble(vm,0,app->wren_file.size());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileCloseCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->wren_file.close();
  app->wren_file = 0;
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileIsOpenCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  if(app->wren_file){
    wrenSetSlotBool(vm,0,true);
  }else{
    wrenSetSlotBool(vm,0,false);
  }
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileNameCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotString(vm,0,app->wren_file.name());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileIsDirectoryCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  wrenSetSlotBool(vm,0,app->wren_file.isDirectory());
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileOpenNextCallback(WrenVM* vm){
  uint8_t mode = 0;
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  if(strcmp("r",wrenGetSlotString(vm,1))==0){
    mode = 0;
  }else if(strcmp("w",wrenGetSlotString(vm,1))==0){
    mode = 1;
  }else if(strcmp("a",wrenGetSlotString(vm,1))==0){
    mode = 2;
  }
  app->wren_file = app->wren_file.openNextFile(mode);
  if (app->wren_file){
    wrenSetSlotBool(vm, 0,true);
  }else wrenSetSlotBool(vm, 0,false);
}

/**
 * @brief VM callback for extention method
 * 
 * @param vm 
 */
void fileRewindDirectoryCallback(WrenVM* vm){
  AppManager* am = AppManager::getInstance();
  AppWren* app = (AppWren*)am->getAppByName("AppWren");
  app->wren_file.rewindDirectory();
}

//
//
//
// EXPORTED FUNCTION BINDINGS
//
//
//

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
WrenForeignMethodFn FLASHMEM bindForeignMethod(
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
      }else if (strcmp(signature, "setClockSpeed(_)") == 0){
        return setClockSpeedCallback;
      }else if (strcmp(signature, "random(_)") == 0){
        return setRandomCallback;
      }else if (strcmp(signature, "restartVM(_)") == 0){
        return restartVMCallback;
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
      }else if (strcmp(signature, "getFunctionList()") == 0){
        return audioDirectorGetFunctionListCallback;
      }else if (strcmp(signature, "getTypeList(_)") == 0){
        return audioDirectorGetTypeListCallback;
      }else if (strcmp(signature, "getTypeInstanceCount(_)") == 0){
        return audioDirectorGetTypeInstanceCountCallback;
      }else if (strcmp(signature, "enableInterrupts()") == 0){
        return audioDirectorEnableAudioInterrupts;
      }else if (strcmp(signature, "disableInterrupts()") == 0){
        return audioDirectorDisableAudioInterrupts;
      }
    }else if (strcmp(className, "Draw") == 0){ //these are static methods... attach them to the class, not an instance
      if (strcmp(signature, "setPixel(_,_,_,_,_)") == 0){
        return setPixelCallback;
      }else if (strcmp(signature, "getPixel(_,_)") == 0){
        return getPixelCallback;
      }else if (strcmp(signature, "setTextColor(_,_,_)") == 0){
        return setTextColorCallback;
      }else if (strcmp(signature, "setCursor(_,_)") == 0){
        return setCursorCallback;
      }else if (strcmp(signature, "print(_)") == 0){
        return printCallback;
      }else if (strcmp(signature, "print(_,_,_,_,_)") == 0){
        return printWithFontCallback;
      }else if (strcmp(signature, "setFontSize(_)") == 0){
        return setFontSizeCallback;
      }else if (strcmp(signature, "getImageSize(_,_)") == 0){
        return getImageSizeCallback;
      }else if (strcmp(signature, "loadImage(_,_,_,_,_)") == 0){
        return loadImageCallback;
      }else if (strcmp(signature, "loadImageToSurface(_,_,_,_,_)") == 0){
        return loadImageSurfaceCallback;
      }else if (strcmp(signature, "blt(_,_,_,_,_,_,_)") == 0){
        return bltCallback;
      }else if (strcmp(signature, "line(_,_,_,_,_,_,_)") == 0){
        return drawLineCallback;
      }else if (strcmp(signature, "fill(_,_,_)") == 0){
        return drawFillCallback;
      }else if (strcmp(signature, "useWrenFileSystem()") == 0){
        return drawUseWrenFileSystem;
      }else if (strcmp(signature, "useSDFileSystem()") == 0){
        return drawUseSDFileSystem;
      }else if (strcmp(signature, "enablePixelOP(_,_)") == 0){
        return drawEnablePixelOp;
      }else if (strcmp(signature, "disablePixelOP()") == 0){
        return drawDisablePixelOp;
      }
    }else if (strcmp(className, "FileSystem") == 0){
      if (strcmp(signature, "open(_,_)") == 0){
        return fsOpenCallback;
      }else if (strcmp(signature, "exists(_)") == 0){
        return fsExistsCallback;
      }else if (strcmp(signature, "mkdir(_)") == 0){
        return fsMkdirCallback;
      }else if (strcmp(signature, "rename(_,_)") == 0){
        return fsRenameCallback;
      }else if (strcmp(signature, "remove(_)") == 0){
        return fsRemoveCallback;
      }else if (strcmp(signature, "rmdir(_)") == 0){
        return fsRmdirCallback;
      }else if (strcmp(signature, "usedSize()") == 0){
        return fsUsedSizeCallback;
      }else if (strcmp(signature, "totalSize()") == 0){
        return fsTotalSizeCallback;
      }else if (strcmp(signature, "importFromSD(_,_,_,_)") == 0){
        return fsimportFromSD;
      }else if (strcmp(signature, "format()") == 0){
        return fsFormat;
      }
    }else if (strcmp(className, "File") == 0){
      if (strcmp(signature, "read(_)") == 0){
        return fileReadCallback;
      }else if (strcmp(signature, "readBytes(_)") == 0){
        return fileReadBytesCallback;
      }else if (strcmp(signature, "write(_)") == 0){
        return fileWriteCallback;
      }else if (strcmp(signature, "available()") == 0){
        return fileAvailableCallback;
      }else if (strcmp(signature, "peek()") == 0){
        return filePeekCallback;
      }else if (strcmp(signature, "flush()") == 0){
        return fileFlushCallback;
      }else if (strcmp(signature, "truncate(_)") == 0){
        return fileTruncateCallback;
      }else if (strcmp(signature, "seek(_,_)") == 0){
        return fileSeekCallback;
      }else if (strcmp(signature, "position()") == 0){
        return filePositionCallback;
      }else if (strcmp(signature, "size()") == 0){
        return fileSizeCallback;
      }else if (strcmp(signature, "close()") == 0){
        return fileCloseCallback;
      }else if (strcmp(signature, "isOpen()") == 0){
        return fileIsOpenCallback;
      }else if (strcmp(signature, "name()") == 0){
        return fileNameCallback;
      }else if (strcmp(signature, "isDirectory()") == 0){
        return fileIsDirectoryCallback;
      }else if (strcmp(signature, "openNextFile(_)") == 0){
        return fileOpenNextCallback;
      }else if (strcmp(signature, "rewindDirectory()") == 0){
        return fileRewindDirectoryCallback;
      }

    }
    // Other classes in main...
  }
  // Other modules...
  //else return NULL?
  return NULL;
}

//end wren callbacks

void FLASHMEM AppWren::vmConstructor(const char* initial_script){
    startVM();
    loadScript(initial_script);
    getWrenHandles();
    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, h_slot0);//App
    isWrenResultOK(wrenCall(vm,h_update));
}

void FLASHMEM AppWren::startVM(){
    Serial.println(F("\nM AppWren::startVM()"));
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
#ifdef USE_EXTMEM
    config.initialHeapSize = WREN_VM_HEAP_SIZE;//WREN_VM_HEAP_SIZE;
    config.minHeapSize = WREN_VM_HEAP_SIZE/2;
    config.heapGrowthPercent = 5;
    //config.reallocateFn = &wrenFastMemoryAllocator; 
#else
    config.initialHeapSize = WREN_VM_HEAP_SIZE * 2;//WREN_VM_HEAP_SIZE;
    config.minHeapSize = WREN_VM_HEAP_SIZE;
    config.heapGrowthPercent = 10;
#endif
    config.bindForeignMethodFn = &bindForeignMethod;
    config.loadModuleFn = &loadModule;
    
    vm = wrenNewVM(&config);
}

void FLASHMEM AppWren::messageHandler(AppBaseClass *sender, const char *message){
  if(0 == strncmp(message,"DEMO",strlen("DEMO"))){
    Serial.println(F("M AppWren::MessageHandler: Demo Mode Request"));
    rebootRequest("demo");
    return;
  }
  
  if(sender->isName("SCI")){
    if(0 == strncmp(message,"WREN_SCRIPT_EXECUTE",strlen("WREN_SCRIPT_EXECUTE"))) {
        Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_COMPILE -> compileOnly = false"));
        compile_only = false;
        return;
    }else if(0 == strncmp(message,"WREN_SCRIPT_COMPILE",strlen("WREN_SCRIPT_COMPILE"))) {
        Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_EXECUTE -> compileOnly = true"));
        compile_only = true;
        return;
    }else if(0 == strncmp(message,"WREN_SCRIPT_SAVE",strlen("WREN_SCRIPT_SAVE"))){
        char cmd[128];
        int total_read;
        total_read = sscanf(message, "%127s %23s\n" , cmd, wren_module_name);
        if (total_read==2){
          strcat(wren_module_name,".wren");
          Serial.print(F("M AppWren::MessageHandler: WREN_SCRIPT_SAVE as: "));
          Serial.println(wren_module_name);
          save_module = true;
        }
        return;
    }else{ //if the message payload is not a command then assume its the data
        if(save_module){
          FsFile file;
          sd->chdir("/wren");
          sd->remove(wren_module_name);
          file = sd->open(wren_module_name, O_WRONLY|O_CREAT);
          file.write(message,strlen(message));
          file.close();
          save_module = false;
          Serial.println(F("M AppWren::MessageHandler: WREN_SCRIPT_SAVE complete"));
          return;
        }else if(compile_only){
          Serial.println(F("M AppWren::MessageHandler: Compiling the received script"));
          return;
        }else{ //execute the script
            Serial.println(F("M ************************************************************************"));
            Serial.println(F("M AppWren::MessageHandler: Restarting the VM"));
            if(!has_focus){ 
              //cycling the focus triggers the parent app to recover any system changes made by the VM
              getFocus(); 
              returnFocus();
            }else{
              returnFocus();
            }
            restartVM();
            Serial.println(F("M AppWren::MessageHandler: Loading the received script"));
            if(!loadScript(message)){
                enable_call_forwarding = false;
                restartVM();
            } else enable_call_forwarding = true;
            
            Serial.println(F("M AppWren::MessageHandler: request complete"));
            Serial.println(F("M ************************************************************************"));
            return;
        }
    }
  }  
  //All other messages from senders other than the SCI will be forwarded to the VM
  //Serial is dedicated to serial comms with the VM for stdio/stderr
  wrenEnsureSlots(vm, 3);
  wrenSetSlotHandle(vm, 0, h_slot0);//App
  wrenSetSlotString(vm, 1, sender->name);//sender
  wrenSetSlotString(vm, 2, message);//message
  isWrenResultOK(wrenCall(vm,h_messageHandler));
    
}

bool AppWren::dynamicSurfaceManager(){
  if(!surface_cache){
    surface_mempool = wrenFastRam;
    if (has_pop || has_focus){
      surface_cache = new Surface((uint16_t *)draw->getFrameAddress(),width,height);
      memset((uint16_t *)draw->getFrameAddress(),0,sizeof(uint16_t)*WREN_FRAME_BUFFER_SIZE);
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
  //wrenCollectGarbage(vm);
  return true;
}

 void AppWren::render(){
        if (enable_call_forwarding == false){ //if no script loaded
            return;
        } else{
          dynamicSurfaceManager();
          if(reboot_request){
            reboot_request = false;
            //handle the reboot request from the running wren script
            restartVM();
            if(!loadScript(loadModuleSource(wren_module_name))){
                enable_call_forwarding = false;
                restartVM();
            } else enable_call_forwarding = true;
            freeModuleSource();
            return;
          }
          wrenEnsureSlots(vm, 1);
          wrenSetSlotHandle(vm, 0, h_slot0);//App
          if (!isWrenResultOK(wrenCall(vm,h_render))){
            releaseWrenHandles();
          }else{
              if(is_pressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                      show_active = false;
                      time_active = 0;
              }
              if(using_image){
                  if(!surface_cache){                        
                      //surface_cache = new Surface(am->p_fast_img_cache_surface, widget_width, widget_height);
                      if(!dynamicSurfaceManager()){ 
                          Serial.println(F("M AppWren::render() VM ERROR: Surface not available"));
                          return;
                      }else Serial.println(F("M AppWren::render() Surface created"));
                  } else{
                    if(has_pop || has_focus){
                      //do nothing
                    }else{
                      draw->bltSurface2Surface(am->p_display_surface,x,y,surface_cache,0,0,surface_cache->getWidth(),surface_cache->getHeight(),BLT_COPY);
                    }
                  }
              }else{
                  draw->fillRoundRect(x,y,w,h/2+3,3,am->data->read("UI_BUTTON_FILL_COLOR"));
                  draw->fillRoundRect(x,y+h/2,w,h/2,3,am->data->read("UI_BUTTON_SHADE_COLOR"));
              }
              if(has_pop || has_focus){
                //do nothing
              }else{
                if (show_active){
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_ACTIVE_BORDER_COLOR")); 
                } else{
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_INACTIVE_BORDER_COLOR"));
                }
                
                if(!using_image){
                    draw->setTextColor(am->data->read("UI_BUTTON_TEXT_COLOR"));
                    draw->setCursor(x+(w/2),y+(h/2),true);
                    draw->setFont(Arial_9);
                    //draw->print(text);
                }
              }
          }
        }
        am->data->update("VM_BYTES_ALLOCATED",(int32_t)wrenCollectGarbage(vm));
        draw->disablePixelOP();//disable pixel op and the end of the render cycle
    };    //called only when the app is active


void AppWren::update(){
  if (enable_call_forwarding == false){ //if no script loaded
      return;
  } else{
    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, h_slot0);//App
    if (!isWrenResultOK(wrenCall(vm,h_update))){
        releaseWrenHandles();
    };
  }
}; //allways called even if app is not active

void FLASHMEM AppWren::releaseWrenHandles(){
  Serial.printf(F("M AppWren::releaseWrenHandles\n"));
  //release any existing handles
  if (h_slot0!=NULL) {wrenReleaseHandle(vm, h_slot0); h_slot0=NULL;}
  if (h_render!=NULL) {wrenReleaseHandle(vm, h_render); h_render=NULL;}
  if (h_update!=NULL) {wrenReleaseHandle(vm, h_update); h_update=NULL;}
  if (h_onFocus!=NULL) {wrenReleaseHandle(vm, h_onFocus); h_onFocus=NULL;}
  if (h_onFocusLost!=NULL) {wrenReleaseHandle(vm, h_onFocusLost); h_onFocusLost=NULL;}
  if (h_onTouch!=NULL) {wrenReleaseHandle(vm, h_onTouch); h_onTouch=NULL;}
  if (h_onTouchDrag!=NULL) {wrenReleaseHandle(vm, h_onTouchDrag); h_onTouchDrag=NULL;}
  if (h_onTouchRelease!=NULL) {wrenReleaseHandle(vm, h_onTouchRelease); h_onTouchRelease=NULL;}
  if (h_onAnalog1!=NULL) {wrenReleaseHandle(vm, h_onAnalog1); h_onAnalog1=NULL;}
  if (h_onAnalog2!=NULL) {wrenReleaseHandle(vm, h_onAnalog2); h_onAnalog2=NULL;}
  if (h_onAnalog3!=NULL) {wrenReleaseHandle(vm, h_onAnalog3); h_onAnalog3=NULL;}
  if (h_onAnalog4!=NULL) {wrenReleaseHandle(vm, h_onAnalog4); h_onAnalog4=NULL;}
  if (h_messageHandler!=NULL) {wrenReleaseHandle(vm, h_messageHandler); h_messageHandler=NULL;}
  enable_call_forwarding = false;
}

void FLASHMEM AppWren::getWrenHandles(){
  Serial.println(F("M AppWren::getWrenHandles"));
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, "main", "ErisApp", 0); //get the instance to call the methods on
  //get the handles
  h_slot0 = wrenGetSlotHandle(vm, 0);
  h_render = wrenMakeCallHandle(vm, "render()");
  h_update = wrenMakeCallHandle(vm, "update()");  
  h_onFocus = wrenMakeCallHandle(vm, "onFocus()");
  h_onFocusLost = wrenMakeCallHandle(vm, "onFocusLost()");
  h_onTouch = wrenMakeCallHandle(vm, "onTouch(_,_)");
  h_onTouchDrag = wrenMakeCallHandle(vm, "onTouchDrag(_,_)");
  h_onTouchRelease = wrenMakeCallHandle(vm, "onTouchRelease(_,_)");
  h_onAnalog1 = wrenMakeCallHandle(vm, "onAnalog1(_)");
  h_onAnalog2 = wrenMakeCallHandle(vm, "onAnalog2(_)");
  h_onAnalog3 = wrenMakeCallHandle(vm, "onAnalog3(_)");
  h_onAnalog4 = wrenMakeCallHandle(vm, "onAnalog4(_)");
  h_messageHandler = wrenMakeCallHandle(vm, "messageHandler(_,_)");
}

const char * FLASHMEM AppWren::loadModuleSource(const char * name){
  char file_name[128]; 
  FsFile file;
  int16_t file_size; //max file size 32K
  safer_strncpy(file_name,name,sizeof(file_name));
  strcat(file_name,".wren");
  sd->chdir("/wren");
  if(!sd->exists(file_name)){
    sd->chdir("/wren/lib");
  }
  if(sd->exists(file_name)){
    file = sd->open(file_name, O_RDONLY);
    file_size = file.available() + 1;
#ifdef USE_EXTMEM  
    module_load_buffer = (char*)extmem_malloc(file_size);
#else
    module_load_buffer = (char*)malloc(file_size);
#endif
    if (module_load_buffer == NULL){
      Serial.println(F("M AppWren::loadModuleSource ERROR: Not Enough Memory"));
      return NULL;
    }
    memset(module_load_buffer,0,file_size);
    file.read(module_load_buffer,file_size);
    file.close();
  }
  return module_load_buffer;
}

void FLASHMEM AppWren::freeModuleSource(){

#ifdef USE_EXTMEM 
  if (module_load_buffer != NULL){
    extmem_free(module_load_buffer);
  }
#else
  if (module_load_buffer != NULL) free(module_load_buffer);
#endif
  module_load_buffer = NULL;
  return;
}

void AppWren::setPosition(int16_t newOriginX, int16_t newOriginY){
  AppBaseClass::setPosition(newOriginX, newOriginY);
}

void AppWren::setDimension(int16_t new_width, int16_t new_height){
  width = new_width;
  height = new_height;
  dynamicSurfaceManager();
}

void AppWren::setWidgetPosition(int16_t newOriginX, int16_t newOriginY){
  AppBaseClass::setWidgetPosition(newOriginX, newOriginY);
}

void AppWren::setWidgetDimension(int16_t new_width, int16_t new_height){
  widget_width = new_width;
  widget_height = new_height;
  dynamicSurfaceManager();
}
    
void AppWren::enablePixelOP(uint16_t param,pixelOPMode operation){
    am->data->update("DEBUG_PIXOP",(int32_t)operation);
    draw->enablePixelOP(param,operation);
}; 


void AppWren::disablePixelOP(){
    draw->disablePixelOP();
}; 