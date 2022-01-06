/**
 * @file AppManager.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//#define SERIAL_PRINT_APP_LOOP_TIME
#include <Arduino.h>
#include "HSI.h"
#include "touch.h"
#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
#include "ili9341_t3n_font_Arial.h"
#include "AppManager.h"
#include "globaldefs.h"
#include "svcSerialCommandInterface.h"
#include <pgmspace.h>

extern AudioDirector _ad;
extern SvcSerialCommandInterface sci;

Touch touch(CS_TOUCH);
ILI9341_t3_ERIS FASTRUN draw(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);
uint16_t DMAMEM FB1[320 * 240] __attribute__ ((aligned (16)));
uint16_t DMAMEM imgCache[AM_IMG_CACHE_SIZE] __attribute__ ((aligned (16)));
SvcDataDictionary FASTRUN _data; 


/**
 * @brief Construct a new App Manager:: App Manager object using a private constuctor (lazy singleton pattern)
 * 
 */
AppManager:: AppManager(){
  ad = 0;
  data = &_data;
  root = 0;
  activeID = 0;
  pActiveApp = 0;
  nextIDAssignment = 1; //id 0 is reserved
  monitor_dd_update_timer = 0;
  state = redraw_objects;
  //attach the global imgCache buffer to a surface
  memset(imgCache,0,sizeof(imgCache));
  fastImgCacheSurfaceP = new Surface(&imgCache[0],AM_IMG_CACHE_SIZE);
  displaySurfaceP = new Surface(&FB1[0],320,240);

  //init the app focus and app stacks
  memset(&appFocusStack,0,sizeof(appFocusStack));
  memset(&appPopUpStack,0,sizeof(appPopUpStack));
  appFocusStackIndex = 0;
  appPopUpStackIndex = 0;
  exclusive_app_render = false;
  cycle_time_max = 0;
  //init the sd card
  if (!sd.begin(SdioConfig(FIFO_SDIO))){
    Serial.println(F("M AppManager: FATAL! SD Card Not Found "));
    sd.initErrorHalt(&Serial);
  } else {Serial.println(F("AppManager: SD Card FOUND"));}
  //init the display
  Serial.println(F("M AppManager: Config display"));
  draw.setPWMPin(TFT_LED_PWM); //library will control the backlight
  draw.setSD(&sd); //provide a cd pointer to the sd class
  draw.setFrameBuffer(FB1);
  draw.useFrameBuffer(true);
  
  //render.setPWMPin(TFT_LED_PWM); //library will control the backlight
  //render.setSD(&sd); //provide a cd pointer to the sd class
  //render.setFrameBuffer(FB1);
  //render.useFrameBuffer(true);
  animated_wallpaper.setSD(&sd);
  animated_wallpaper.setPath(UI_ANIMATED_WALLPAPER_PATH);
  Serial.println(F("M AppManager: Init display"));
  draw.begin();
  //render.begin();
  Serial.println(F("M AppManager: Init touch controller"));
  //init the touch screen
  touch.setCalibrationInputs(452,374,3830,3800); //inital cal values; app manager will monitor and update
  touch.setRotation(3);
  touch.begin();
  touch_state = 0;
  //enable cpu temp monitoring
  tempmon_init();
  //set the default template colors for the gui
  data->create(UI_BUTTON_FILL_COLOR,(int32_t)76);
  data->create(UI_BUTTON_SHADE_COLOR,(int32_t)37);
  data->create(UI_BUTTON_ACTIVE_BORDER_COLOR,(int32_t)23254);
  data->create(UI_BUTTON_INACTIVE_BORDER_COLOR,(int32_t)23244);
  data->create(UI_BUTTON_TEXT_COLOR,(int32_t)50713);
  data->create(UI_SLIDER_BORDER_COLOR,(int32_t)44374);
  data->create(UI_SLIDER_SHADE_COLOR,(int32_t)44344);
  data->create(UI_SLIDER_FILL_COLOR,(int32_t)1530);
  data->create(UI_SLIDER_TEXT_COLOR,(int32_t)39222);

  Serial.println(F("M AppManager: Contructor complete"));
  #ifdef ENABLE_ASYNC_SCREEN_UPDATES
  //render.updateScreenAsync(true);
  display_refresh_time = 0;
  #endif
  
};

/**
 * @brief AppManager update executes single update of the state machine
 * 
 */
