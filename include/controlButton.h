/**
 * @file controlButton.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"

#define MAX_BUTTON_TEXT_LENGTH 16
#define SHOW_ACTIVE_TIME_MILLISEC 500

// Button 
//

class ControlButton:public AppBaseClass {
  public:
    ControlButton(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        isPressed = false;
        show_active = false;
        time_active = 0;
        strcpy(text,"ControlButton");
        strcpy(name,"ControlButton");
    };

    void setText(const char* name_string){
      if (strlen(name_string) < MAX_BUTTON_TEXT_LENGTH - 1){strcpy(text,name_string);
      } else strncpy(text,name_string,MAX_BUTTON_TEXT_LENGTH - 1);
    }

    char text[MAX_BUTTON_TEXT_LENGTH];

  protected:
    bool isPressed;
    elapsedMillis time_active;
    bool show_active;
    void update(){
        tft.fillRoundRect(x,y,w,h,3,CL(12,0,20));
        if (show_active){
            //tft.bltSD("/I/U/W","greenhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(x,y,w,h,4,ILI9341_GREENYELLOW);
            if(isPressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                show_active = false;
            }
        } else{
            //tft.bltSD("/I/U/W","redhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
        }
        tft.setCursor(x+(w/2),y+(h/2),true);
        tft.setFont(Arial_9);
        tft.print(text);
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t t_x, uint16_t t_y){
        //Serial.println("MyButton:onTouch");
        //check if touch point is within the application bounding box
        if ((t_x > x && t_x < x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouch Button Pressed");
            isPressed = true;
            show_active = true;
        }
    };
    void onTouchRelease(uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouchRelease Button Press Event Triggered");
            parentNode->MessageHandler(this,"Pressed");
        }
        isPressed = false;
        time_active = 0;
    };
};