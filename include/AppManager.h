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
#include "globaldefs.h"
#include <Arduino.h>
#include <vector>
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
    bool redraw_background;
    bool redraw_objects;
    bool redraw_render;
    bool redraw_popup;
    TS_Point p;
    bool touch_state;
    uint16_t appFocusStack[8]; //supports application switching
    uint16_t appPopUpStack[8]; //supports ordered overlay rendering
    uint8_t appFocusStackIndex;
    uint8_t appPopUpStackIndex;

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