void FLASHMEM AppManager::update(){
    elapsedMicros app_time;
    uint32_t heapTop;
    uint32_t drt;
    bool touch_updated;
    bool update_analog;
    AppBaseClass *node;
    bool monitor_update;
    
    monitor_update = (monitor_dd_update_timer > APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC);
    cycle_time=0;
    drt = display_refresh_time;
    touch_updated = false;
    node = root;

    if (node == 0){
      Serial.println(F("AppManager::update called without an application initalized"));
      return;
    }
  
    //update analog inputs
    update_analog = analog.update();
    
    //update loop is directed by the state variable 
    switch(state){
      case redraw_background:
        app_time=0;
        if (!draw.busy()){
            data->update(RENDER,(int32_t)0);
          if (animated_wallpaper.getNextFrameChunk()){
              draw.bltSDAnimationFullScreen(&animated_wallpaper);
              data->update(AM_REDRAW_BG,(int32_t)app_time);
          } else Serial.println(F("M bad chunk"));
        } else Serial.println(F("M draw busy"));
        if ((data->read(RENDER) == 0) && animated_wallpaper.isFrameComplete()){
          data->update(RENDER,(int32_t)1);
          state = redraw_objects;
        }
        break;
        
      case redraw_wait://idle render time to give the screen refresh a head start
        if (drt > 22){ //magic number is to tune the delay between frame buffer writes
                       //as the data is simutaniously being written out to SPI.
                       //This ensures we are not overwritting the current frame 
                       //with the next frame. 
          if(exclusive_app_render) state = redraw_objects;
          else state = redraw_background;
        };
        //note: this is  a good place for application manager housekeeping tasks where screen access is not required
        data->update(AM_AUDIO_CPU_MAX,(float32_t)AudioProcessorUsageMax());
        float32_t cpu;
        cpu = AudioProcessorUsage();
        data->update(AM_AUDIO_CPU,(float32_t)cpu);
        data->update(AM_AUDIO_MEM_MAX,(int32_t)AudioMemoryUsageMax());
        data->update(AM_AUDIO_MEM,(int32_t)AudioMemoryUsage());
        break;

      case redraw_render:
        app_time=0;      
        data->update(RENDER_PERIOD,(int32_t)drt);
        data->update(RENDER,(int32_t)4);
        data->increment(RENDER_FRAME);
        draw.updateScreenAsync(false);//updateScreenAsyncFrom(&draw,false);
        state = redraw_wait;
        display_refresh_time = 0;
        break;

      case redraw_objects:      
        node = root;
        if (exclusive_app_render == false){
          do{
              app_time=0;
              if (node->update_period > node->update_period_max) node->update_period_max = node->update_period;
              node->update(); //update active window
              node->update_cpu_time = app_time;
              if (node->update_cpu_time > node->update_cpu_time_max){
                  node->update_cpu_time_max = node->update_cpu_time;
              }
              //update the data dictionary
              node->update_period =0;
              node=node->nextAppicationNode;//check next node
            }while(node !=NULL);
        };
        data->update(RENDER,(int32_t)2);
        data->increment(UPDATE_CALLS);
        state = redraw_popup;
        break;

      case redraw_popup:
        app_time=0;
        //draw any popups
        if (!appPopUpStackIndex==0 && appPopUpStack[appPopUpStackIndex-1] != 0){
          node = getApp(appPopUpStack[appPopUpStackIndex-1]);
          if (node != 0){
            //Serial.println(F("AppManager::redraw_popup"));
            node->update();
          }
        }
        data->update(RENDER,(int32_t)3);
        state = redraw_render;     
        break;
    }
    //always call the apps updateRT function on every loop
    touch.update();
    touch_updated = true;    
    data->increment(RT_CALLS);
    node = root;
    bool isactive_child;
    do{
      app_time=0;
      if (node->updateRT_priority > 0){
        node->updateRT_priority--;
      }else{
        if (node->updateRT_period > node->updateRT_period_max) node->updateRT_period_max = node->updateRT_period;
        node->updateRT(); //real time update (always called)
        node->updateRT_cpu_time = app_time;
        if (node->updateRT_cpu_time > node->updateRT_cpu_time_max) node->updateRT_cpu_time_max = node->updateRT_cpu_time;
        node->updateRT_period =0;
      }
      isactive_child = false;
      if (node->id == activeID && !draw.busy()) {
          if (pActiveApp != node){
            if (pActiveApp != 0) pActiveApp->onFocusLost();
            node->onFocus();
            pActiveApp = node;
          }
      }
      if (node->parentNode!=NULL){if(node->parentNode->id == activeID){isactive_child = true;}}; //send event triggers to any child apps
      if (node->id == activeID || isactive_child) {
          //Serial.print("AppManager::updating active application");Serial.println(activeID);
          //trigger any events and then call the update function for this 'active' app or child node
          if (update_analog){
            data->update(AM_AN1,(int32_t)analog.readAN1());
            data->update(AM_AN2,(int32_t)analog.readAN2());
            data->update(AM_AN3,(int32_t)analog.readAN3());
            data->update(AM_AN4,(int32_t)analog.readAN4());
            
            node->onAnalog1(analog.readAN1(),analog.freadAN1());
            node->onAnalog2(analog.readAN2(),analog.freadAN2());
            node->onAnalog3(analog.readAN3(),analog.freadAN3());
            node->onAnalog4(analog.readAN4(),analog.freadAN4());
          }
          if (touch_updated == true && touch.touched()) {
            p = touch.getPoint();
            //data->update("TOUCH_X",(int32_t)p.x);
            //data->update("TOUCH_Y",(int32_t)p.y);
            data->update("TOUCH_Z",(int32_t)p.z);
            if (node->touch_state == 0){
                node->onTouch(p.x, p.y);
                node->touch_state=1;
            } else{
                node->onTouchDrag(p.x, p.y);
            }
          } else if (node->touch_state==1){
            node->touch_state=0;
            node->onTouchRelease(p.x, p.y);
          }
      }
      node->cycle_time = cycle_time;
      if (node->cycle_time > node->cycle_time_max){
          node->cycle_time_max = node->cycle_time;
      }
      node=node->nextAppicationNode;//check next node
    }while(node !=NULL);

    if (cycle_time > cycle_time_max){
      cycle_time_max = cycle_time;
    } else{
      //delayMicroseconds((cycle_time_max - cycle_time)/100);
    }
    
    if(monitor_update){
      monitor_dd_update_timer = 0;
      data->update(AM_LOOP_TIME,(int32_t)cycle_time_max);
      data->update(AM_SERIAL_AVAIL,(int32_t)Serial.availableForWrite());
      //data->update("FRAME_PTR1",(int32_t)draw.getFrameAddress());
      //data->update("FRAME_PTR2",(int32_t)render.getFrameAddress());
      int32_t free_mem;
      void* htop;
      free_mem = 1000;
      cycle_time_max*=0.999;
      htop = malloc(1000);
      while (htop){
        memset(htop,0x5A,1000);
        heapTop = (uint32_t) htop;
        free(htop);
        free_mem += 1000;
        htop = malloc(free_mem);
      }
      
      data->update(FREE_MEM,(int32_t)(free_mem - 1000));
      data->update(HEAP_FREE,(int32_t)(0x20280000 - heapTop));
      data->update(LOCAL_MEM,(int32_t)(0x2007F000 - (uint32_t)(&heapTop)));
      heapTop = 0;
      data->update(CPU_TEMP,(float32_t)tempmonGetTemp());
      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();    
    }
};
/**
 * @brief provides an interface for apps to request the SdFs object
 * 
 * @return SdFs* 
 */
