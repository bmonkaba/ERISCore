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

#include "Eris.h"
#include <Arduino.h>
#include <elapsedMillis.h>
#include "HSI.h"
#include "ILI9341_t3_ERIS.h"
#include "ili9341_t3n_font_Arial.h"
#include "AudioDirector.h"
#include "ErisUtils.h"

#ifndef _AppBase_
#define _AppBase_

//forward declaration of the AppManager class to be defined later

class AppManager;
class AudioDirector;
class SvcSerialCommandInterface;

/**
 * @brief base class definition / implementation from which all app classes will be derived and override
 * 
 */
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
    uint16_t update_priority;
    uint16_t update_priority_counter;
    uint16_t render_cpu_time;
    uint16_t render_cpu_time_max;
    uint16_t update_cpu_time;
    uint16_t update_cpu_time_max;
    uint16_t cycle_time;
    uint16_t cycle_time_max;
    elapsedMicros et_render_period;
    uint16_t render_period_max;
    elapsedMicros et_update_period;
    uint16_t update_period_max;
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
    ~AppBaseClass();

    /**
     * @brief Get the ID object
     * 
     * @return uint16_t 
     */
    uint16_t getID(){return id;};

    /**
     * @brief Compares the name_string to the app class instance name (string)
     * 
     * @param name_string 
     * @return true 
     * @return false 
     */
    bool isName(const char * name_string){
      bool is;
      (0==strncmp(name,name_string,sizeof(name)))?is=true:is=false;
      return is; 
    };

    /**
     * @brief Set the app class instance name (string)
     * 
     * @param name_string 
     */
    void setName(const char* name_string){
      if (strlen(name_string) < MAX_NAME_LENGTH - 1){strcpy(name,name_string);
      } else safer_strncpy(name,name_string,MAX_NAME_LENGTH - 1);
    }

    /**
     * @brief registers the app instance with the AppManager
     * 
     * @param app 
     */
    void registerApp(AppBaseClass *app);
    
    /**
     * @brief Set the Parent object
     * 
     * @param parent 
     */
    void setParent(AppBaseClass *parent){parent_node = parent;};
    
    /**
     * @brief Set the Position object
     * 
     * @param new_origin_x 
     * @param new_origin_y 
     */
    void setPosition(int16_t new_origin_x, int16_t new_origin_y){origin_x=new_origin_x;origin_y=new_origin_y;_updatePosition();}
    
    /**
     * @brief Set the Dimension object
     * 
     * @param new_width 
     * @param new_height 
     */
    void setDimension(int16_t new_width, int16_t new_height){width=new_width;height=new_height;_updatePosition();}

    /**
     * @brief Set the Widget Position object
     * 
     * @param new_origin_x 
     * @param new_origin_y 
     */
    void setWidgetPosition(int16_t new_origin_x, int16_t new_origin_y){widget_origin_x=new_origin_x;widget_origin_y=new_origin_y;_updatePosition();}

    /**
     * @brief Set the Widget Dimension object
     * 
     * @param new_width 
     * @param new_height 
     */
    void setWidgetDimension(int16_t new_width, int16_t new_height){widget_width=new_width;widget_height=new_height;_updatePosition();}

    /**
     * @brief Get focus from the AppManager
     * 
     */
    void getFocus();

    /**
     * @brief render loop
     * 
     */
    virtual void render(){};  //will be called only when the app has the screen focus and the screen isnt busy redrawing

    /**
     * @brief update loop
     * 
     */
    virtual void update(){}; //will be called every loop and prior to a potential update call

    //Event handlers
    /**
     * @brief Event handler
     * called when the app gains focus
     */
    virtual void onFocus(){};

    /**
     * @brief Event handler
     * called when the app loses focus
     */
    virtual void onFocusLost(){};

    /**
     * @brief Event handler
     * called on touch
     * @param t_x 
     * @param t_y 
     */
    virtual void onTouch(uint16_t t_x, uint16_t t_y){};

    /**
     * @brief Event handler for touch drag
     * 
     * @param t_x 
     * @param t_y 
     */

    virtual void onTouchDrag(uint16_t t_x, uint16_t t_y){};
    /**
     * @brief Event handler for touch release
     * 
     * @param t_x 
     * @param t_y 
     */
    virtual void onTouchRelease(uint16_t t_x, uint16_t t_y){};

    /**
     * @brief Event handler for analog 1 event
     * 
     * @param uval 
     * @param fval 
     */
    virtual void onAnalog1(uint16_t uval, float fval){};

    /**
     * @brief Event handler for analog 2 event
     * 
     * @param uval 
     * @param fval 
     */
    virtual void onAnalog2(uint16_t uval, float fval){};

    /**
     * @brief Event handler for analog 3 event
     * 
     * @param uval 
     * @param fval 
     */
    virtual void onAnalog3(uint16_t uval, float fval){};

    /**
     * @brief Event handler for analog 4 event
     * 
     * @param uval 
     * @param fval 
     */
    virtual void onAnalog4(uint16_t uval, float fval){};

    /**
     * @brief receiver method for inter-app string based communication
     * 
     * @param uval 
     * @param fval 
     */
    virtual void messageHandler(AppBaseClass *sender, const char *message){};

  protected:
    bool has_focus;
    bool has_pop;
    /**
     * @brief give up focus
     * 
     */
    void returnFocus();

    /**
     * @brief request popup from the AppManager\n
     * will be activated by the next render loop  
     * applications in popup mode are guaranteed to be rendered last; preventing any overdraw\n
     * exclusive mode option blocks the render calls of all other apps by the AppManager usefull\n 
     * for taking over full screen rendering while allowing the apps update loops to continue.
     * @param exclusive 
     */
    void requestPopUp(bool exclusive=false);

    /**
     * @brief gives up popup
     * 
     */
    void releasePopUp();

    /**
     * @brief Set the update priority\n 
     * the level sets the number of update opportunities before the update method is actually called
     *  
     * @param level 
     */
    void setUpdatePriority(uint16_t level);
    
    /**
     * @brief self register the instance with the AppManager
     * 
     */
    void selfRegister();
    
    void _updatePosition();
};

#endif
