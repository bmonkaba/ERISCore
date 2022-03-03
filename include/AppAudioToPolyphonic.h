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

/**
 * @brief this is the main example application
 * here you can see how to make audio connections, respond to events and messages, etc..
 */
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
      update_priority = 10; //set to higher priority
      id = 1;
      t_lastupdate = micros();

      x_end = 0;
      x_start = 0;
      y_end = 0;
      y_start = 0;
      x_last = 0;
      y_last = 0;
      y_last_scope = 0;
      
      erisAudioSynthNoisePink* pink = (erisAudioSynthNoisePink*) (ad->getAudioStreamObjByName("pink:1"));
      pink->amplitude(1.0);
      
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:1"));
      filter->setLowpass(0, 1100);
      filter->setHighpass(1, 270);

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:2"));
      filter->setLowpass(0, 300);

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:3"));
      filter->setLowpass(0,8052);
      
      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:4"));
      filter->setLowpass(0,18400);

      filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:5"));
      filter->setNotch(0,475,2.0);
      
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad->getAudioStreamObjByName("freeverb:1"));
      am->data->create("REVERB_ROOM_SIZE",(float32_t)0.88);
      am->data->create("REVERB_DAMPING",(float32_t)0.37);
      reverb->roomsize(am->data->readf("REVERB_ROOM_SIZE"));
      reverb->damping(am->data->readf("REVERB_DAMPING"));
      
      erisAudioMixer4* mix = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer:1"));
      mix->gain(0,1.0);
      mix->gain(1,0.40);
      mix->gain(3,0.40);

      output_gate = 1.0;

      erisAudioEffectDelay* delay = (erisAudioEffectDelay*)(ad->getAudioStreamObjByName("delay:1"));
      delay->delay(0,30);
      delay->delay(1,0);
      delay->delay(2,0);
      delay->delay(3,100);
      mix = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer:2"));
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
      cqt.setWidgetDimension(SCREEN_WIDTH-10-125,50);
      cqt.setParent(this);
      
      slider = new ControlSlider(this);
      slider->setWidgetPosition(0,65);
      slider->setWidgetDimension(SCREEN_WIDTH,10);
      slider->setName("SLIDER");
      slider->setText("");
      slider->setValue(0);


      uint8_t si = 0;
      uint16_t bx = 5;
      uint16_t by = 90;
      for (uint16_t i=0;i<sizeof(s)/16;i+=1){
        button = new ControlButton(this); //reuse the button var to create many instances
        if(bx>(SCREEN_WIDTH-78)){
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
    void FLASHMEM render(){
      erisAudioEffectFreeverb* reverb = (erisAudioEffectFreeverb*)(ad->getAudioStreamObjByName("freeverb:1"));
      reverb->roomsize(am->data->readf(REVERB_ROOM_SIZE));
      reverb->damping(am->data->readf(REVERB_DAMPING));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer:2"));
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

      mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer:6"));
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
    void FLASHMEM messageHandler(AppBaseClass *sender, const char *message){   
        if (sender == slider){ //can detect message sender by ptr...
          //erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:3"));
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
          erisAudioSynthWaveform* wav = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform:17"));
          //AudioNoInterrupts();
          wav->frequency(atoi(message));
          wav->amplitude(0.01);
          //AudioNoInterrupts();
        }
        else if(sender->isName("BACK")){
           am->sendMessage(this,"AppWren","DEMO");
        }
    }

    void FLASHMEM onFocus(){
      makeAudioConnections();
      am->sendMessage(this,"CQT","ENABLE");
    }
    
    void FLASHMEM onAnalog1(uint16_t uval, float fval){
     // Serial.print("AN1 ");Serial.printf("%0.4f\n",fval);
      //analog 1 controls the dry signal input to the fft blocks (used by the cqt app)
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp:2"));
      amp->gain(1 + log1p(fval));
    };
    
    void FLASHMEM onAnalog2(uint16_t uval, float fval){
      //Serial.print("AN2 ");Serial.printf("%0.4f\n",fval);
      //analog 2 controls the resynthisized signal biquad output filter
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:3"));
      filter->setLowpass(0,220.0 + (5000.0 * fval));
    };
    
    void FLASHMEM onAnalog3(uint16_t uval, float fval){
      float lp,hp,gain;
      //Serial.print("AN3 ");Serial.printf("%0.4f\n",fval);
      //analog 3 controls the dry signal biquad output filter and additional gain stage (post cqt)
      erisAudioFilterBiquad* filter = (erisAudioFilterBiquad*) (ad->getAudioStreamObjByName("biquad:4"));
      erisAudioMixer4* mixer = (erisAudioMixer4*)(ad->getAudioStreamObjByName("mixer:1"));
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
      erisAudioAmplifier* amp = (erisAudioAmplifier*)(ad->getAudioStreamObjByName("amp:1"));
      float gain = log1p(fval);
      amp->gain(1.0 + gain);
    };
    
    void FLASHMEM makeAudioConnections(){
      AudioNoInterrupts();
      ad->disconnectAll();      
      //input to input amplifier      
      ad->connect("i2s-in:1 1 amp:2 0");

      //amplified input -> filter -> master mixer
      ad->connect("amp:2 0 biquad:4 0");
      //ad->connect("amp:2 0 mixer:1 1");
      ad->connect("biquad:4 0 mixer:1 2");
      
      //master mixer -> output amp
      ad->connect("mixer:1 0 amp:1 0");
      ad->connect("amp:1 0 i2s-out:1 0");

      //16 input bank bus mixer structure (4in x 4blocks)
      ad->connect("console:1 0 mixer:6 0");
      ad->connect("console:2 0 mixer:6 1");
      
      //bus output to filter -> reverb -> master mixer
      //ad->connect("mixer:6 0 amp:1 0");
      ad->connect("mixer:6 0 biquad:3 0");
      ad->connect("biquad:3 0 freeverb:1 0");
      ad->connect("freeverb:1 0 mixer:1 4");
      ad->connect("freeverb:1 0 delay:1 0");

      //filtered bus mixer -> master mixer
      ad->connect("biquad:3 0 mixer:1 0");
      
      //connect the oscillators to console 1
      ad->connect("waveform:1 0 console:1 0");
      ad->connect("waveform:2 0 console:1 1");
      ad->connect("waveform:3 0 console:1 2");
      ad->connect("waveform:4 0 console:1 3");
      ad->connect("waveform:5 0 console:1 4");
      ad->connect("waveform:6 0 console:1 5");
      ad->connect("waveform:7 0 console:1 6");
      ad->connect("waveform:8 0 console:1 7");
      
      ad->connect("waveform:9 0 console:2 0");
      ad->connect("waveform:10 0 console:2 1");
      ad->connect("waveform:11 0 console:2 2");
      ad->connect("waveform:12 0 console:2 3");
      ad->connect("waveform:13 0 console:2 4");
      ad->connect("waveform:14 0 console:2 5");
      ad->connect("waveform:15 0 console:2 6");
      ad->connect("waveform:16 0 console:2 7");

      //fft connections
      ad->connect("amp:2 0 biquad:2 0");
      ad->connect("biquad:1 0 fft1024:1 0"); //lp filter
      
      ad->connect("amp:2 0 biquad:1 0");
      ad->connect("biquad:2 0 fft1024:2 0"); //lp filter

      //input amp-> scope ch1
      ad->connect("amp:2 0 scope:1 0");
      //filtered bus mixer output -> scope ch2
      ad->connect("biquad:3 0 scope:1 1");

      //delay connections
      ad->connect("delay:1 0 mixer:2 0");
      ad->connect("delay:1 1 mixer:2 1");
      ad->connect("delay:1 2 mixer:2 2");
      ad->connect("delay:1 3 mixer:2 3");
      
      ad->connect("mixer:2 0 biquad:5 0");
      ad->connect("biquad:5 0 mixer:6 3");
      AudioInterrupts();
    }

    void FLASHMEM changeVoice(uint16_t voice_type){
      char buffer[32];
      erisAudioSynthWaveform* w;
      
      for (int16_t i=1; i <= OSC_BANK_SIZE; i++){
        sprintf(buffer, "waveform:%d", i);
        w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(buffer));
        w->begin(voice_type);
      }
      
      //change the voice of the test signal too
      w = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName("waveform:17"));
      w->begin(voice_type);
      return;
    }
  protected:
    float output_gate;
};