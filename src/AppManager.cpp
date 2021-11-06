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
//#include "AudioDirector.h"
#include "AppManager.h"

Touch touch(CS_TOUCH);
ILI9341_t3_ERIS draw(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);

uint16_t DMAMEM FB1[320 * 240] __attribute__ ((aligned (32)));
//DMAMEM uint16_t FB2[320 * 240]__attribute__ ((aligned (32)));

SvcDataDictionary _data __attribute__ ((aligned(32)));;


AppManager::AppManager(){ //private constuctor (lazy singleton pattern)
  data = &_data;
  root = 0;
  activeID = 0;
  pActiveApp = 0;
  nextIDAssignment = 1; //id 0 is reserved
  monitor_dd_update_timer = 0;
  state = redraw_objects;
  //init the app focus and app stacks
  memset(&appFocusStack,0,sizeof(appFocusStack));
  memset(&appPopUpStack,0,sizeof(appPopUpStack));
  appFocusStackIndex = 0;
  appPopUpStackIndex = 0;
  exclusive_app_render = false;
  cycle_time_max = 0;
  //init the sd card
  if (!sd.begin(SdioConfig(FIFO_SDIO))){
    Serial.println(F("AppManager: FATAL! SD Card Not Found "));
    sd.initErrorHalt(&Serial);
  } else {Serial.println(F("AppManager: SD Card FOUND"));}
  //init the display
  Serial.println(F("AppManager: Config display"));
  draw.setPWMPin(TFT_LED_PWM); //library will control the backlight
  draw.setSD(&sd); //provide a cd pointer to the sd class
  draw.setFrameBuffer(FB1);
  draw.useFrameBuffer(true);
  //render.setPWMPin(TFT_LED_PWM); //library will control the backlight
  //render.setSD(&sd); //provide a cd pointer to the sd class
  //render.setFrameBuffer(FB1);
  //render.useFrameBuffer(true);
  animated_wallpaper.setPath("/V/BROKCHRD");
  Serial.println(F("AppManager: Init display"));
  draw.begin();
  //render.begin();
  Serial.println(F("AppManager: Init touch controller"));
  //init the touch screen
  touch.setCalibrationInputs(452,374,3830,3800); //inital cal values; app manager will monitor and update
  touch.setRotation(3);
  touch.begin();
  touch_state = 0;
  Serial.println(F("AppManager: Contructor complete"));
  #ifdef ENABLE_ASYNC_SCREEN_UPDATES
  //render.updateScreenAsync(true);
  display_refresh_time = 0;
  #endif
};

