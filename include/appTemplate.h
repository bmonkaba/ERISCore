#include "AppManager.h"
// Basic App Template
//

class AppTemplate:public AppBaseClass {
  public:
    AppTemplate():AppBaseClass(){
        
    }; 
    //define event handlers
    void MessageHandler(AppBaseClass *sender, const char *message){   
        if(sender->isName("SOME APP NAME")){
         //do something
        }
    }
  protected:
    void update() override{};    //called only when the app is active
    void updateRT() override{}; //allways called even if app is not active
    void onFocus() override{};   //called when given focus
    void onFocusLost() override{}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y) override{
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y) override{
    };
};