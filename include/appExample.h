#include "apps.h"

// Example application
//
class MyAppExample:public AppBaseClass {
  public:
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last;
    
    MyAppExample():AppBaseClass(){
      Serial.println("MyApp constructor called");
      id = 1;
      tft.updateScreenAsync(true);
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      int32_t x,y;
      uint16_t p;
      for (int32_t i=0; i < 2; i++){
        x = random(0,320);
        y = random(0,240);
        p = tft.readPixel(x,y);
        tft.drawPixel(x,y,p);
        tft.drawPixel(x + random(0,2),y + random(0,2), p);
        tft.drawPixel(x - random(0,2),y + random(0,2), p);
      }
      //tft.drawPixel(random(0,320),random(0,240),ILI9341_BLACK);
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