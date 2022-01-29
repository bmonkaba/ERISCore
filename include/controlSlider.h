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
        update_priority = 253;
        is_pressed = false;
        is_dirty = true;
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
    bool is_dirty;
    bool is_pressed;
    void render() override{
        is_dirty = true;
        if (is_dirty){
            int16_t marker = w*((float)value/100.0);
            draw->fillRoundRect(x+1, y, marker+2, h/2 + 4,4, am->data->read("UI_SLIDER_FILL_COLOR"));
            draw->fillRoundRect(x+1, y+ h/2, marker+2, h/2,4, am->data->read("UI_SLIDER_SHADE_COLOR"));
            draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_SLIDER_BORDER_COLOR"));
            draw->setTextColor(am->data->read("UI_SLIDER_TEXT_COLOR"));
            draw->setCursor(x+(w/2),y+(h/2),true);
            draw->print(text);
            is_dirty = false;
        }
    };
    void onFocusLost() override{is_pressed=false;};
    void onTouch (uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            is_pressed = true;
            last_x = t_x;
            is_dirty = true;
        }
    };

    void onTouchDrag (uint16_t t_x, uint16_t t_y){
        if(is_pressed){
            value += ((t_x-last_x)/2);
            if (value>100)value = 100;
            if (last_x != t_x) parent_node->messageHandler(this,"Changed");
            last_x = t_x;
            is_dirty = true;
        }
    }

    void onTouchRelease (uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            parent_node->messageHandler(this,"Set");
        }
        is_pressed = false;
        last_x = x;
    };
};