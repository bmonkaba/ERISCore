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
        strcpy(name,"AppScope");
        hasFocus = false;
    }; 
    void setPosition (int16_t newOriginX, int16_t newOriginY){
        origin_x=newOriginX;
        origin_y=newOriginY;
        widget_origin_x=newOriginX;
        widget_origin_y=newOriginY;
    }
    void setDimension(int16_t new_width, int16_t new_height){
        width=new_width;
        height=new_height;
        widget_width=new_width;
        widget_height=new_height;
    }
  protected:
    erisAudioAnalyzeScope* scope;
    uint16_t y_last_scope;
    uint16_t y_last_scope_ch2;
    bool hasFocus=false;
    uint16_t widget_origin_x; 
    uint16_t widget_origin_y;
    uint16_t widget_width;
    uint16_t widget_height;

    void update(){
        float scale;
        if (scope->available()){
            scale = 30000.0 / ((float)scope->getPeakValue() + 0.0001);
            if (scale > 15000.0) scale = 15000.0;
            uint16_t w;
            uint16_t h;
            uint16_t x;
            uint16_t y;
            if(hasFocus){
                w = width;
                h = height;
                x = origin_x;
                y = origin_y;
            }else {
                w = widget_width;
                h = widget_height;
                x = widget_origin_x;
                y = widget_origin_y;
            }
            tft.fillRoundRect(x,y,w,h,3,CL(12,0,20));
            for (int16_t i=0;i<w;i++){
                int16_t v;
                float f;
                uint16_t ch1,ch2;
                v = scope->read(0,i) * scale;
                f = ((v * 0.000030517578125) + 1.0) * 0.5; // 1/32768 = 0.000030517578125 
                ch1 = y + (uint16_t)(f * h);
                if (x + i > 0) tft.drawLine(x + i-1,y_last_scope,x + i,ch1,ILI9341_ORANGE);
                //draw the second channel
                v = scope->read(1,i) * scale;
                f = ((v * 0.000030517578125) + 1.0) * 0.5;
                ch2 = y + (uint16_t)(f * h);
                if (x + i > 0) tft.drawLine(x + i-1,y_last_scope_ch2,x + i,ch2,ILI9341_GREEN);
                //draw x-y plot
                if (x + i > 0) tft.drawLine(y_last_scope_ch2,y_last_scope,ch2,ch1,ILI9341_GREENYELLOW);
                y_last_scope = ch1; 
                y_last_scope_ch2 = ch2;
            }
            

            //publish the scopes math functions
            AppManager::getInstance()->data.update("DOT",(int32_t)(scope->getDotProduct()>>8));
            AppManager::getInstance()->data.update("DOT_AVG",(int32_t)(scope->getDotProductAvg()>>8));
            AppManager::getInstance()->data.update("DOT_DELTA",(int32_t)(scope->getDotDelta()));
            AppManager::getInstance()->data.update("DOT_DELTA_MACD",(int32_t)scope->getDotDeltaMACD());
            AppManager::getInstance()->data.update("DOT_MACD",(int32_t)scope->getDotMACD());
            AppManager::getInstance()->data.update("EDGE_COUNT",(int32_t)scope->getEdgeCount());
            AppManager::getInstance()->data.update("EDGE_COUNT_CH2",(int32_t)scope->getEdgeCount_ch2());
            AppManager::getInstance()->data.update("EDGE_DELAY",(int32_t)scope->getEdgeDelay());
            AppManager::getInstance()->data.update("EDGE_DELAY2",(int32_t)scope->getEdgeDelay2());
            AppManager::getInstance()->data.update("EDGE_DELTA",(int32_t)scope->getEdgeDelay()-(int32_t)scope->getEdgeDelay2());
            AppManager::getInstance()->data.update("INPUT_PEAK",(int32_t)scope->getPeakValue());
            
            //Serial.printf("%" PRId64 "\n", scope->getDotDelta());
            scope->trigger();
            tft.setCursor(x+w - 100,y+5);
            tft.print("scale: ");
            tft.print(scale);
            tft.setCursor(x+w - 100,y+20);
            tft.print("hdiv: ");
            tft.print(scope->getHDiv());
            tft.drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
        }
        
    };    //called only when the app is active
    void updateRT(){}; //allways called even if app is not active
    void onFocus(){};   //called when given focus
    void onFocusLost(){}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y){
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
            if(!hasFocus){
                AppManager::getInstance()->getFocus(this->id);
                hasFocus = true;
                origin_x = 0; origin_y = 0;
                width = 320; height = 240;

            }else{
                AppManager::getInstance()->returnFocus();
                hasFocus = false;
            }
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
    };
};