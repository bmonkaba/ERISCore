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

uint16_t DMAMEM FB1[320 * 240]__attribute__ ((aligned (32)));
//DMAMEM uint16_t FB2[320 * 240]__attribute__ ((aligned (32)));

SvcDataDictionary _data;


AppManager::AppManager(){ //private constuctor (lazy singleton pattern)
  data = &_data;
  root = 0;
  activeID = 0;
  pActiveApp = 0;
  nextIDAssignment = 1; //id 0 is reserved
  monitor_dd_update_timer = 0;
  redraw_background=false;
  redraw_objects=true;
  redraw_render=false;
  redraw_popup=false;
  //init the app focus and app stacks
  memset(&appFocusStack,0,sizeof(appFocusStack));
  memset(&appPopUpStack,0,sizeof(appPopUpStack));
  appFocusStackIndex = 0;
  appPopUpStackIndex = 0;

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
  animated_wallpaper.setPath("/V/POINTSOLIGHT");
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
    uint32_t heapTop;
    void* htop;
    uint32_t drt;
    bool touch_updated;
    bool monitor_update = (monitor_dd_update_timer > APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC);
    cycle_time=0;
    #ifdef ENABLE_ASYNC_SCREEN_UPDATES
    //bool screenBusy;
    //screenBusy = (tft.busy()|| redraw_objects==true || redraw_background==true || redraw_popup ==true || redraw_render==true);
    
    //update the screen background
    drt = display_refresh_time;
    if (!draw.busy() && redraw_background==true && (drt > DISPLAY_UPDATE_PERIOD)){
        data->update("RENDER",0);
      //tft.bltSDFullScreen("bluehex.ile");
      //tft.fillRect(0, 0, 320, 240, 7565);
      if (animated_wallpaper.getNextFrameChunk(&sd)){
          draw.bltSDAnimationFullScreen(&animated_wallpaper);   
          //Serial.printf(F("AppManager::bltSDAnimation Chunk %d\n"),t); 
      }
    }
    
    #else
    if (!screenBusy) draw.updateScreen();
    #endif

    elapsedMicros app_time=0;
    if (root == 0){
      Serial.println(F("AppManager::update called without an application initalized"));
      return;
    }
    if (!draw.busy()){
      touch.update(); //wait until draw is complete in case of dma 
      touch_updated = true;
    } else touch_updated = false;
    bool update_analog = analog.update();
    AppBaseClass *node = root;
    bool isactive_child;
    #ifdef ENABLE_ASYNC_SCREEN_UPDATES
    //if (!screenBusy) draw.fillRect(0, 0, 320, 240, 0);//draw.bltSDFullScreen("bluehex.ile");//draw.fillRect(0, 0, 320, 20, 0);
    #else
    //if (!screenBusy) draw.bltSDFullScreen("bluehex.ile");
    #endif
    //search the linked list
    
    do{
      app_time=0;
      if (node->updateRT_call_period > node->updateRT_call_period_max) node->updateRT_call_period_max = node->updateRT_call_period;
      //Serial.printf("%s_RT ",node->name);
      //Serial.flush();
      node->updateRT(); //real time update (always called)
      Serial.flush();
      node->updateRT_loop_time = app_time;
      if (node->updateRT_loop_time > node->updateRT_loop_time_max) node->updateRT_loop_time_max = node->updateRT_loop_time;
      node->updateRT_call_period =0;
      //Serial.println("AppManager:: real time update");
      isactive_child = false;
      if (node->id == activeID) {
          if (pActiveApp != node){
            if (pActiveApp != 0) pActiveApp->onFocusLost();
            node->onFocus();
            pActiveApp = node;
          }
      }
      if (node->parentNode!=NULL){if(node->parentNode->id == activeID){isactive_child = true;}}; //send event triggers to any child apps
      if (node->id == activeID || isactive_child) {
          //Serial.print("AppManager::updating active application");Serial.println(activeID);
          //active app found - trigger any events and then call the update function
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
          if (redraw_objects){
            app_time=0;
            if (node->update_call_period > node->update_call_period_max) node->update_call_period_max = node->update_call_period;
            //Serial.printf("%s ",node->name);
            node->update(); //update active window
            Serial.flush();
            node->update_loop_time = app_time;
            if (node->update_loop_time > node->update_loop_time_max){
                node->update_loop_time_max = node->update_loop_time;
                //update the data dictionary
                data->update(node->name,node->update_loop_time_max); 
            }
            node->update_call_period =0;
          }
          //return ;//dont return in case multiple apps share the same id (app specific overlay)
                  //update order follows the order of app instance creation
      }

      #ifdef SERIAL_PRINT_APP_LOOP_TIME
      Serial.print(node->name);
      Serial.print(F(":"));
      Serial.print(cycle_time);
      Serial.print(F(" "));
      #endif
      node->cycle_time = cycle_time;
      if (node->cycle_time > node->cycle_time_max){
          node->cycle_time_max = node->cycle_time;
      }
      node=node->nextAppicationNode;//check next node
    }while(node !=0);

    if(redraw_popup==true){
      AppBaseClass* a;
      //draw any popups
      if (!appPopUpStackIndex==0 && appPopUpStack[appPopUpStackIndex-1] != 0){
        a = getApp(appPopUpStack[appPopUpStackIndex-1]);
        if (a != 0){
          //Serial.println(F("AppManager::redraw_popup"));
          a->update();
        }
      }
      redraw_popup = false;
      redraw_render = true;
    } else if (redraw_background==true && (data->read("RENDER") == 0) && animated_wallpaper.isFrameComplete()){
      //Serial.printf(F("AppManager::isFrameComplete %d\n"),drt);
      data->update("RENDER",1);
      redraw_objects = true;
      redraw_background=false;
      //redraw_render = true;
    } else if (redraw_render == true){
        //Serial.printf(F("AppManager::Rendering %d\n"),drt);
        data->update("RENDER_PERIOD",drt);
        data->update("RENDER",4);
        data->increment("RENDER_FRAME");
        Serial.flush();
        draw.updateScreenAsync(false);//updateScreenAsyncFrom(&draw,false);
        redraw_render = false;
        redraw_background=true;
        //redraw_objects = true;
        display_refresh_time = 0;
    }else if(redraw_popup==true ){
      data->update("RENDER",3);
      redraw_popup = false;
      redraw_render = true;          
    }else if (redraw_objects==true ){
      //Serial.printf(F("AppManager::ObjDrawComplete %d\n"),drt);
      data->update("RENDER",2);
      redraw_objects = false;
      redraw_popup = true;
      //redraw_render = true;
    }

    #ifdef SERIAL_PRINT_APP_LOOP_TIME
    Serial.println();
    #endif

    if (cycle_time > cycle_time_max) cycle_time_max = cycle_time;
    
    if(monitor_update){
      monitor_dd_update_timer = 0;
      data->update("LOOP_TIME",cycle_time);
      data->update("FRAME_PTR1",(int32_t)draw.getFrameAddress());
      //data->update("FRAME_PTR2",(int32_t)render.getFrameAddress()); 
      cycle_time_max=0;
      htop = malloc(100);
      heapTop = (uint32_t) htop;
      free(htop);
      data->update("HEAP_FREE",0x20280000 - heapTop);
      data->update("LOCAL_MEM",0X7FFFF - (uint32_t)(&heapTop) - 0x20000000);
      
      
      //Serial.printf("heap pointer: %08XH\n",heapTop);
      //Serial.printf("heap free: %d bytes\n",0x20280000 - heapTop);
    }
    drt = display_refresh_time;
    //Serial.printf(" %u\n",drt);
    Serial.flush();

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
    }while(node !=0);
    return NULL;
}

