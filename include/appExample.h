#include "AudioUtilities.h"
#include "AppManager.h"
#include "AppButton.h"
#include "AppSlider.h"
#include "AppScope.h"
#include "AppCQT.h"

// Example application
//
class MyAppExample:public AppBaseClass {
  public:
    AppCQT *cqt;
    AppScope *oscope;
    AppButton *button;
    AppSlider *slider;
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last,y_last_scope;
    uint32_t t_lastupdate;
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    int16_t fft2AutoOffset;
    erisAudioAnalyzeScope* scope;
    MyAppExample():AppBaseClass(){
      Serial.println("MyApp constructor called");
      id = 1;
      t_lastupdate = micros();

      erisAudioFilterStateVariable* filter = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_1"));
      filter->frequency(22050/2);

      erisAudioFilterStateVariable* filter2 = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_2"));
      filter2->frequency(22050/16);

      erisAudioFilterStateVariable* filter3 = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_3"));
      filter3->frequency(22050/16);

      oscope = new AppScope;
      oscope->setPosition(10,20);
      oscope->setDimension(290,100);
      oscope->setParent(this);

      cqt = new AppCQT;
      cqt->setPosition(10,20);
      cqt->setDimension(290,100);
      cqt->setParent(this);

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
          button->setDimension(60,30);
          button->setParent(this);
          button->setName(s[si]);
          strcpy(button->text,s[si++]);
        }
      }
      
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      float fps = (float)(micros()-t_lastupdate)/1000000.0;
      tft.setCursor(5,5);
      tft.print(F("CPU: "));
      tft.print(AudioProcessorUsageMax());
      tft.print(F(" ("));
      tft.print(AudioProcessorUsage());
      tft.print(F(")"));
      tft.setCursor(130,5);
      tft.print(F("ABMEM: "));
      tft.print(AudioMemoryUsageMax());
      tft.print(F(" ("));
      tft.print(AudioMemoryUsage());
      tft.print(F(")"));
      tft.setCursor(260,5);
      tft.print(F("CON: "));
      tft.print(ad.connectionCount());
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
          fm_mod->begin(1.0, slider->value/200.0, WAVEFORM_SINE);
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