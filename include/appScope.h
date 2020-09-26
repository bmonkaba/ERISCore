#include "AppManager.h"
// Basic Oscilloscope App
//

class AppScope:public AppBaseClass {
  public:
    AppScope():AppBaseClass(){
        y_last_scope = 0;
        y_last_scope_ch2 = 0;
        scope = (erisAudioAnalyzeScope*) (ad.getAudioStreamObjByName("scope_1"));
        scope->trigger();    
    }; 
    //define event handlers
  protected:
    erisAudioAnalyzeScope* scope;
    uint16_t y_last_scope;
    uint16_t y_last_scope_ch2;
    void update(){
        tft.fillRoundRect(origin_x,origin_y,width,height,3,CL(12,0,20));
        if (scope->available()){
            for (int16_t i=0;i<width;i++){
                int16_t v;
                float f;
                uint16_t ch1,ch2;
                v = scope->read(0,i);
                f = ((v * 0.000030517578125) + 1.0) * 0.5; // 1/32768 = 0.000030517578125 
                ch1 = origin_y + (uint16_t)(f * height);
                if (origin_x + i > 0) tft.drawLine(origin_x + i-1,y_last_scope,origin_x + i,ch1,ILI9341_RED);
                //draw the second channel
                v = scope->read(1,i);
                f = ((v * 0.000030517578125) + 1.0) * 0.5;
                ch2 = origin_y + (uint16_t)(f * height);
                if (origin_x + i > 0) tft.drawLine(origin_x + i-1,y_last_scope_ch2,origin_x + i,ch2,ILI9341_DARKCYAN);
                //draw x-y plot
                if (origin_x + i > 0) tft.drawLine(y_last_scope_ch2,y_last_scope,ch2,ch1,ILI9341_GREENYELLOW);
                y_last_scope = ch1; 
                y_last_scope_ch2 = ch2;

            }
            scope->trigger();
        }
        tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
    };    //called only when the app is active
    void updateRT(){}; //allways called even if app is not active
    void onFocus(){};   //called when given focus
    void onFocusLost(){}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y){
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
    };
};