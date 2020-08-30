#include "AppManager.h"
#include "AppButton.h"
// Example application
//
class MyAppExample:public AppBaseClass {
  public:
    AppButton *button;
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last;
    unsigned long t_lastupdate;
    erisAudioAnalyzeFFT1024* fft;
    MyAppExample():AppBaseClass(){
      Serial.println("MyApp constructor called");
      id = 1;
      t_lastupdate = micros();
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();
      button = new AppButton();
      button->origin_x=160;
      button->origin_y=120;
      button->width=160;
      button->height=120;
      button->setParent((AppBaseClass*)this);
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      int32_t x,y;
      uint16_t p;
      /*
      for (int32_t i=0; i < 1; i++){
        x = random(0,320);
        y = random(0,240);
        p = tft.readPixel(x,y);
        tft.drawPixel(x,y,p);
        tft.drawPixel(x + random(0,2),y + random(0,2), p);
        tft.drawPixel(x - random(0,2),y - random(0,2), p);
      }
      */

       //to use the objects they must be downcast to the correct object type
        //erisAudioAnalyzeFFT1024* fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
        float n;

        if (!tft.busy() & fft->available()) {
          float fps = (float)(micros()-t_lastupdate)/1000000.0;
          tft.bltSDFullScreen("bluehex.ile");
          tft.setCursor(10,10);
          tft.println(1.0/fps);
          tft.println("AUDIO CPU max(current):");
          tft.print(AudioProcessorUsageMax());
          tft.print(" (");
          tft.print(AudioProcessorUsage());
          tft.println(")");
          tft.println("AUDIO MEM max(current):");
          tft.print(AudioMemoryUsageMax());
          tft.print(" (");
          tft.print(AudioMemoryUsage());
          tft.println(")");
          t_lastupdate = micros(); 
          int16_t last_y=0;
          for (int16_t j=1; j<320/2; j+=1) {
            n = fft->read(j/1);
            //tft.drawFastVLine(j,0,(int16_t)(log(n*200)*50),ILI9341_DARKGREY);
            //tft.drawPixel(j,(int16_t)(log(n*200)*50),ILI9341_DARKCYAN);
            
            tft.drawLine(j-1,240-last_y,j,240-((int16_t)(log(n*200)*50)/2),ILI9341_DARKGREY);
            last_y = (int16_t)(log(n*200)*50)/2;
          }
          //tft.updateScreen();
        }
      //tft.drawPixel(random(0,320),random(0,240),ILI9341_BLACK);
      t_lastupdate = micros();
    }
    void onTouch(uint16_t x, uint16_t y){
      Serial.println("MyApp:onTouch");
      x_start = x;
      y_start = y;
      x_last = x;
      y_last = y;
      tft.bltSDFullScreen("bluehex.ile");
    }
    void onTouchRelease(uint16_t x, uint16_t y){
      //Serial.println("MyApp:onTouch");
      x_end = x;
      y_end = y;
      x_last = x;
      y_last = y;
      tft.drawLine(x_start,y_start,x_end,y_end,ILI9341_ORANGE);
    }
    void onTouchDrag(uint16_t x, uint16_t y){
      //Serial.println("MyApp:onTouchDrag");
        tft.drawPixel(x,y,ILI9341_BLUE);
        tft.drawPixel(x-1,y,ILI9341_RED);
        tft.drawPixel(x,y-1,ILI9341_YELLOW);
        tft.drawPixel(x+1,y,ILI9341_ORANGE);
        tft.drawLine(x_last,y_last,x,y,ILI9341_WHITE);
        x_last = x;
        y_last = y;
    }
};