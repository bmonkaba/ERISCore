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
        tft.fillRoundRect(origin_x,origin_y,width,height,3,CL(12,0,20));
        if (show_active){
            //tft.bltSD("/I/U/W","greenhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_GREENYELLOW);
            if(isPressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                show_active = false;
            }
        } else{
            //tft.bltSD("/I/U/W","redhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
        }
        tft.setCursor(origin_x+(width/2),origin_y+(height/2),true);
        tft.setFont(Arial_9);
        tft.print(text);
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t x, uint16_t y){
        //Serial.println("MyButton:onTouch");
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //Serial.println("MyButton:onTouch Button Pressed");
            isPressed = true;
            show_active = true;
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
        if (isPressed && x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //Serial.println("MyButton:onTouchRelease Button Press Event Triggered");
            parentNode->MessageHandler(this,"Pressed");
        }
        isPressed = false;
        time_active = 0;
    };
};