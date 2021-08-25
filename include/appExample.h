/**
 * @file appExample.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief Application Example
 * 
 * Using the AppBaseClass template and standard functional apps
 * this example shows how a complete application can be built 
 * and communicate with the various components.
 * 
 * @version 0.1
 * @date 2021-07-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AudioUtilities.h"
#include "AppManager.h"
#include "controlButton.h"
#include "controlSlider.h"
#include "appScope.h"
#include "appCQT.h"


class MyAppExample:public AppBaseClass {
  public:
    AppCQT cqt;
    AppScope oscope;
    ControlButton *button;
    ControlSlider *slider;
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last,y_last_scope;
    uint32_t t_lastupdate;

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
      
      erisAudioSynthNoisePink* pink = (erisAudioSynthNoisePink*) (ad.getAudioStreamObjByName("pink_1"));
      pink->amplitude(1.0);
      
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_1"));
      //filter->setLowpass(0,440);
      filter->setLowpass(0,2200);
      filter->setLowpass(1,2400);
      filter->setLowpass(2,2500);
      filter->setLowpass(3,2600);
      //setHighShelf(0, 1800, -24,3.0f);
      //filter->resonance(0.9);
      filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_2"));
      filter->setLowpass(0,570);
      filter->setLowpass(1,500);
      filter->setLowpass(2,433);
      filter->setLowpass(3,400);
      
      filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_3"));
      filter->setLowpass(0,8200);
      
      filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_4"));
      filter->setHighpass(0,2400);
      
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad.getAudioStreamObjByName("freeverb_1"));
      reverb->roomsize(0.39);
      reverb->damping(0.42);

      erisAudioMixer4* mix = (erisAudioMixer4*)(ad.getAudioStreamObjByName("mixer_1"));
      mix->gain(2,0.0);

      //oscope = new AppScope;
      oscope.setWidgetPosition(5,20);
      oscope.setWidgetDimension(110,50);
      oscope.setParent(this);

      //cqt = new AppCQT;
      cqt.setWidgetPosition(125,20);
      cqt.setWidgetDimension(320-10-125,50);
      cqt.setParent(this);

      slider = new ControlSlider(this);
      slider->setWidgetPosition(10,80);
      slider->setWidgetDimension(260,30);
      slider->setName("SLIDER");
      slider->setText("Dry Mix");
      slider->setValue(0);

      
      char s[][16] = {"SIN","TRI","SAW","REVSAW","SQUARE"};
      uint8_t si = 0;
      uint16_t bx = 10;
      uint16_t by = 120;
      for (uint16_t i=0;i<5;i+=1){
        button = new ControlButton(this); //reuse the button var to create many instances
        if(bx>(320-68)){
          bx=10;by+=55;
        }
        button->setWidgetPosition(bx,by);
        button->setWidgetDimension(65,50);
        button->setName(s[si]);
        button->setText(s[si++]);
        bx+=69;
      }

      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();      
    } 
    //define event handlers
    void update(){
      //Serial.println("MyApp:update");
      //float fps = (float)(micros()-t_lastupdate)/1000000.0;
      tft.fillRect(0, 0, 320, 19, 0);
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

    void onTouch(uint16_t t_x, uint16_t t_y){
      x_start = t_x;
      y_start = t_y;
      x_last = t_x;
      y_last = t_y;
      if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
      }
    }

    void onTouchRelease(uint16_t t_x, uint16_t t_y){
      x_end = t_x;
      y_end = t_y;
      x_last = t_x;
      y_last = t_y;
      //tft.drawLine(x_start,y_start,x_end,y_end,ILI9341_ORANGE);
    }

    void onTouchDrag(uint16_t t_x, uint16_t t_y){
      //Serial.println("MyApp:onTouchDrag");
      //tft.drawPixel(x,y,ILI9341_BLUE);
      //tft.drawPixel(x-1,y,ILI9341_RED);
      //tft.drawPixel(x,y-1,ILI9341_YELLOW);
      //tft.drawPixel(x+1,y,ILI9341_ORANGE);
      //tft.drawLine(x_last,y_last,x,y,ILI9341_WHITE);
      x_last = t_x;
      y_last = t_y;
    }
    void MessageHandler(AppBaseClass *sender, const char *message){   
        if (sender == slider){ //can detect message sender by ptr...
          //erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_3"));
          //filter->setLowpass(0,100.0 + (8000.0 * (slider->value/100.0)));
        }
        else if(sender->isName("SIN")){ //...or, can detect sender by name
          changeVoice(WAVEFORM_SINE);
        }
        else if(sender->isName("TRI")){
          changeVoice(WAVEFORM_TRIANGLE);
        }
        else if(sender->isName("SAW")){
          changeVoice(WAVEFORM_SAWTOOTH);
        }
        else if(sender->isName("REVSAW")){
          changeVoice(WAVEFORM_SAWTOOTH_REVERSE);
        }
        else if(sender->isName("SQUARE")){
          changeVoice(WAVEFORM_SQUARE);
        }
        else if(sender->isName("SCI")){
          //Serial.print(F("M appExample::MessageHandler SCI param: "));
          //Serial.println(message);
          //Serial.flush();
          erisAudioSynthWaveform* wav = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName("waveform_16"));
          wav->frequency(atoi(message));
          wav->amplitude(0.15);
        }
    }

    void onFocus(){
      makeAudioConnections();
    }
    
    void onAnalog1(uint16_t uval, float fval){
     // Serial.print("AN1 ");Serial.printf("%0.4f\n",fval);
      //analog 1 controls the dry signal input to the fft blocks (used by the cqt app)
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad.getAudioStreamObjByName("amp_2"));

      AudioNoInterrupts();
      amp->gain(6*log1p(fval));
      AudioInterrupts();
    };
    
    void onAnalog2(uint16_t uval, float fval){
      //Serial.print("AN2 ");Serial.printf("%0.4f\n",fval);
      //analog 2 controls the resynthisized signal biquad output filter
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_3"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad.getAudioStreamObjByName("mixer_6"));
      AudioNoInterrupts();
      filter->setLowpass(0,601.0 + (16141.0 * fval));
      filter->setLowpass(1,727.0 + (11279.0 * fval));
      filter->setLowpass(2,857.0 + (9137.0 * fval));
      filter->setLowpass(3,919.0 + (8221.0 * fval));
      mixer->gain(0,log1p(1));
      AudioInterrupts();
    };
    
    void onAnalog3(uint16_t uval, float fval){
      //Serial.print("AN3 ");Serial.printf("%0.4f\n",fval);
      //analog 3 controls the dry signal biquad output filter and additional gain stage (post cqt)
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad.getAudioStreamObjByName("biquad_4"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad.getAudioStreamObjByName("mixer_1"));
      float hp = 2400.0 + (2400.0 * log1p(10.0*fval));
      float lp = 150.0 + (300.0 * log1p(10.0*fval));
      float gain = log1p(fval);
      AudioNoInterrupts();
      filter->setHighpass(0,hp);
      filter->setLowpass(0,lp);
      mixer->gain(2,gain);
      AudioInterrupts();
    };

    void onAnalog4(uint16_t uval, float fval){
      //Serial.print("AN4 ");Serial.printf("%0.4f\n",fval);
      //output volume
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad.getAudioStreamObjByName("amp_1"));
      float gain = log1p(9.0 * fval);
      AudioNoInterrupts();
      amp->gain(gain);
      AudioInterrupts();
    };
    
    void makeAudioConnections(){
      ad.disconnectAll();      
      //16 voice oscillator bank bus mixer (6)
      ad.connect("mixer_2 0 mixer_6 0");
      ad.connect("mixer_3 0 mixer_6 1");
      ad.connect("mixer_4 0 mixer_6 2");
      ad.connect("mixer_5 0 mixer_6 3");
      //synth output to reverb and master mixer (1)

      ad.connect("mixer_6 0 biquad_3 0");
      ad.connect("biquad_3 0 mixer_1 0");
      ad.connect("biquad_3 0 freeverb_1 0");
      //ad.connect("freeverb_1 0 mixer_1 1");

      //input through filter 3 to the master mixer
      ad.connect("i2s-in_1 1 biquad_4 2");//HP
      //ad.connect("biquad_4 0 mixer_1 2");
      ad.connect("i2s-in_1 1 mixer_1 2");

      //master mixer to the output amp
      ad.connect("mixer_1 0 amp_1 0");
      ad.connect("amp_1 0 i2s-out_1 0");

      //osc banks
      ad.connect("waveform_1 0 mixer_2 0");
      ad.connect("waveform_2 0 mixer_2 1");
      ad.connect("waveform_3 0 mixer_2 2");
      ad.connect("waveform_4 0 mixer_2 3");
      
      ad.connect("waveform_5 0 mixer_3 0");
      ad.connect("waveform_6 0 mixer_3 1");
      ad.connect("waveform_7 0 mixer_3 2");
      ad.connect("waveform_8 0 mixer_3 3");
      
      ad.connect("waveform_9 0 mixer_4 0");
      ad.connect("waveform_10 0 mixer_4 1");
      ad.connect("waveform_11 0 mixer_4 2");
      ad.connect("waveform_12 0 mixer_4 3");
      
      ad.connect("waveform_13 0 mixer_5 0");
      ad.connect("waveform_14 0 mixer_5 1");
      ad.connect("waveform_15 0 mixer_5 2");
      ad.connect("waveform_16 0 mixer_5 3");

      //fft connections
      ad.connect("i2s-in_1 1 amp_2 0");

      ad.connect("amp_2 0 biquad_1 0");
      ad.connect("biquad_1 0 fft1024_1 0"); //lp filter
      
      ad.connect("amp_2 0 biquad_2 0");
      ad.connect("biquad_2 0 fft1024_2 0"); //lp filter

      //scope connections
      //input through the fft amp
      ad.connect("amp_2 0 scope_1 0");
      //resynthed signal output filter
      ad.connect("biquad_3 0 scope_1 1");

      AudioInterrupts();
    }

    void changeVoice(uint16_t voice_type){
      char buffer[32];
      erisAudioSynthWaveform* w;
      
      
      for (int16_t i=1; i <= OSC_BANK_SIZE; i++){
        sprintf(buffer, "waveform_%d", i);
        w = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        w->begin(voice_type);
        AudioInterrupts();
      }
      
      //change the voice of the test signal too
      w = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName("waveform_16"));
      AudioNoInterrupts();
      w->begin(voice_type);
      AudioInterrupts();

      return;
    }
};