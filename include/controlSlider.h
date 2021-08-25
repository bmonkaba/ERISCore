/**
 * @file controlSlider.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"

#define MAX_TEXT_LENGTH 16
// Slider 
//

class ControlSlider:public AppBaseClass {
  public:
    ControlSlider(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        isPressed = false;
        strcpy(text,"ControlSlider");
        strcpy(name,"ControlSlider");
        last_x = 0;
        value = 0;
    }; 
    int16_t getValue(){return value;}
    void setValue(int16_t new_value){value = new_value;}
    void setText(const char* name_string){
      if (strlen(name_string) < MAX_TEXT_LENGTH - 1){strcpy(text,name_string);
      } else strncpy(text,name_string,MAX_TEXT_LENGTH - 1);
    }
    //define event handlers
    int16_t value;
    char text[MAX_TEXT_LENGTH];
  private:
    uint16_t last_x;
  protected:
    bool isPressed;
    void update(){
        int16_t marker = w*(value/100.0);
        //tft.drawFastVLine(marker,y,h,ILI9341_MAGENTA);
        tft.fillRoundRect(x,y,w,h,3,CL(12,0,20));
        tft.fillRoundRect(x, y, marker, h,4, CL(36, 54, 84));
        tft.drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
        tft.setTextColor(CL(0,0,0));
        tft.setCursor(x+(w/2)-1,y+(h/2),true);
        tft.print(text);
        tft.setCursor(x+(w/2)+1,y+(h/2),true);
        tft.print(text);
        tft.setCursor(x+(w/2),y+(h/2)-1,true);
        tft.print(text);
        tft.setCursor(x+(w/2),y+(h/2)+1,true);
        tft.print(text);
        tft.setCursor(x+(w/2)+1,y+(h/2)+1,true);
        tft.print(text);
        tft.setCursor(x+(w/2)-1,y+(h/2)-1,true);
        tft.print(text);
        tft.setTextColor(CL(74, 143, 255));
        tft.setCursor(x+(w/2),y+(h/2),true);
        tft.print(text);
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            isPressed = true;
            last_x = t_x;
        }
    };

    void onTouchDrag(uint16_t t_x, uint16_t t_y){
        if(isPressed){
            value += ((t_x-last_x)/2);
            if (value>100)value = 100;
            if(value <0)value=0;
            if (last_x != t_x) parentNode->MessageHandler(this,"Changed");
            last_x = t_x;
        }
    }

    void onTouchRelease(uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            parentNode->MessageHandler(this,"Set");
        }
        isPressed = false;
        last_x = x;
    };
};