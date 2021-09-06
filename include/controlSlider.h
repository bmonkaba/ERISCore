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
// Slider 
//

class ControlSlider:public AppBaseClass {
  public:
    ControlSlider(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        isPressed = false;
        isDirty = true;
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
    uint16_t value;
    char text[MAX_TEXT_LENGTH];
  private:
    uint16_t last_x;
  protected:
    bool isDirty;
    bool isPressed;
    void update(){
        isDirty = true;
        if (isDirty){
            int16_t marker = w*(value/100.0);
            //draw->drawFastVLine(marker,y,h,ILI9341_MAGENTA);
            //draw->fillRoundRect(x,y,w,h,4,CL(12,0,20));
            draw->fillRoundRect(x+1, y, marker+2, h,4, CL(36, 54, 84));
            draw->drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
            /*
            draw->setTextColor(CL(0,0,0));
            draw->setCursor(x+(w/2)-1,y+(h/2),true);
            draw->print(text);
            draw->setCursor(x+(w/2)+1,y+(h/2),true);
            draw->print(text);
            draw->setCursor(x+(w/2),y+(h/2)-1,true);
            draw->print(text);
            draw->setCursor(x+(w/2),y+(h/2)+1,true);
            draw->print(text);
            draw->setCursor(x+(w/2)+1,y+(h/2)+1,true);
            draw->print(text);
            draw->setCursor(x+(w/2)-1,y+(h/2)-1,true);
            draw->print(text);
            */
            draw->setTextColor(CL(74, 143, 255));
            //draw->setCursor(x+(w/2),y+(h/2),true);
            //draw->print(text);
            isDirty = false;
        }
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            isPressed = true;
            last_x = t_x;
            isDirty = true;
        }
    };

    void onTouchDrag(uint16_t t_x, uint16_t t_y){
        if(isPressed){
            value += ((t_x-last_x)/2);
            if (value>100)value = 100;
            if(value <0)value=0;
            if (last_x != t_x) parentNode->MessageHandler(this,"Changed");
            last_x = t_x;
            isDirty = true;
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