SdFs* AppManager::getSD(){
  return &sd;
}

/**
 * @brief provides an interface for apps to request another app object by id
 * 
 * @param id 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getApp(uint16_t id){
    AppBaseClass *node = root;
    do{
      if (node->id == id){
        return node;
      }
      node=node->nextAppicationNode;
    }while(node !=NULL);
    return NULL;
}
/**
 * @brief provides an interface for apps to request pop up focus with or without exclusive screen access
 * 
 * @param id 
 * @param exclusive 
 * @return true 
 * @return false 
 */
bool AppManager::requestPopUp(uint16_t id,bool exclusive){
  if (appPopUpStackIndex == 8) return false;
  appPopUpStack[appPopUpStackIndex] = id;
  appPopUpStackIndex+=1;
  exclusive_app_render = exclusive;
  return true;
}
/**
 * @brief provides an interface for apps to release pop up focus
 * 
 * @return true 
 * @return false 
 */
bool AppManager::releasePopUp(){
  if (appFocusStackIndex == 0)return false;
  appPopUpStackIndex-=1;
  appPopUpStack[appPopUpStackIndex] = 0;
  exclusive_app_render = false;
  return true;
}
/**
 * @brief provides an interface for apps to request focus
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool AppManager::getFocus(uint16_t id){
  if (appPopUpStackIndex == 8) return false;
  appFocusStack[appFocusStackIndex++] = activeID;
  activeID=id;
  return true;
} 
/**
 * @brief provides an interface for apps to release focus
 * 
 * @return true 
 * @return false 
 */
