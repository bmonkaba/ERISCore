#include "AppManager.h"
#include "AppButton.h"
#include "AppSlider.h"
// Example application
//
class MyAppExample:public AppBaseClass {
  public:
    AppButton *button;
    AppSlider *slider;
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last;
    unsigned long t_lastupdate;
    erisAudioAnalyzeFFT1024* fft;
    MyAppExample():AppBaseClass(){
      Serial.println("MyApp constructor called");
      id = 1;
      t_lastupdate = micros();
      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();

      slider = new AppSlider();
      slider->origin_x=160;       //for testing
      slider->origin_y=200;
      slider->width=120;
      slider->height=30;
      slider->value=20;
      slider->setParent(this);
      slider->setName("FM OSC");
      strcpy(slider->text,"FM OSC");
      char s[4][16] = {"MAKE","BREAK","SIN","SQUARE"};
      uint8_t si = 0;
      for (int x=160;x<320-40;x+=80){
        for(int y=120; y<240-40;y+=40){
          button = new AppButton(); //reuse the button var to create many instances
          button->setPosition(x,y);
          //button->origin_x=x;       //for testing
          //button->origin_y=y;
          button->setDimension(60,30);
          //button->width=60;
          //button->height=30;
          button->setParent(this);
          button->setName(s[si]);
          strcpy(button->text,s[si++]);
        }
      }
      
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      int32_t x,y;
      uint16_t p;
      float n;

      if (!tft.busy()) tft.bltSDFullScreen("bluehex.ile");
      if (!tft.busy() & fft->available()) {
        float fps = (float)(micros()-t_lastupdate)/1000000.0;
        tft.setCursor(5,5);
        //tft.println(1.0/fps);
        tft.print("CPU: ");
        tft.print(AudioProcessorUsageMax());
        tft.print(" (");
        tft.print(AudioProcessorUsage());
        tft.print(")");
        tft.setCursor(130,5);
        tft.print("ABMEM: ");
        tft.print(AudioMemoryUsageMax());
        tft.print(" (");
        tft.print(AudioMemoryUsage());
        tft.print(")");
        tft.setCursor(260,5);
        tft.print("CON: ");
        tft.print(ad.connectionCount());

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
      x_start = x;
      y_start = y;
      x_last = x;
      y_last = y;
      tft.bltSDFullScreen("bluehex.ile");
    }

    void onTouchRelease(uint16_t x, uint16_t y){
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
    void MessageHandler(AppBaseClass *sender, const char *message){   
        if (sender == slider){ //can detect sender by ptr...
          erisAudioSynthWaveform* fm_mod = (erisAudioSynthWaveform*)(ad.getAudioStreamObjByName("waveform_1"));
          fm_mod->begin(1.0, slider->value/10, WAVEFORM_TRIANGLE);
        }
        else if(sender->isName("BREAK")){ //...or, can detect sender by name
          //disconnect the fft block
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          ad.disconnect(wav,0,fft,0);
        }
        else if(sender->isName("MAKE")){ //...or, can detect sender by name
          //disconnect the fft block
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          ad.connect(wav,0,fft,0);
        }
        else if(sender->isName("SIN")){
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          wav->begin(1.0, 440, WAVEFORM_SINE);
        }
        else if(sender->isName("SQUARE")){
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          wav->begin(1.0, 440, WAVEFORM_SQUARE);
        }
    }
};