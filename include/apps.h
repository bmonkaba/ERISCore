#ifndef _AppBase_
#define _AppBase_
#include <Arduino.h>
#include "HSI.h"
#include "touch.h"
#include "ILI9341_t3_ERIS.h"
#include "AudioDirector.h"

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
    AppBaseClass *childNode;
    ILI9341_t3_ERIS *_tft;
    uint16_t id; //app id for derived class instances
                  // 255 - reserved for the base class (unused)
                  // 254 - dashboard widgets (dashboard active only)
                  // 253 - widgets (always active)
                  // 253 - widgets (active off dashboard)
                  // 1-250 - applications
  public:
    AppBaseClass();
    uint16_t getId(){return id;};
    virtual void update(){Serial.println("AppBaseClass:update");};  //will be called only when the app has the screen focus and the screen isnt busy redrawing
    virtual void updateRT(){}; //will be called every loop and prior to a potential update call
    //Event handlers
    virtual void onFocus(){};
    virtual void onFocusLost(){};
    virtual void onTouch(uint16_t x, uint16_t y){};
    virtual void onTouchDrag(uint16_t x, uint16_t y){};
    virtual void onTouchRelease(uint16_t x, uint16_t y){};

};

class AppManager {
  private:
    static AppManager* obj; //make appManager a singleton
    SdFs sd;
    //FsFile file;
    AppBaseClass *root; //root linked list node
    uint16_t nextIDAssignment;
    uint16_t activeID; //active app
                      //TODO: implement an active id push/pop stack for nesting apps
    AppManager(){ //private constuctor (lazy singleton pattern)
      root = 0;
      nextIDAssignment = 1; //id 0 is reserved
      //init the sd card
      if (!sd.begin(SdioConfig(FIFO_SDIO))){
        Serial.println("AppManager: FATAL! SD Card Not Found ");
        sd.initErrorHalt(&Serial);
      } else {Serial.println("AppManager: SD Card FOUND");}
      //init the display
      Serial.println("AppManager: Config display");
      tft.setPWMPin(TFT_LED_PWM); //library will control the backlight
      tft.setSD(&sd); //provide a cd pointer to the sd class
      Serial.println("AppManager: Init display");
      tft.begin();
      Serial.println("AppManager: Init touch controller");
      //init the touch coms
      touch.begin();
      touch_state = 0;
      Serial.println("AppManager: Contructor complete");
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
      //search the linked list
      do{
        node->updateRT(); //real time update (always called)
        //Serial.println("AppManager:: real time update");
        if (node->id == activeID) {
          //Serial.print("AppManager::updating active application");Serial.println(activeID);
          //active app found - trigger any events and then call the update function
          if (touch.touched()) {
            p = touch.getPoint();
            //Serial.print(p.x);Serial.print(" ");Serial.println(p.y);
            if (touch_state == 0){
              node->onTouch(p.x, p.y);
              touch_state=1;
            } else{
              node->onTouchDrag(p.x, p.y);
            }
          } else if (touch_state==1){
            touch_state=0;
            node->onTouchRelease(p.x, p.y);
          }
          //TODO: after integrating the UI class add automatic wallpaper & control rendering
          node->update(); //update active window
          //return ;//dont return in case multiple apps share the same id (app specific overlay)
                    //update order follows the order of app instance creation
        }
        node=node->childNode;//check next node
      }while(node !=0);
    };

    void RegisterApp(AppBaseClass *app){
      //assign a unique id to the object
      app->id = nextIDAssignment++;
      if (root == 0) {root = app; return;}
      else{
        AppBaseClass *endNode = root;
        //add to the linked list
        while(endNode->childNode !=0){endNode=endNode->childNode;}
        endNode->childNode = app;
        app->parentNode = endNode;
      }
    };
};

AppManager* AppManager::obj = 0; // or NULL, or nullptr in c++11

AppBaseClass::AppBaseClass(){
  Serial.println("AppBaseClass constructor called");
  AppManager::getInstance()->RegisterApp(this); //self register on construction
  parentNode=0;
  childNode=0;
}

#endif