bool AppManager::requestPopUp(uint16_t id,bool exclusive){
  if (appPopUpStackIndex == 8) return false;
  appPopUpStack[appPopUpStackIndex] = id;
  appPopUpStackIndex+=1;
  return true;
}
bool AppManager::releasePopUp(){
  if (appFocusStackIndex == 0)return false;
  //appPopUpStack[appPopUpStack.size()-1]=0;
  appPopUpStackIndex-=1;
  appPopUpStack[appPopUpStackIndex] = 0;
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
  }while(node !=0);
  return false;
}
void AppManager::printStats(){
  AppBaseClass *node = root;
  do{
    Serial.flush();
    Serial.print(node->name);
    //Serial.print("\tupdate_loop_time: ");Serial.print(node->update_loop_time);
    Serial.print(F("\n&emsp;update_loop_time_max: "));Serial.print(node->update_loop_time_max);
    //Serial.print("\tupdateRT_loop_time: ");Serial.print(node->updateRT_loop_time);
    Serial.print(F("\n&emsp;updateRT_loop_time_max: "));Serial.print(node->updateRT_loop_time_max);
    //Serial.print("\tcycle_time: ");Serial.print(node->cycle_time);
    Serial.print(F("\n&emsp;cycle_time_max: "));Serial.print(node->cycle_time_max);
    //Serial.print("\tupdate_call_period: ");Serial.print(node->update_call_period);
    Serial.print(F("\n&emsp;update_call_period_max: "));Serial.print(node->update_call_period_max);
    Serial.print(F("\n&emsp;updateRT_call_period_max: "));Serial.print(node->updateRT_call_period_max);
    Serial.println(F("\n-------------------------------"));
    //clear the stats
    node->update_loop_time_max = 0;
    node->updateRT_loop_time_max = 0;
    node->cycle_time_max = 0;
    node->update_call_period_max = 0;
    node->updateRT_call_period_max = 0;
  
    node=node->nextAppicationNode;//check next node
  }while(node !=0);
  //print app manager stats
  Serial.println(F("ApplicationManager"));
  Serial.print(F("\n&emsp;cycle_time: "));Serial.print(cycle_time);
  Serial.print(F("\n&emsp;cycle_time_max: "));Serial.println(cycle_time_max);
  //clear the stats
  cycle_time_max = 0;
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