#include "AppManager.h"
// Slider 
//

class AppSlider:public AppBaseClass {
  public:
    AppSlider():AppBaseClass(){
        isPressed = false;
        strcpy(text,"NONE");
        strcpy(name,"NONE");
    }; 
    //define event handlers
    int16_t value;
    char text[16];
  private:
    uint16_t last_x;
  protected:
    bool isPressed;
    void update(){
        int16_t marker = width*(value/100.0);
        //tft.drawFastVLine(marker,origin_y,height,ILI9341_MAGENTA);
        tft.fillRoundRect(origin_x, origin_y, marker, height,4, CL(36, 54, 84));
        tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
        tft.setTextColor(CL(0,0,0));
        tft.setCursor(origin_x+(width/2)-1,origin_y+(height/2),true);
        tft.print(text);
        tft.setCursor(origin_x+(width/2)+1,origin_y+(height/2),true);
        tft.print(text);
        tft.setCursor(origin_x+(width/2),origin_y+(height/2)-1,true);
        tft.print(text);
        tft.setCursor(origin_x+(width/2),origin_y+(height/2)+1,true);
        tft.print(text);
        tft.setCursor(origin_x+(width/2)+1,origin_y+(height/2)+1,true);
        tft.print(text);
        tft.setCursor(origin_x+(width/2)-1,origin_y+(height/2)-1,true);
        tft.print(text);
        tft.setTextColor(CL(74, 143, 255));
        tft.setCursor(origin_x+(width/2),origin_y+(height/2),true);
        tft.print(text);
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t x, uint16_t y){
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            isPressed = true;
            last_x = x;
        }
    };

    void onTouchDrag(uint16_t x, uint16_t y){
        if(isPressed){
            value += ((x-last_x)/2);
            if (value>100)value = 100;
            if(value <0)value=0;
            if (last_x != x) parentNode->MessageHandler(this,"Changed");
            last_x = x;
        }
    }

    void onTouchRelease(uint16_t x, uint16_t y){
        if (isPressed && x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            parentNode->MessageHandler(this,"Set");
        }
        isPressed = false;
        last_x = x;
    };
};