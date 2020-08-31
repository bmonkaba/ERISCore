#include <core_pins.h>
#include "AppManager.h"
// Button 
//

class AppButton:public AppBaseClass {
  public:
    bool isPressed;
    AppButton():AppBaseClass(){
        isPressed = false;
    }; 
    //define event handlers
    void update(){
        if (isPressed==true){
            tft.bltSD("/I/U/W","greenhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_GREENYELLOW);
        } else{
            tft.bltSD("/I/U/W","redhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
        }
    };
    void onFocus(){};
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t x, uint16_t y){
        Serial.println("MyButton:onTouch");
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            Serial.println("MyButton:onTouch Button Pressed");
            isPressed = true;
        }
    };
    void onTouchDrag(uint16_t x, uint16_t y){};
    void onTouchRelease(uint16_t x, uint16_t y){
        isPressed = false;
    };
};