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
#include <Arduino.h>
#include "globaldefs.h"
#include "HSI.h"
#include "touch.h"
#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
//#include "font_Arial.h"
#include "AudioDirector.h"
#include "AppBaseClass.h"
#include "svcDataDictionary.h"
#ifndef _AppManager_
#define _AppManager_

//test variable (serial client remote modification)
extern short g_octave_down_shift;

enum loopState{
  redraw_wait,
  redraw_background,
  redraw_objects,
  redraw_render,
  redraw_popup,
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
    Animation animated_wallpaper;
    loopState state;
    TS_Point p;
    bool touch_state;
    uint16_t appFocusStack[8]; //supports application switching
    uint16_t appPopUpStack[8]; //supports ordered overlay rendering
    uint8_t appFocusStackIndex;
    uint8_t appPopUpStackIndex;
    bool exclusive_app_render;

                      //TODO: implement an active id push/pop stack for nesting apps
    AppManager();

  public:
    SvcDataDictionary* data;//data dictionary service;
    static AppManager* getInstance() //public access to get a reference to the singleton instance
    {
      if (NULL==obj) {
        obj = new AppManager(); //create the object instance if not yet available
      }
      return obj;
    }
    void update();
    SdFs* getSD();
    bool chdir(){return sd.chdir();};
    bool chdir(const char* path){return sd.chdir(path);};
    bool ls(){return sd.ls();};
    AppBaseClass* getApp(uint16_t id);
    bool requestPopUp(uint16_t id,bool exclusive=false);
    bool releasePopUp();
    bool getFocus(uint16_t id);
    bool returnFocus();
    uint16_t peekAppFocus();//used by apps to find out which has focus
    AppBaseClass* getActiveApp();
    bool sendMessage(AppBaseClass *sender, const char *to_app, const char *message);
    void printStats();
    void RegisterApp(AppBaseClass *app);
};

#endif
