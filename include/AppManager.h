/**
 * @file AppManager.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _AppBase_
#define _AppBase_

#define MAX_NAME_LENGTH 24
#define ENABLE_ASYNC_SCREEN_UPDATES
#define DISPLAY_UPDATE_PERIOD 20
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 500 
//#define SERIAL_PRINT_APP_LOOP_TIME

#include <Arduino.h>
#include <deque>
#include "HSI.h"
#include "touch.h"
#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
//#include "font_Arial.h"
#include "AudioDirector.h"
#include "svcDataDictionary.h"

extern const ILI9341_t3_font_t Arial_9;

class AppManager;

ILI9341_t3_ERIS tft(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);
Touch touch(CS_TOUCH);
AudioDirector ad;

// Application Base Class
//
class AppBaseClass {
  friend class AppManager;
  protected:
    AppBaseClass *parentNode;
    AppBaseClass *previousAppicationNode;
    AppBaseClass *nextAppicationNode;
    bool touch_state;   //set by the app manager
    uint16_t id; //app id for derived class instances
                  // 65000 - reserved for the base class (unused)
                  // 35000 - dashboard widgets (dashboard active only)
                  // 253 - widgets (always active)
                  // 253 - widgets (active off dashboard)
                  // 1-250 - applications
    uint16_t update_loop_time;
    uint16_t update_loop_time_max;
    uint16_t updateRT_loop_time;
    uint16_t updateRT_loop_time_max;
    uint16_t cycle_time;
    uint16_t cycle_time_max;
    elapsedMicros update_call_period;
    uint16_t update_call_period_max;
    elapsedMicros updateRT_call_period;
    uint16_t updateRT_call_period_max;
  public:
    int16_t origin_x;
    int16_t origin_y;
    int16_t width;
    int16_t height;
    char name[MAX_NAME_LENGTH];
    AppBaseClass();
    AppBaseClass(const AppBaseClass &) = delete;	//delete the copy constructor
    uint16_t getId(){return id;};
    bool isName(const char * name_string){
      bool is;
      (0==strcmp(name,name_string))?is=true:is=false;
      return is; 
    };
    void setName(const char* name_string){
      if (strlen(name_string) < MAX_NAME_LENGTH - 1){strcpy(name,name_string);
      } else strncpy(name,name_string,MAX_NAME_LENGTH - 1);
    }
    void setParent(AppBaseClass *parent){parentNode = parent;};
    void setPosition(int16_t newOriginX, int16_t newOriginY){origin_x=newOriginX;origin_y=newOriginY;}
    void setDimension(int16_t new_width, int16_t new_height){width=new_width;height=new_height;}
    virtual void update(){Serial.println(F("AppBaseClass:update"));};  //will be called only when the app has the screen focus and the screen isnt busy redrawing
    virtual void updateRT(){}; //will be called every loop and prior to a potential update call
    //Event handlers
    virtual void onFocus(){};
    virtual void onFocusLost(){};
    virtual void onTouch(uint16_t x, uint16_t y){};
    virtual void onTouchDrag(uint16_t x, uint16_t y){};
    virtual void onTouchRelease(uint16_t x, uint16_t y){};
    virtual void onAnalog1(uint16_t uval, float fval){};
    virtual void onAnalog2(uint16_t uval, float fval){};
    virtual void onAnalog3(uint16_t uval, float fval){};
    virtual void onAnalog4(uint16_t uval, float fval){};
  public:
    virtual void MessageHandler(AppBaseClass *sender, const char *message){};
};

class AppManager {
  private:
    static AppManager* obj; //make appManager a singleton
    SdFs sd;
    AnalogInputs analog;
    AppBaseClass *root; //root linked list node
    AppBaseClass *pActiveApp; //active app
    uint16_t nextIDAssignment;
    uint16_t activeID; //active app
    uint16_t cycle_time_max;
    elapsedMillis display_refresh_time;
    elapsedMicros cycle_time;
    elapsedMillis monitor_dd_update_timer;
    bool redraw_background;
    bool redraw_objects;

                      //TODO: implement an active id push/pop stack for nesting apps
    AppManager(){ //private constuctor (lazy singleton pattern)
      root = 0;
      activeID = 0;
      pActiveApp = 0;
      nextIDAssignment = 1; //id 0 is reserved
      monitor_dd_update_timer = 0;
      redraw_background=true;
      redraw_objects=true;
      //init the appStack
      // allocate an array with space for 5 elements using deque's allocator:
      unsigned int *p;
      p = appFocusStack.get_allocator().allocate(16);
      // construct values in-place on the array:
      for (int8_t i=0; i<16; i++) appFocusStack.get_allocator().construct(&p[i],i);
      //init the sd card
      if (!sd.begin(SdioConfig(FIFO_SDIO))){
        Serial.println(F("AppManager: FATAL! SD Card Not Found "));
        sd.initErrorHalt(&Serial);
      } else {Serial.println(F("AppManager: SD Card FOUND"));}
      //init the display
      Serial.println(F("AppManager: Config display"));
      tft.setPWMPin(TFT_LED_PWM); //library will control the backlight
      tft.setSD(&sd); //provide a cd pointer to the sd class
      Serial.println(F("AppManager: Init display"));
      tft.begin();
      Serial.println(F("AppManager: Init touch controller"));
      //init the touch coms
      touch.begin();
      touch_state = 0;
      Serial.println(F("AppManager: Contructor complete"));
      #ifdef ENABLE_ASYNC_SCREEN_UPDATES
      tft.updateScreenAsync(false);
      display_refresh_time = 0;
      #endif
    };

  public:
    static AppManager* getInstance() //public access to get a reference to the singleton instance
    {
      if (NULL==obj) {
        obj = new AppManager(); //create the object instance if not yet available
      }
      return obj;
    }

    void update(){
        bool monitor_update = (monitor_dd_update_timer > APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC);

        cycle_time=0;
        #ifdef ENABLE_ASYNC_SCREEN_UPDATES
        bool screenBusy;
        screenBusy = (tft.busy() || redraw_objects==true || redraw_background==true);
        
        
        //finally update the screen
        #ifdef ENABLE_ASYNC_SCREEN_UPDATES
        if (redraw_background==true){
          redraw_background=false;
          //background was updated this loop.. on the next loop draw the objects
          //tft.bltSDFullScreen("bluehex.ile");
          tft.fillRect(0, 0, 320, 240, 0);
          redraw_objects = true;
        }
        #else
        if (!screenBusy) tft.updateScreen();
        #endif
        
        
        if (!screenBusy){
          if (display_refresh_time > DISPLAY_UPDATE_PERIOD){
            tft.updateScreenAsync(false);
            redraw_background=true;
            display_refresh_time = 0;
          }
        }

        #else
        if (!screenBusy) tft.updateScreen();
        #endif

        elapsedMicros app_time=0;
        if (root == 0){
          Serial.println(F("AppManager::update called without an application initalized"));
          return;
        }
        touch.update();
        bool update_analog = analog.update();
        AppBaseClass *node = root;
        bool isactive_child;
        #ifdef ENABLE_ASYNC_SCREEN_UPDATES
        //if (!screenBusy) tft.fillRect(0, 0, 320, 240, 0);//tft.bltSDFullScreen("bluehex.ile");//tft.fillRect(0, 0, 320, 20, 0);
        #else
        //if (!screenBusy) tft.bltSDFullScreen("bluehex.ile");
        #endif
        //search the linked list
        cycle_time = 0;
        do{
          app_time=0;
          if (node->updateRT_call_period > node->updateRT_call_period_max) node->updateRT_call_period_max = node->updateRT_call_period;
          node->updateRT(); //real time update (always called)
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
                data.update("AN1",analog.readAN1());
                data.update("AN2",analog.readAN2());
                data.update("AN3",analog.readAN3());
                data.update("AN4",analog.readAN4());
                
                node->onAnalog1(analog.readAN1(),analog.freadAN1());
                node->onAnalog2(analog.readAN2(),analog.freadAN2());
                node->onAnalog3(analog.readAN3(),analog.freadAN3());
                node->onAnalog4(analog.readAN4(),analog.freadAN4());
              }
              if (touch.touched()) {
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
                node->update(); //update active window
                node->update_loop_time = app_time;
                if (node->update_loop_time > node->update_loop_time_max){
                    node->update_loop_time_max = node->update_loop_time;
                    //update the data dictionary
                    data.update(node->name,node->update_loop_time_max); 
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
        redraw_objects = false;
        #ifdef SERIAL_PRINT_APP_LOOP_TIME
        Serial.println();
        #endif

        if (cycle_time > cycle_time_max) cycle_time_max = cycle_time;
        
        if(monitor_update){
        monitor_dd_update_timer = 0;
        data.update("LOOP_TIME_MAX",cycle_time_max);  
        cycle_time_max=0;
        }
    };

    SdFs* getSD(){
      return &sd;
    }
    
    TS_Point p;
    bool touch_state;
    SvcDataDictionary data;//data dictionary service
    std::deque<unsigned int> appFocusStack;
    void getFocus(uint16_t id){
      pushAppFocus(activeID);
      activeID=id;
    } //switch focus
    void returnFocus(){popAppFocus();} //alias for popAppFocus
  private:
    void pushAppFocus(uint16_t app_id){appFocusStack.push_front(app_id);}  //used by switchAppFocus to store the requesting app for return
    void popAppFocus(){
      activeID=appFocusStack.front();//load from the stack
      appFocusStack.pop_front();//then remove
    }  //return to the requesting app
  public:
    uint16_t peekAppFocus(){return activeID;}//used by apps to find out which has focus
    AppBaseClass* getActiveApp(){
      return pActiveApp;
    }
    bool sendMessage(AppBaseClass *sender, const char *to_app, const char *message){
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
    void printStats(){
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


    void RegisterApp(AppBaseClass *app){
      //assign a unique id to the object
      app->id = nextIDAssignment++;
      if (root == 0) {root = app; return;}
      else{
        AppBaseClass *endNode = root;
        //add to the linked list
        while(endNode->nextAppicationNode !=0){endNode=endNode->nextAppicationNode;}
        endNode->nextAppicationNode = app;
        app->previousAppicationNode = endNode;
      }
    };
};

AppManager* AppManager::obj = 0; // or NULL, or nullptr in c++11

AppBaseClass:: AppBaseClass(){
  //Serial.println("AppBaseClass constructor called");
  parentNode=NULL;          //set by the parent
  nextAppicationNode=NULL;
  previousAppicationNode=NULL;
  touch_state = false;      //set by the application manager
  origin_x=0;
  origin_y=0;
  width=320;
  height=240;
  update_loop_time =0;
  update_loop_time_max =0;
  updateRT_loop_time =0;
  updateRT_loop_time_max =0;
  cycle_time =0;
  cycle_time_max =0;
  update_call_period =0;
  update_call_period_max =0;
  updateRT_call_period =0;
  updateRT_call_period_max =0;
  strcpy(name,"NONE");
  AppManager::getInstance()->RegisterApp(this); //self register on construction
}

#endif
