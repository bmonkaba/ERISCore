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
#include "ErisUtils.h"
// Slider 
//

class ControlSlider:public AppBaseClass {
  public:
    ControlSlider(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        updateRT_priority = 5;
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
      } else safer_strncpy(text,name_string,MAX_TEXT_LENGTH - 1);
    }
    //define event handlers
    int16_t value;
    char text[MAX_TEXT_LENGTH];
  private:
    uint16_t last_x;
  protected:
    bool isDirty;
    bool isPressed;
    void update() override{
        isDirty = true;
        if (isDirty){
            int16_t marker = w*((float)value/100.0);
            draw->fillRoundRect(x+1, y, marker+2, h/2 + 4,4, am->data->read("UI_SLIDER_FILL_COLOR"));
            draw->fillRoundRect(x+1, y+ h/2, marker+2, h/2,4, am->data->read("UI_SLIDER_SHADE_COLOR"));
            draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_SLIDER_BORDER_COLOR"));
            draw->setTextColor(am->data->read("UI_SLIDER_TEXT_COLOR"));
            draw->setCursor(x+(w/2),y+(h/2),true);
            draw->print(text);
            isDirty = false;
        }
    };
    void onFocusLost() override{isPressed=false;};
    void onTouch (uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            isPressed = true;
            last_x = t_x;
            isDirty = true;
        }
    };

    void onTouchDrag (uint16_t t_x, uint16_t t_y){
        if(isPressed){
            value += ((t_x-last_x)/2);
            if (value>100)value = 100;
            if (last_x != t_x) parentNode->MessageHandler(this,"Changed");
            last_x = t_x;
            isDirty = true;
        }
    }

    void onTouchRelease (uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            parentNode->MessageHandler(this,"Set");
        }
        isPressed = false;
        last_x = x;
    };
};