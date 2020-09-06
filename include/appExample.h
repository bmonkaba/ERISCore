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
    int16_t x_last,y_last,y_last_scope;
    unsigned long t_lastupdate;
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    erisAudioAnalyzeScope* scope;
    MyAppExample():AppBaseClass(){
      Serial.println("MyApp constructor called");
      id = 1;
      t_lastupdate = micros();
      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft->enableFFT(true);

      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange();
      fft2->enableFFT(true);

      erisAudioFilterStateVariable* filter = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_1"));
      filter->frequency(2000);

      erisAudioFilterStateVariable* filter2 = (erisAudioFilterStateVariable*) (ad.getAudioStreamObjByName("filter_2"));
      filter2->frequency(600);

      scope = (erisAudioAnalyzeScope*) (ad.getAudioStreamObjByName("scope_1"));
      scope->trigger();

      //fft->toggleActiveRange();
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
      if (scope->available()){
        for (int16_t i=0;i<320;i++){
          int16_t v;
          float f;
          uint16_t ss;

          v = scope->read(i);
          f = ((v / 32768.0) + 1.0)/2;
          ss = (uint16_t)(f * height / 2.0);
          if (i>0) tft.drawLine(i-1,y_last_scope,i,ss,ILI9341_DARKGREY);
          y_last_scope = ss;
        }
        scope->trigger();
      }
      if (!tft.busy() && fft->available()) {
        float fps = (float)(micros()-t_lastupdate)/1000000.0;
        tft.setCursor(5,5);
        //tft.println(1.0/fps);
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
        int16_t last_y=0;
        int16_t peak1 = 0;
        int16_t peak2 = 0;
        int16_t plotx;
        float max = -9999;
        for (int16_t j=2; j<510; j+=1) {
          n = min(fft2->read(j),fft2->read(j-1));
          n = min(fft2->read(j+1),n);
          if (n>max){max=n;peak2=j;}
          plotx = (int16_t)(((float)j/512.0)*320);
          //tft.drawLine(plotx-1,240-last_y,plotx,240-((int16_t)(log(n*200)*50)/2),ILI9341_MAROON);
          last_y = (int16_t)(log(n*200)*50)/2;
        }
        max = -9999;
        for (int16_t j=2; j<510; j+=1) {
          n = min(fft->read(j/4),fft->read(j/4-1));
          n = min(fft->read(j/4+1),n);
          if (n>max){max=n;peak1=j;}
          plotx = (int16_t)(((float)j/512.0)*320);
          tft.drawLine(plotx-1,240-last_y,plotx,240-((int16_t)(log(n*200)*50)/2),ILI9341_DARKGREY);
          last_y = (int16_t)(log(n*200)*50)/2;
        }
        int16_t offset = (peak2-peak1)/2;

        for (int16_t j=2; j<510; j+=1) {
          if (j+offset > 1 && j+offset < 510){
            n = min(fft2->read(j+offset),fft2->read(j+offset-1));
            n = min(fft2->read(j+offset+1),n);
            float signal_a = 240-((int16_t)(log(n*200)*50)/2);

            n = min(fft->read(j/4),fft->read(j/4-1));
            n = min(fft->read(j/4+1),n);
            float signal_b = 240-((int16_t)(log(n*200)*50)/2);
            signal_a = min(signal_a,signal_b);
            plotx = (int16_t)(((float)j/512.0)*320);
            tft.drawLine(plotx-1,last_y,plotx,signal_a,ILI9341_PURPLE);
            last_y = signal_a;
          }
        }
        
      }
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