bool AppManager::returnFocus(){
  if (appFocusStackIndex == 0) return false;
  activeID = appFocusStack[--appFocusStackIndex];//load from the stack
  appFocusStack[appFocusStackIndex] = 0;
  return true;
} 
/**
 * @brief provides an interface for apps to request the id of the active app
 * 
 * @return uint16_t 
 */
uint16_t AppManager::peekAppFocus(){return activeID;}
/**
 * @brief provides an interface for apps to request the active app object
 * 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getActiveApp(){ return pActiveApp;}
/**
 * @brief provides an interface for apps to send messages to other apps
 * 
 * @param sender 
 * @param to_app 
 * @param message 
 * @return true 
 * @return false 
 */
bool AppManager::sendMessage(AppBaseClass *sender, const char *to_app, const char *message){
  // sends a message to the requested app
  AppBaseClass *node = root;
  do{
    if (node->isName(to_app)){
      node->MessageHandler(sender,message);
      return true;
    }
    node=node->nextAppicationNode;//check next node
  }while(node !=NULL);
  return false;
}


/**
 * @brief Get the App pointer By Name. Returns NULL if not found.
 * 
 * @param appName 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getAppByName(const char *appName){
  AppBaseClass *node = root;
  do{
    if (node->isName(appName)){
      return node;
    }
    node=node->nextAppicationNode;//check next node
  }while(node !=NULL);
  return NULL;
}

/**
 * @brief prints out some stats in JSON format to the serial port
 * 
 */
void AppManager::printStats (){
  SvcSerialCommandInterface* sci = (SvcSerialCommandInterface*)getAppByName("SCI"); //request the serial command interface
  sci->startLZ4Message();
  AppBaseClass *node = root;
  if (node == NULL) return;
  sci->print(F("STATS {\"APPS\":{"));
  do{
    sci->print(F("\""));
    sci->print(node->name);
    sci->print(F("\":{"));
    sci->print(F("\"update_cpu_time_max\":"));sci->print(node->update_cpu_time_max);sci->print(F(","));
    sci->print(F("\"updateRT_cpu_time_max\":"));sci->print(node->updateRT_cpu_time_max);sci->print(F(","));
    sci->print(F("\"update_period_max\":"));sci->print(node->update_period_max);sci->print(F(","));
    sci->print(F("\"updateRT_period_max\":"));sci->print(node->updateRT_period_max);
    sci->print(F("},"));
    //clear the stats
    node->update_cpu_time_max = 0;
    node->updateRT_cpu_time_max = 0;
    node->cycle_time_max = 0;
    node->update_period_max = 0;
    node->updateRT_period_max = 0;
    node=node->nextAppicationNode;//check next node
  }while(node !=NULL);
  if (root != 0){
    sci->print(F("\"root\":\""));sci->print(root->name);sci->print(F("\""));
  }else{
    sci->print(F("\"root\":"));sci->print(F("\"NULL\""));
  }
  sci->println(F("}}"));
  //sci->endLZ4Message();

  //print app manager stats
  //sci->startLZ4Message();
  sci->print(F("STATS {\"APPMANAGER\":{"));
  sci->print(F("\"cpu_time\":"));sci->print(cycle_time);sci->print(F(","));
  sci->print(F("\"cpu_time_max\":"));sci->print(cycle_time_max);sci->print(F(","));
  sci->print(F("\"touch_state\":"));sci->print(touch_state);sci->print(F(","));
  sci->print(F("\"active_app_id\":"));sci->print(activeID);sci->print(F(","));
  sci->print(F("\"exclusive_app_render\":"));sci->print(exclusive_app_render);
  sci->println(F("}}"));
  sci->endLZ4Message();
  //clear the app manager stats
  cycle_time_max = 0;
  return;
}
/**
 * @brief provides an interface for apps to register themselves with the AppManager
 * 
 * @param app 
 */
void AppManager::RegisterApp(AppBaseClass *app){
  //assign a unique id to the object
  app->id = nextIDAssignment++;
  app->draw = &draw;
  app->am = this;
  app->ad = &_ad;
  app->sd = &sd;
  app->sci = &sci;
  if (root == 0) {root = app; return;}
  else{
    AppBaseClass *endNode = root;
    //add to the linked list
    while(endNode->nextAppicationNode !=0){endNode=endNode->nextAppicationNode;}
    endNode->nextAppicationNode = app;
    app->previousAppicationNode = endNode;
  }
};

AppManager* AppManager::obj = 0; // or NULL, or nullptr in c++11

