#include "AppManager.h"

#define MAX_BUTTON_TEXT_LENGTH 16
// Button 
//

class AppButton:public AppBaseClass {
  public:
    AppButton():AppBaseClass(){
        isPressed = false;
        showActivatedUntil=micros();
        strcpy(text,"AppButton");
        strcpy(name,"AppButton");
    };

    void setText(const char* name_string){
      if (strlen(name_string) < MAX_BUTTON_TEXT_LENGTH - 1){strcpy(text,name_string);
      } else strncpy(text,name_string,MAX_BUTTON_TEXT_LENGTH - 1);
    }

    char text[MAX_BUTTON_TEXT_LENGTH];

  protected:
    bool isPressed;
    unsigned long showActivatedUntil;
    void update(){
        if (showActivatedUntil > micros()){
            //tft.bltSD("/I/U/W","greenhex.ile",origin_x,origin_y,AT_NONE);
            tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_GREENYELLOW);
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
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
        if (isPressed && x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //Serial.println("MyButton:onTouchRelease Button Press Event Triggered");
            parentNode->MessageHandler(this,"Pressed");
            showActivatedUntil = micros() + 500000; //show the active state for 1/2 sec
        }
        isPressed = false;
    };
};