void AppManager::update(){
    elapsedMicros app_time;
    uint32_t heapTop;
    void* htop;
    uint32_t drt;
    bool touch_updated;
    bool update_analog;
    AppBaseClass *node;
    bool isactive_child;
    bool monitor_update;
    
    monitor_update = (monitor_dd_update_timer > APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC);
    cycle_time=0;
    drt = display_refresh_time;
    touch_updated = false;
    update_analog = false;
    node = root;

    //update touch
    app_time=0;
    touch.update();
    touch_updated = true;
    //update analog inputs
    update_analog = analog.update();
    data->update("AM_TOUCH",app_time);
    
    if (node == 0){
      Serial.println(F("AppManager::update called without an application initalized"));
      return;
    }

    //update loop is directed by the state variable 
    switch(state){
      case redraw_background:
        app_time=0;
        if (!draw.busy()){
            data->update("RENDER",0);
          if (animated_wallpaper.getNextFrameChunk(&sd)){
              draw.bltSDAnimationFullScreen(&animated_wallpaper);   
          }
        }
        if ((data->read("RENDER") == 0) && animated_wallpaper.isFrameComplete()){
          //Serial.printf(F("AppManager::isFrameComplete %d\n"),drt);
          data->update("RENDER",1);
          state = redraw_objects;
        }
        data->update("AM_DRAW",app_time);    
        break;
        
      case redraw_wait: //idle render time to give the screen refresh a head start
        if (drt > 30){  
          if(exclusive_app_render) state = redraw_objects;
          else state = redraw_background;
        }
        //note: this is  a good place for application manager housekeeping tasks where screen access is not required
        
        break;

      case redraw_render:
        app_time=0;      
        data->update("RENDER_PERIOD",drt);
        data->update("RENDER",4);
        data->increment("RENDER_FRAME");
        //delayNanoseconds(20);
        draw.updateScreenAsync(false);//updateScreenAsyncFrom(&draw,false);
        state = redraw_wait;
        display_refresh_time = 0;
        data->update("AM_RENDER",app_time);
        break;

      case redraw_objects:        
        node = root;
        if (exclusive_app_render == false){
          do{
              app_time=0;
              if (node->update_call_period > node->update_call_period_max) node->update_call_period_max = node->update_call_period;
              node->update(); //update active window
              node->update_loop_time = app_time;
              if (node->update_loop_time > node->update_loop_time_max){
                  node->update_loop_time_max = node->update_loop_time;
              }
              //update the data dictionary
              node->update_call_period =0;
              node=node->nextAppicationNode;//check next node
            }while(node !=NULL);
        };
        data->update("RENDER",2);
        data->increment("UPDATE_CALLS");
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
        data->update("RENDER",3);
        state = redraw_render;     
        break;
    }
    
    //always call the apps updateRT function on every loop
    data->increment("RT_CALLS");
    node = root;
    do{
      app_time=0;
      if (node->updateRT_call_period > node->updateRT_call_period_max) node->updateRT_call_period_max = node->updateRT_call_period;
      node->updateRT(); //real time update (always called)
      node->updateRT_loop_time = app_time;
      if (node->updateRT_loop_time > node->updateRT_loop_time_max) node->updateRT_loop_time_max = node->updateRT_loop_time;
      node->updateRT_call_period =0;
      isactive_child = false;
      if (node->id == activeID && !draw.busy()) {
          if (pActiveApp != node){
            if (pActiveApp != 0) pActiveApp->onFocusLost();
            node->onFocus();
            delayNanoseconds(220000);
            pActiveApp = node;
          }
      }
      if (node->parentNode!=NULL){if(node->parentNode->id == activeID){isactive_child = true;}}; //send event triggers to any child apps
      if (node->id == activeID || isactive_child) {
          //Serial.print("AppManager::updating active application");Serial.println(activeID);
          //trigger any events and then call the update function for this 'active' app or child node
          if (update_analog){
            data->update("AN1",analog.readAN1());
            data->update("AN2",analog.readAN2());
            data->update("AN3",analog.readAN3());
            data->update("AN4",analog.readAN4());
            
            node->onAnalog1(analog.readAN1(),analog.freadAN1());
            node->onAnalog2(analog.readAN2(),analog.freadAN2());
            node->onAnalog3(analog.readAN3(),analog.freadAN3());
            node->onAnalog4(analog.readAN4(),analog.freadAN4());
          }
          if (touch_updated == true && touch.touched()) {
            p = touch.getPoint();
            data->update("TOUCH_X",(int32_t)p.x);
            data->update("TOUCH_Y",(int32_t)p.y);
            //Serial.print(p.x);Serial.print(" ");Serial.println(p.y);
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

    if (cycle_time > cycle_time_max) cycle_time_max = cycle_time;
    
    if(monitor_update){
      monitor_dd_update_timer = 0;
      data->update("LOOP_TIME",cycle_time);
      data->update("SERIAL_AVAIL",Serial.availableForWrite());
      //data->update("FRAME_PTR1",(int32_t)draw.getFrameAddress());
      //data->update("FRAME_PTR2",(int32_t)render.getFrameAddress()); 
      cycle_time_max=0;
      htop = malloc(1000);
      memset(htop,0x5A,1000);
      heapTop = (uint32_t) htop;
      free(htop);
      data->update("HEAP_FREE",0x20280000 - heapTop);
      data->update("LOCAL_MEM",0x2007F000 - (uint32_t)(&heapTop));
      heapTop = 0;
    } 
};

SdFs* AppManager::getSD(){
  return &sd;
}

AppBaseClass* AppManager::getApp(uint16_t id){
  // sends a message to the requested app
    AppBaseClass *node = root;
    do{
      if (node->id == id){
        return node;
      }
      node=node->nextAppicationNode;//check next node
    }while(node !=NULL);
    return NULL;
}

bool AppManager::requestPopUp(uint16_t id,bool exclusive){
  if (appPopUpStackIndex == 8) return false;
  appPopUpStack[appPopUpStackIndex] = id;
  appPopUpStackIndex+=1;
  exclusive_app_render = exclusive;
  return true;
}
bool AppManager::releasePopUp(){
  if (appFocusStackIndex == 0)return false;
  //appPopUpStack[appPopUpStack.size()-1]=0;
  appPopUpStackIndex-=1;
  appPopUpStack[appPopUpStackIndex] = 0;
  exclusive_app_render = false;
  return true;
}

bool AppManager::getFocus(uint16_t id){
  if (appPopUpStackIndex == 8) return false;
  appFocusStack[appFocusStackIndex++] = activeID;
  activeID=id;
  return true;
} 

bool AppManager::returnFocus(){
  if (appFocusStackIndex == 0) return false;
  activeID = appFocusStack[--appFocusStackIndex];//load from the stack
  appFocusStack[appFocusStackIndex] = 0;
  return true;
} 

uint16_t AppManager::peekAppFocus(){return activeID;}//used by apps to find out which has focus

AppBaseClass* AppManager::getActiveApp(){ return pActiveApp;}

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
void AppManager::printStats(){
  AppBaseClass *node = root;
  if (node == NULL) return;
  Serial.print(F("STATS {\"APPS\":{"));
  do{
    //Serial.flush();
    Serial.print(F("\""));
    Serial.print(node->name);
    Serial.print(F("\":{"));
    //Serial.print("\tupdate_loop_time: ");Serial.print(node->update_loop_time);
    Serial.print(F("\"update_loop_time_max\":"));Serial.print(node->update_loop_time_max);Serial.print(F(","));
    //Serial.print("\tupdateRT_loop_time: ");Serial.print(node->updateRT_loop_time);
    Serial.print(F("\"updateRT_loop_time_max\":"));Serial.print(node->updateRT_loop_time_max);Serial.print(F(","));
    //Serial.print("\tcycle_time: ");Serial.print(node->cycle_time);
    Serial.print(F("\"cycle_time_max\":"));Serial.print(node->cycle_time_max);Serial.print(F(","));
    //Serial.print("\tupdate_call_period: ");Serial.print(node->update_call_period);
    Serial.print(F("\"update_call_period_max\":"));Serial.print(node->update_call_period_max);Serial.print(F(","));
    Serial.print(F("\"updateRT_call_period_max\":"));Serial.print(node->updateRT_call_period_max);
    Serial.print(F("},"));
    //clear the stats
    node->update_loop_time_max = 0;
    node->updateRT_loop_time_max = 0;
    node->cycle_time_max = 0;
    node->update_call_period_max = 0;
    node->updateRT_call_period_max = 0;
  
    node=node->nextAppicationNode;//check next node
  }while(node !=NULL);

  if (root != 0){
    Serial.print(F("\"root\":\""));Serial.print(root->name);Serial.print(F("\""));
  }else{
    Serial.print(F("\"root\":"));Serial.print(F("\"NULL\""));
  }
  
  Serial.println(F("}}"));
  //Serial.flush();
  //print app manager stats
  Serial.print(F("STATS {\"APPMANAGER\":{"));
  Serial.print(F("\"cycle_time\":"));Serial.print(cycle_time);Serial.print(F(","));
  Serial.print(F("\"cycle_time_max\":"));Serial.print(cycle_time_max);Serial.print(F(","));
  Serial.print(F("\"touch_state\":"));Serial.print(touch_state);Serial.print(F(","));
  Serial.print(F("\"active_app_id\":"));Serial.print(activeID);Serial.print(F(","));
  Serial.print(F("\"exclusive_app_render\":"));Serial.print(exclusive_app_render);
  Serial.println(F("}}"));
  //clear the stats
  cycle_time_max = 0;
  return;
}

void AppManager::RegisterApp(AppBaseClass *app){
  //assign a unique id to the object
  app->id = nextIDAssignment++;
  app->draw = &draw;
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