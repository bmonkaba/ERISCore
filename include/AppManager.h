#ifndef _AppBase_
#define _AppBase_
#include <Arduino.h>
#include "HSI.h"
#include "touch.h"
#include "ILI9341_t3_ERIS.h"
//#include "font_Arial.h"
#include "AudioDirector.h"

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
    ILI9341_t3_ERIS *_tft;
    bool touch_state;   //set by the app manager
    uint16_t id; //app id for derived class instances
                  // 255 - reserved for the base class (unused)
                  // 254 - dashboard widgets (dashboard active only)
                  // 253 - widgets (always active)
                  // 253 - widgets (active off dashboard)
                  // 1-250 - applications
  public:
    int16_t origin_x;
    int16_t origin_y;
    int16_t width;
    int16_t height;
    char name[16];
    AppBaseClass();
    uint16_t getId(){return id;};
    bool isName(const char * name_string){
      bool is;
      (0==strcmp(name,name_string))?is=true:is=false;
      return is; 
    };
    void setName(const char* name_string){strcpy(name,name_string);}
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
    virtual void MessageHandler(AppBaseClass *sender, const char *message){};
};

class AppManager {
  private:
    static AppManager* obj; //make appManager a singleton
    SdFs sd;
    AppBaseClass *root; //root linked list node
    uint16_t nextIDAssignment;
    uint16_t activeID; //active app
                      //TODO: implement an active id push/pop stack for nesting apps
    AppManager(){ //private constuctor (lazy singleton pattern)
      root = 0;
      nextIDAssignment = 1; //id 0 is reserved
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
    };

  public:
    static AppManager* getInstance() //public access to get a reference to the singleton instance
    {
      if (NULL==obj) {
        obj = new AppManager(); //create the object instance if not yet available
      }
      return obj;
    }

    TS_Point p;
    bool touch_state;

    void switchAppFocus(uint16_t id){activeID=id;}; //switch focus
    void pushAppFocus();  //used by switchAppFocus to store the requesting app for return
    void popAppFocus();  //return to the requesting app
    void peekAppFocus(); //used by apps to find out who called it
    void update(){
      if (root == 0){
        Serial.println("AppManager::update called without an application initalized");
        return;
      }
      touch.update();
      AppBaseClass *node = root;
      bool isactive_child;
      //search the linked list
      do{
        node->updateRT(); //real time update (always called)
        //Serial.println("AppManager:: real time update");
        isactive_child = false;
        if (node->parentNode!=NULL){if(node->parentNode->id == activeID){isactive_child = true;}}; //send event triggers to any child apps
        if (node->id == activeID || isactive_child) {
          //Serial.print("AppManager::updating active application");Serial.println(activeID);
          //active app found - trigger any events and then call the update function
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
          //TODO: after integrating the UI class add automatic wallpaper & control rendering
          node->update(); //update active window
          //return ;//dont return in case multiple apps share the same id (app specific overlay)
                    //update order follows the order of app instance creation
        }
        node=node->nextAppicationNode;//check next node
      }while(node !=0);
      //finally update the screen
      if (!tft.busy()) tft.updateScreen();
    };

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

AppBaseClass::AppBaseClass(){
  //Serial.println("AppBaseClass constructor called");
  parentNode=NULL;          //set by the parent
  nextAppicationNode=NULL;
  previousAppicationNode=NULL;
  touch_state = false;      //set by the application manager
  origin_x=0;
  origin_y=0;
  width=320;
  height=240;
  strcpy(name,"NONE");
  AppManager::getInstance()->RegisterApp(this); //self register on construction
}

#endif
