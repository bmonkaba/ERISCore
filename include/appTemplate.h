/**
 * @file appTemplate.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"
// Basic App Template
//

class AppTemplate:public AppBaseClass {
  public:
    AppTemplate():AppBaseClass(){
        
    }; 
    //define event handlers
    void messageHandler(AppBaseClass *sender, const char *message){   
        if(sender->isName("SOME APP NAME")){
         //do something
        }
    }
  protected:
    void render() override{};    //called only when the app is active
    void update() override{}; //allways called even if app is not active
    void onFocus() override{};   //called when given focus
    void onFocusLost() override{}; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{};
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{};
    void onAnalog1(uint16_t uval, float fval) override{};
    void onAnalog2(uint16_t uval, float fval) override{};
    void onAnalog3(uint16_t uval, float fval) override{};
    void onAnalog4(uint16_t uval, float fval) override{};
};