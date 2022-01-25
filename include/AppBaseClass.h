/**
 * @file AppBaseClass.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */
//#define ENABLE_ASYNC_SCREEN_UPDATES
//#define DISPLAY_UPDATE_PERIOD 40
//#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 500 
//#define SERIAL_PRINT_APP_LOOP_TIME
#include "ErisGlobals.h"
#include <Arduino.h>
#include <elapsedMillis.h>
//#include <vector>
#include "HSI.h"
//#include "touch.h"
//#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
#include "ili9341_t3n_font_Arial.h"
#include "AudioDirector.h"
#include "ErisUtils.h"

//#include "AppManager.h"

#ifndef _AppBase_
#define _AppBase_

//forward declaration of the AppManager class to be defined later
class AppManager;
class AudioDirector;
class SvcSerialCommandInterface;

// Application Base Class
//
class AppBaseClass {
  friend class AppManager;
  protected:
    AudioDirector* ad;
    AppManager* am;
    SvcSerialCommandInterface* sci;
    SdFs* sd;
    ILI9341_t3_ERIS* draw;
    AppBaseClass *parent_node;
    AppBaseClass *previous_app_node;
    AppBaseClass *next_app_node;
    bool touch_state;   //set by the app manager
    uint16_t id; //app id for derived class instances
                  // 65000 - reserved for the base class (unused)
                  // 35000 - dashboard widgets (dashboard active only)
                  // 253 - widgets (always active)
                  // 253 - widgets (active off dashboard)
                  // 1-250 - applications
    uint16_t updateRT_priority;
    uint16_t updateRT_priority_counter;
    uint16_t update_cpu_time;
    uint16_t update_cpu_time_max;
    uint16_t updateRT_cpu_time;
    uint16_t updateRT_cpu_time_max;
    uint16_t cycle_time;
    uint16_t cycle_time_max;
    elapsedMicros update_period;
    uint16_t update_period_max;
    elapsedMicros updateRT_period;
    uint16_t updateRT_period_max;
  public:
    int16_t origin_x;
    int16_t origin_y;
    int16_t width;
    int16_t height;
    int16_t widget_origin_x;
    int16_t widget_origin_y;
    int16_t widget_width;
    int16_t widget_height;
    int16_t x,y,w,h;
    char name[MAX_NAME_LENGTH];
    AppBaseClass();
    AppBaseClass(const AppBaseClass &) = delete;	//delete the copy constructor
    uint16_t getId(){return id;};
    bool isName(const char * name_string){
      bool is;
      (0==strncmp(name,name_string,sizeof(name)))?is=true:is=false;
      return is; 
    };
    void setName(const char* name_string){
      if (strlen(name_string) < MAX_NAME_LENGTH - 1){strcpy(name,name_string);
      } else safer_strncpy(name,name_string,MAX_NAME_LENGTH - 1);
    }
    void registerApp(AppBaseClass *app);
    void setParent(AppBaseClass *parent){parent_node = parent;};
    void setPosition(int16_t newOriginX, int16_t newOriginY){origin_x=newOriginX;origin_y=newOriginY;_updatePosition();}
    void setDimension(int16_t new_width, int16_t new_height){width=new_width;height=new_height;_updatePosition();}
    void setWidgetPosition(int16_t newOriginX, int16_t newOriginY){widget_origin_x=newOriginX;widget_origin_y=newOriginY;_updatePosition();}
    void setWidgetDimension(int16_t new_width, int16_t new_height){widget_width=new_width;widget_height=new_height;_updatePosition();}
    void getFocus();
    virtual void update(){};  //will be called only when the app has the screen focus and the screen isnt busy redrawing
    virtual void updateRT(){}; //will be called every loop and prior to a potential update call
    //Event handlers
    virtual void onFocus(){};
    virtual void onFocusLost(){};
    virtual void onTouch(uint16_t t_x, uint16_t t_y){};
    virtual void onTouchDrag(uint16_t t_x, uint16_t t_y){};
    virtual void onTouchRelease(uint16_t t_x, uint16_t t_y){};
    virtual void onAnalog1(uint16_t uval, float fval){};
    virtual void onAnalog2(uint16_t uval, float fval){};
    virtual void onAnalog3(uint16_t uval, float fval){};
    virtual void onAnalog4(uint16_t uval, float fval){};
    virtual void messageHandler(AppBaseClass *sender, const char *message){};
  protected:
    bool has_focus;
    bool has_pop;
    void returnFocus();
    void requestPopUp(bool exclusive=false);
    void releasePopUp();
    void setRTPriority(uint16_t level);
    void selfRegister();
    void _updatePosition();
};

#endif
