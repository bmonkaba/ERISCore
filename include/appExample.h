#include "AudioUtilities.h"
#include "AppManager.h"
#include "appButton.h"
#include "appSlider.h"
#include "appScope.h"
#include "appCQT.h"

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
    erisAudioAnalyzeScope* scope;
    MyAppExample():AppBaseClass(){
      sprintf(name, "MyAppExample");
      Serial.println("MyApp constructor called");
      id = 1;
      t_lastupdate = micros();

      x_end = 0;
      x_start = 0;
      y_end = 0;
      y_start = 0;
      x_last = 0;
      y_last = 0;
      y_last_scope = 0;
      
      erisAudioFilterStateVariable* filter = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_1"));
      filter->frequency(14000);
      filter->resonance(0.20);
      erisAudioFilterStateVariable* filter2 = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_2"));
      filter2->frequency(100);
      filter2->resonance(0.20);
      erisAudioFilterStateVariable* filter3 = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_3"));
      filter3->frequency(800);

      oscope = new AppScope;
      oscope->setPosition(0,20);
      oscope->setDimension(320,220);
      oscope->setParent(this);

      cqt = new AppCQT;
      cqt->setPosition(0,20);
      cqt->setDimension(320,220);
      cqt->setParent(this);

      slider = new AppSlider();
      slider->setPosition(50,205);
      slider->setDimension(270,35);
      slider->setName("SLIDER");
      slider->setText("Dry Mix");
      slider->setValue(20);
      slider->setParent(this);
      
      char s[4][16] = {"MAKE","BREAK","SIN","SQUARE"};
      uint8_t si = 0;
      for (int x=0;x<320-40;x+=80){
        for(int y=160; y<240-40;y+=40){
          button = new AppButton(); //reuse the button var to create many instances
          button->setPosition(x,y);
          button->setDimension(60,30);
          button->setParent(this);
          button->setName(s[si]);
          button->setText(s[si++]);
        }
      }

      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();      
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      //float fps = (float)(micros()-t_lastupdate)/1000000.0;
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
      //tft.print(F("CON: "));
      //tft.print(ad.connectionCount());
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
          erisAudioMixer4* mix = (erisAudioMixer4*)(ad.getAudioStreamObjByName("mixer_5"));
          mix->gain(3,slider->value/100.0);
          //fm_mod->begin(1.0, slider->value/2000.0, WAVEFORM_TRIANGLE);
        }
        else if(sender->isName("BREAK")){ //...or, can detect sender by name
          //disconnect the fft block
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          //ad.disconnect(wav,0,fft,0);
        }
        else if(sender->isName("MAKE")){ //...or, can detect sender by name
          //disconnect the fft block
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          wav->begin(0.3, 440, WAVEFORM_SAWTOOTH);
        }
        else if(sender->isName("SIN")){
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          wav->begin(0.3, 220, WAVEFORM_SINE);
        }
        else if(sender->isName("SQUARE")){
          erisAudioSynthWaveformModulated* wav = (erisAudioSynthWaveformModulated*)(ad.getAudioStreamObjByName("waveformMod_1"));
          wav->begin(0.3, 2637.02, WAVEFORM_SQUARE);
        }
    }
};