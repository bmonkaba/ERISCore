/**
 * @file appScope.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"
// Basic Oscilloscope App
//

class AppScope:public AppBaseClass {
  public:
    AppScope():AppBaseClass(){
        scope = (erisAudioAnalyzeScope*) (ad->getAudioStreamObjByName("scope_1"));
        scope->trigger();
        strcpy(name,"AppScope");
    }; 

  protected:
    erisAudioAnalyzeScope* scope;
    float32_t scale;
    void update(){
        
        uint16_t y_last_scope;
        uint16_t y_last_scope_ch2;

        y_last_scope=h/2;
        y_last_scope_ch2=h/2;
       
        scale = 30000.0 / ((float)scope->getPeakValue() + 0.0001);
        if (scale > 10000.0) scale = 10000.0;
        am->data->update("OSCOPE_SCALE",scale);
        draw->fillRoundRect(x,y,w,h,3,CL(0,0,0));
        for (int16_t i=0;i<w;i++){
            int16_t v;
            float f;
            uint16_t ch1,ch2;
            v = scope->read(0,i) * scale;
            f = ((v * 0.000030517578125) + 1.0) * 0.5; // 1/32768 = 0.000030517578125 
            ch1 = y + (uint16_t)(f * h);
            if (i > 0) draw->drawLine(x + i-1,y_last_scope,x + i,ch1,ILI9341_ORANGE);
            //draw the second channel
            v = scope->read(1,i) * scale;
            f = ((v * 0.000030517578125) + 1.0) * 0.5;
            ch2 = y + (uint16_t)(f * h);
            if (i > 0) draw->drawLine(x + i-1,y_last_scope_ch2,x + i,ch2,ILI9341_CYAN);
            //draw x-y plot
            if (i > 0) draw->drawLine(y_last_scope_ch2,y_last_scope,ch2,ch1,ILI9341_GREENYELLOW);
            y_last_scope = ch1; 
            y_last_scope_ch2 = ch2;
        }

        if (w>120 && h>60){
            draw->setCursor(x+w - 100,y+5);
            draw->print("scale: ");
            draw->print(scale);
            draw->setCursor(x+w - 100,y+20);
            draw->print("hdiv: ");
            draw->print(scope->getHDiv());   
        } 
        draw->drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
    };    //called only when the app is active

    void updateRT(){
        am->data->update("INPUT_PEAK",(int32_t)scope->getPeakValue());
        /*
        //publish the scopes math functions
        AppManager *am = AppManager::getInstance();
        //am->data->update("DOT",scope->getDotProduct());
        //am->data->update("DOT_AVG",scope->getDotProductAvg());
        am->data->update("DOT_AVG_SLOW",scope->getDotProductAvgSlow());
        am->data->update("DOT_DELTA",scope->getDotDelta());
        //am->data->update("DOT_DELTA_MACD",scope->getDotDeltaMACD());
        am->data->update("DOT_ACCEL",scope->getDotAcceleration());
        //am->data->update("DOT_MACD",scope->getDotMACD());
        //am->data->update("EDGE_COUNT",scope->getEdgeCount());
        //am->data->update("EDGE_COUNT_CH2",scope->getEdgeCount_ch2());
        //am->data->update("EDGE_DELAY",scope->getEdgeDelay());
        //am->data->update("EDGE_DELAY2",scope->getEdgeDelay2());
        am->data->update("EDGE_DELTA",scope->getEdgeDelay()-scope->getEdgeDelay2() + 1); //min value of 1 (protect for div by zero)
        am->data->update("INPUT_PEAK",scope->getPeakValue());
        if(scope->getEdgeDelay()>20) am->data->update("CH1_FREQ",(int32_t)(AUDIO_SAMPLE_RATE_EXACT/(0.001* ((float32_t)scope->getEdgeDelay() + 0.00001))));
        if(scope->getEdgeDelay2()>20) am->data->update("CH2_FREQ",(int32_t)(AUDIO_SAMPLE_RATE_EXACT/(0.001* ((float32_t)scope->getEdgeDelay2()+ 0.00001))));
        */
    }; //allways called even if app is not active
  
    void onFocus(){};   //called when given focus
  
    void onFocusLost(){}; //called when focus is taken
  
    void onTouch(uint16_t t_x, uint16_t t_y){
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
            if(!has_pop){
                //getFocus();
                requestPopUp(); 
                
            }else{
                //returnFocus();
                releasePopUp();
            }
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
    };
};