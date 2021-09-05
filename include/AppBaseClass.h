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
#include "globaldefs.h"
#include <Arduino.h>

//#include <vector>
#include "HSI.h"
//#include "touch.h"
//#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
#include "ili9341_t3n_font_Arial.h"
#include "AudioDirector.h"

//#include "AppManager.h"

#ifndef _AppBase_
#define _AppBase_

// Application Base Class
//
class AppBaseClass {
  friend class AppManager;
  protected:
    AudioDirector* ad;
    ILI9341_t3_ERIS* draw;
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
      (0==strcmp(name,name_string))?is=true:is=false;
      return is; 
    };
    void setName(const char* name_string){
      if (strlen(name_string) < MAX_NAME_LENGTH - 1){strcpy(name,name_string);
      } else strncpy(name,name_string,MAX_NAME_LENGTH - 1);
    }
    void RegisterApp(AppBaseClass *app);
    void setParent(AppBaseClass *parent){parentNode = parent;};
    void setPosition(int16_t newOriginX, int16_t newOriginY){origin_x=newOriginX;origin_y=newOriginY;_updatePosition();}
    void setDimension(int16_t new_width, int16_t new_height){width=new_width;height=new_height;_updatePosition();}
    void setWidgetPosition(int16_t newOriginX, int16_t newOriginY){widget_origin_x=newOriginX;widget_origin_y=newOriginY;_updatePosition();}
    void setWidgetDimension(int16_t new_width, int16_t new_height){widget_width=new_width;widget_height=new_height;_updatePosition();}
    void getFocus();
    virtual void update(){Serial.println(F("AppBaseClass:update"));};  //will be called only when the app has the screen focus and the screen isnt busy redrawing
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
    virtual void MessageHandler(AppBaseClass *sender, const char *message){};
  protected:
    bool has_focus;
    bool has_pop;
    void returnFocus();
    void requestPopUp();
    void releasePopUp();
    void selfRegister();
  private:
    void _updatePosition();
};

#endif
