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
      
      erisAudioSynthNoisePink* pink = (erisAudioSynthNoisePink*) (ad->getAudioStreamObjByName("pink_1"));
      pink->amplitude(1.0);
      
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_1"));
      filter->setLowpass(0, 1100,0.6);

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_2"));
      filter->setLowpass(0, 300);

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_3"));
      filter->setLowpass(0,8052);
      
      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_4"));
      filter->setLowpass(0,18400);
      
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad->getAudioStreamObjByName("freeverb_1"));
      reverb->roomsize(0.95);
      reverb->damping(0.22);

      erisAudioMixer4* mix = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_1"));
      mix->gain(0,0.75);
      mix->gain(1,0.25);

      //oscope = new AppScope;
      oscope.setWidgetPosition(5,20);
      oscope.setWidgetDimension(110,50);
      oscope.setPosition(5,20);
      oscope.setDimension(315,100);
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
      
      char s[][16] = {"SIN","TRI","SAW","REVSAW","SQUARE","TEST"};
      uint8_t si = 0;
      uint16_t bx = 10;
      uint16_t by = 124;
      for (uint16_t i=0;i<sizeof(s)/16;i+=1){
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

      draw->fillRect(0, 0, 320, 19, 0);
      draw->setCursor(5,5);
      draw->print(F("CPU: "));
      draw->print(AudioProcessorUsageMax());
      draw->print(F(" ("));
      draw->print(AudioProcessorUsage());
      draw->print(F(")"));
      draw->setCursor(130,5);
      draw->print(F("ABMEM: "));
      draw->print(AudioMemoryUsageMax());
      draw->print(F(" ("));
      draw->print(AudioMemoryUsage());
      draw->print(F(")"));
      draw->setCursor(260,5);
      draw->print(F("CON: "));
      draw->print(ad->connectionCount());
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
      //draw->drawLine(x_start,y_start,x_end,y_end,ILI9341_ORANGE);
    }

    void onTouchDrag(uint16_t t_x, uint16_t t_y){
      //Serial.println("MyApp:onTouchDrag");
      //draw->drawPixel(x,y,ILI9341_BLUE);
      //draw->drawPixel(x-1,y,ILI9341_RED);
      //draw->drawPixel(x,y-1,ILI9341_YELLOW);
      //draw->drawPixel(x+1,y,ILI9341_ORANGE);
      //draw->drawLine(x_last,y_last,x,y,ILI9341_WHITE);
      x_last = t_x;
      y_last = t_y;
    }
    void MessageHandler(AppBaseClass *sender, const char *message){   
        if (sender == slider){ //can detect message sender by ptr...
          //erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_3"));
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
          erisAudioSynthWaveform* wav = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform_16"));
          //AudioNoInterrupts();
          wav->frequency(atoi(message));
          wav->amplitude(0.01);
          //AudioNoInterrupts();
        }
        else if(sender->isName("TEST")){
          AudioNoInterrupts();
          ad->disconnect("fft1024_1 0");
          ad->disconnect("fft1024_2 0");
          ad->connect("waveform_16 0 fft1024_1 0");
          ad->connect("waveform_16 0 fft1024_2 0");
          //ad->connect("waveform_16 0 scope_1 0");
          AudioInterrupts();
        }
    }

    void onFocus(){
      makeAudioConnections();
      am->sendMessage(this,"CQT","ENABLE");
    }
    
    void onAnalog1(uint16_t uval, float fval){
     // Serial.print("AN1 ");Serial.printf("%0.4f\n",fval);
      //analog 1 controls the dry signal input to the fft blocks (used by the cqt app)
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp_2"));
      AudioNoInterrupts();
      amp->gain(log1p(fval));
      AudioInterrupts();
    };
    
    void onAnalog2(uint16_t uval, float fval){
      //Serial.print("AN2 ");Serial.printf("%0.4f\n",fval);
      //analog 2 controls the resynthisized signal biquad output filter
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_3"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_6"));
      AudioNoInterrupts();
      filter->setLowpass(0,220.0 + (6000.0 * fval));
      mixer->gain(0,2.0);
      AudioInterrupts();
    };
    
    void onAnalog3(uint16_t uval, float fval){
      //Serial.print("AN3 ");Serial.printf("%0.4f\n",fval);
      //analog 3 controls the dry signal biquad output filter and additional gain stage (post cqt)
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_4"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_1"));
      float lp = 300.0 + (18000.0 * log1p(fval));
      float hp = 210.0 + (100.0 * log1p(fval));
      float gain = 1.0 + log1p(fval);
      AudioNoInterrupts();
      filter->setLowpass(0,lp);
      filter->setHighpass(1,hp);
      mixer->gain(2,gain);
      AudioInterrupts();
    };

    void onAnalog4(uint16_t uval, float fval){
      //Serial.print("AN4 ");Serial.printf("%0.4f\n",fval);
      //output volume
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp_1"));
      float gain = log1p(fval);
      AudioNoInterrupts();
      amp->gain(gain);
      AudioInterrupts();
    };
    
    void makeAudioConnections(){
      AudioNoInterrupts();
      ad->disconnectAll();      
      //input to input amplifier      
      ad->connect("i2s-in_1 1 amp_2 0");

      //amplified input -> filter -> master mixer
      ad->connect("amp_2 0 biquad_4 0");
      ad->connect("biquad_4 0 mixer_1 2");
      
      //master mixer -> output amp
      ad->connect("mixer_1 0 amp_1 0");
      ad->connect("amp_1 0 i2s-out_1 0");

      //16 input bank bus mixer structure (4in x 4blocks)
      ad->connect("mixer_2 0 mixer_6 0");
      ad->connect("mixer_3 0 mixer_6 1");
      ad->connect("mixer_4 0 mixer_6 2");
      ad->connect("mixer_5 0 mixer_6 3");

      //bus output to filter -> reverb -> master mixer
      ad->connect("mixer_6 0 biquad_3 0");
      ad->connect("biquad_3 0 freeverb_1 0");
      ad->connect("freeverb_1 0 mixer_1 1");
      //filtered bus mixer -> master mixer
      ad->connect("biquad_3 0 mixer_1 0");
      
      //connect the oscillators to the bus mixer
      ad->connect("waveform_1 0 mixer_2 0");
      ad->connect("waveform_2 0 mixer_2 1");
      ad->connect("waveform_3 0 mixer_2 2");
      ad->connect("waveform_4 0 mixer_2 3");
      
      ad->connect("waveform_5 0 mixer_3 0");
      ad->connect("waveform_6 0 mixer_3 1");
      ad->connect("waveform_7 0 mixer_3 2");
      ad->connect("waveform_8 0 mixer_3 3");
      
      ad->connect("waveform_9 0 mixer_4 0");
      ad->connect("waveform_10 0 mixer_4 1");
      ad->connect("waveform_11 0 mixer_4 2");
      ad->connect("waveform_12 0 mixer_4 3");
      
      ad->connect("waveform_13 0 mixer_5 0");
      ad->connect("waveform_14 0 mixer_5 1");
      ad->connect("waveform_15 0 mixer_5 2");
      ad->connect("waveform_16 0 mixer_5 3");

      //fft connections
      ad->connect("amp_2 0 biquad_2 0");
      ad->connect("biquad_1 0 fft1024_1 0"); //lp filter
      
      ad->connect("amp_2 0 biquad_1 0");
      ad->connect("biquad_2 0 fft1024_2 0"); //lp filter

      //input amp-> scope ch1
      ad->connect("amp_2 0 scope_1 0");
      //filtered bus mixer output -> scope ch2
      ad->connect("biquad_3 0 scope_1 1");
      AudioInterrupts();
      delay(10);
    }

    void changeVoice(uint16_t voice_type){
      char buffer[32];
      erisAudioSynthWaveform* w;
      
      
      for (int16_t i=1; i <= OSC_BANK_SIZE; i++){
        sprintf(buffer, "waveform_%d", i);
        w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        w->begin(voice_type);
        AudioInterrupts();
      }
      
      //change the voice of the test signal too
      w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform_16"));
      AudioNoInterrupts();
      w->begin(voice_type);
      AudioInterrupts();

      return;
    }
};