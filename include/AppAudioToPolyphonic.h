/**
 * @file appAudioToPolyPhonic.h
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


const char APPNAME[] PROGMEM = "POLY";
const char REVERB_ROOM_SIZE[] PROGMEM = "REVERB_ROOM_SIZE";
const char REVERB_DAMPING[] PROGMEM = "REVERB_DAMPING";

class AppAudioToPolyphonic:public AppBaseClass {
  public:
    AppCQT cqt;
    AppScope oscope;
    ControlButton *button;
    ControlSlider *slider;
    int16_t x_end,x_start;
    int16_t y_end,y_start;
    int16_t x_last,y_last,y_last_scope;
    uint32_t t_lastupdate;

    AppAudioToPolyphonic():AppBaseClass(){
      initalize();
    };

    void FLASHMEM initalize(){
      sprintf(name, APPNAME);
      Serial.println(F("MyApp constructor called"));

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

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_5"));
      filter->setNotch(0,475,2.0);
      
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad->getAudioStreamObjByName("freeverb_1"));
      am->data->create("REVERB_ROOM_SIZE",(float32_t)0.88);
      am->data->create("REVERB_DAMPING",(float32_t)0.37);
      reverb->roomsize(am->data->readf("REVERB_ROOM_SIZE"));
      reverb->damping(am->data->readf("REVERB_DAMPING"));
      
      erisAudioMixer4* mix = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_1"));
      mix->gain(0,1.0);
      mix->gain(1,0.40);
      mix->gain(3,0.40);

      output_gate = 1.0;

      erisAudioEffectDelay* delay = (erisAudioEffectDelay*)(ad->getAudioStreamObjByName("delay_1"));
      delay->delay(0,30);
      delay->delay(1,0);
      delay->delay(2,0);
      delay->delay(3,100);
      mix = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_2"));
      mix->gain(0,0.3);
      mix->gain(1,0.001);
      mix->gain(2,0.0001);
      mix->gain(3,0.7);
      
      //oscope = new AppScope;
      oscope.setWidgetPosition(5,5);
      oscope.setWidgetDimension(110,50);
      oscope.setPosition(5,20);
      oscope.setDimension(315,100);
      oscope.setParent(this);
 
      //cqt = new AppCQT;
      cqt.setWidgetPosition(125,5);
      cqt.setWidgetDimension(320-10-125,50);
      cqt.setParent(this);
      
      slider = new ControlSlider(this);
      slider->setWidgetPosition(0,65);
      slider->setWidgetDimension(320,10);
      slider->setName("SLIDER");
      slider->setText("");
      slider->setValue(0);


      uint8_t si = 0;
      uint16_t bx = 5;
      uint16_t by = 90;
      for (uint16_t i=0;i<sizeof(s)/16;i+=1){
        button = new ControlButton(this); //reuse the button var to create many instances
        if(bx>(320-78)){
          bx=5;by+=78;
        }
        button->setWidgetPosition(bx,by);
        button->setWidgetDimension(66,66);
        button->setName(s[si]);
        button->setText(s[si]);
        button->setImage("/I/U/I",p[si++]);
        bx+=78;
      }
    } 
    //define event handlers
    void update(){
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad->getAudioStreamObjByName("freeverb_1"));
      reverb->roomsize(am->data->readf(REVERB_ROOM_SIZE));
      reverb->damping(am->data->readf(REVERB_DAMPING));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_2"));
      if(am->data->read(INPUT_PEAK) < 200){
        mixer->gain(0,0.3);
        mixer->gain(1,0.0);
        mixer->gain(2,0.0);
        mixer->gain(3,0.3);
      }else{
        mixer->gain(0,0.3);
        mixer->gain(1,0.0);
        mixer->gain(2,0.0);
        mixer->gain(3,0.3);
      }

      mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_6"));
      if(am->data->read(INPUT_PEAK) < 300){
        mixer->gain(0,0.3);
        mixer->gain(1,0.3);
        mixer->gain(2,0.0);
        mixer->gain(3,0.3);
      }else{
        mixer->gain(0,0.3);
        mixer->gain(1,0.3);
        mixer->gain(2,0.0);
        mixer->gain(3,0.3);

      }

      slider->setValue((int16_t)(100 * ((float)am->data->read(INPUT_PEAK))/16384.0));
    }

    void FLASHMEM onTouch(uint16_t t_x, uint16_t t_y){
      x_start = t_x;
      y_start = t_y;
      x_last = t_x;
      y_last = t_y;
      if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
      }
    }

    void FLASHMEM onTouchRelease(uint16_t t_x, uint16_t t_y){
      x_end = t_x;
      y_end = t_y;
      x_last = t_x;
      y_last = t_y;
      //draw->drawLine(x_start,y_start,x_end,y_end,ILI9341_ORANGE);
    }

    void FLASHMEM onTouchDrag(uint16_t t_x, uint16_t t_y){
      //Serial.println("MyApp:onTouchDrag");
      //draw->drawPixel(x,y,ILI9341_BLUE);
      //draw->drawPixel(x-1,y,ILI9341_RED);
      //draw->drawPixel(x,y-1,ILI9341_YELLOW);
      //draw->drawPixel(x+1,y,ILI9341_ORANGE);
      //draw->drawLine(x_last,y_last,x,y,ILI9341_WHITE);
      x_last = t_x;
      y_last = t_y;
    }
    void FLASHMEM MessageHandler(AppBaseClass *sender, const char *message){   
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
          erisAudioSynthWaveform* wav = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform_17"));
          //AudioNoInterrupts();
          wav->frequency(atoi(message));
          wav->amplitude(0.01);
          //AudioNoInterrupts();
        }
        else if(sender->isName("TEST")){
          AudioNoInterrupts();
          ad->disconnect("fft1024_1 0");
          ad->disconnect("fft1024_2 0");
          ad->connect("waveform_17 0 fft1024_1 0");
          ad->connect("waveform_17 0 fft1024_2 0");
          AudioInterrupts();
        }
    }

    void FLASHMEM onFocus(){
      makeAudioConnections();
      am->sendMessage(this,"CQT","ENABLE");
    }
    
    void FLASHMEM onAnalog1(uint16_t uval, float fval){
     // Serial.print("AN1 ");Serial.printf("%0.4f\n",fval);
      //analog 1 controls the dry signal input to the fft blocks (used by the cqt app)
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp_2"));
      amp->gain(log1p(fval));
    };
    
    void FLASHMEM onAnalog2(uint16_t uval, float fval){
      //Serial.print("AN2 ");Serial.printf("%0.4f\n",fval);
      //analog 2 controls the resynthisized signal biquad output filter
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_3"));
      filter->setLowpass(0,220.0 + (5000.0 * fval));
    };
    
    void FLASHMEM onAnalog3(uint16_t uval, float fval){
      float lp,hp,gain;
      //Serial.print("AN3 ");Serial.printf("%0.4f\n",fval);
      //analog 3 controls the dry signal biquad output filter and additional gain stage (post cqt)
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad_4"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer_1"));
      lp = 300.0 + (18000.0 * log1p(fval));
      hp = 210.0 + (100.0 * log1p(fval));
      gain = log1p(fval);
      filter->setLowpass(0,lp);
      filter->setHighpass(1,hp);
      mixer->gain(2,gain);
    };

    void FLASHMEM onAnalog4(uint16_t uval, float fval){
      //Serial.print("AN4 ");Serial.printf("%0.4f\n",fval);
      //output volume
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp_1"));
      float gain = log1p(fval);
      amp->gain(1.0 + gain);
    };
    
    void FLASHMEM makeAudioConnections(){
      AudioNoInterrupts();
      ad->disconnectAll();      
      //input to input amplifier      
      ad->connect("i2s-in_1 1 amp_2 0");

      //amplified input -> filter -> master mixer
      ad->connect("amp_2 0 biquad_4 0");
      //ad->connect("amp_2 0 mixer_1 1");
      ad->connect("biquad_4 0 mixer_1 2");
      
      //master mixer -> output amp
      ad->connect("mixer_1 0 amp_1 0");
      ad->connect("amp_1 0 i2s-out_1 0");

      //16 input bank bus mixer structure (4in x 4blocks)
      ad->connect("console_1 0 mixer_6 0");
      ad->connect("console_2 0 mixer_6 1");
      
      //bus output to filter -> reverb -> master mixer
      //ad->connect("mixer_6 0 amp_1 0");
      ad->connect("mixer_6 0 biquad_3 0");
      ad->connect("biquad_3 0 freeverb_1 0");
      ad->connect("freeverb_1 0 mixer_1 4");
      ad->connect("freeverb_1 0 delay_1 0");

      //filtered bus mixer -> master mixer
      ad->connect("biquad_3 0 mixer_1 0");
      
      //connect the oscillators to console 1
      ad->connect("waveform_1 0 console_1 0");
      ad->connect("waveform_2 0 console_1 1");
      ad->connect("waveform_3 0 console_1 2");
      ad->connect("waveform_4 0 console_1 3");
      ad->connect("waveform_5 0 console_1 4");
      ad->connect("waveform_6 0 console_1 5");
      ad->connect("waveform_7 0 console_1 6");
      ad->connect("waveform_8 0 console_1 7");
      
      ad->connect("waveform_9 0 console_2 0");
      ad->connect("waveform_10 0 console_2 1");
      ad->connect("waveform_11 0 console_2 2");
      ad->connect("waveform_12 0 console_2 3");
      ad->connect("waveform_13 0 console_2 4");
      ad->connect("waveform_14 0 console_2 5");
      ad->connect("waveform_15 0 console_2 6");
      ad->connect("waveform_16 0 console_2 7");

      //fft connections
      ad->connect("amp_2 0 biquad_2 0");
      ad->connect("biquad_1 0 fft1024_1 0"); //lp filter
      
      ad->connect("amp_2 0 biquad_1 0");
      ad->connect("biquad_2 0 fft1024_2 0"); //lp filter

      //input amp-> scope ch1
      ad->connect("amp_2 0 scope_1 0");
      //filtered bus mixer output -> scope ch2
      ad->connect("biquad_3 0 scope_1 1");

      //delay connections
      
      ad->connect("delay_1 0 mixer_2 0");
      ad->connect("delay_1 1 mixer_2 1");
      ad->connect("delay_1 2 mixer_2 2");
      ad->connect("delay_1 3 mixer_2 3");

      ad->connect("mixer_2 0 biquad_5 0");
      ad->connect("biquad_5 0 mixer_6 3");

      AudioInterrupts();
      delay(10);
    }

    void FLASHMEM changeVoice(uint16_t voice_type){
      char buffer[32];
      erisAudioSynthWaveform* w;
      
      for (int16_t i=1; i <= OSC_BANK_SIZE; i++){
        sprintf(buffer, "waveform_%d", i);
        w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(buffer));
        w->begin(voice_type);
      }
      
      //change the voice of the test signal too
      w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform_17"));
      w->begin(voice_type);
      return;
    }
  protected:
    float output_gate;
};