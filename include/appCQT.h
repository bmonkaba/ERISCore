#include "AudioUtilities.h"
#include "AppManager.h"

#define OSC_BANK_SIZE 12

// Constant Q Transform App
//

class AppCQT:public AppBaseClass {
  public:
    AppCQT():AppBaseClass(){
      char buffer[32];
      sprintf(name, "AppCQT");
      for (int16_t i=0; i < OSC_BANK_SIZE; i++){
        sprintf(buffer, "waveform_%d", i);
        osc[i] = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        if(i < OSC_BANK_SIZE/2){osc[i]->begin(0.0, 440, WAVEFORM_SAWTOOTH);}
        else osc[i]->begin(0.0, 440, WAVEFORM_SAWTOOTH);
        AudioInterrupts();
      }

      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange();
      

      memset(&fftRVal,0,sizeof(FFTReadRange));
      memset(&oscBank,0,sizeof(FFTReadRange)* OSC_BANK_SIZE);

      //init the QCT bins
      float flow;
      float fhigh;
      highRange = 42;

      for (uint16_t i=0;i < NOTE_ARRAY_LENGTH - 1;i++){
        flow = 0;
        fhigh = 0;
        if (i != 0){
          flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
          fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
        }
        memset(&fftHighRR[i],0,sizeof(FFTReadRange));
        memset(&fftLowRR[i],0,sizeof(FFTReadRange));
        
        if (i >= highRange){
          fftHighRR[i].cqtBin =i;
          fftLowRR[i].cqtBin =0;
          
          fftHighRR[i].startFrequency = flow;
          fftLowRR[i].startFrequency =0;
          
          fftHighRR[i].stopFrequency =fhigh;
          fftLowRR[i].stopFrequency =0;  

        }
        if (i < highRange){
          fftHighRR[i].cqtBin =0;
          fftLowRR[i].cqtBin =i;
          
          fftHighRR[i].startFrequency =0;
          fftLowRR[i].startFrequency =flow;
          
          fftHighRR[i].stopFrequency =0;
          fftLowRR[i].stopFrequency =fhigh;            
        }
        /*
        if (i > 80){
          memset(&fftHighRR[i],0,sizeof(FFTReadRange));
          memset(&fftLowRR[i],0,sizeof(FFTReadRange));
        }
        if (i < 35){
          memset(&fftHighRR[i],0,sizeof(FFTReadRange));
          memset(&fftLowRR[i],0,sizeof(FFTReadRange));
        }
        */
       }
      
      //updateOscillatorBank();
      /*
      Serial.flush();
      for (uint16_t i=1;i< NOTE_ARRAY_LENGTH - 1;i++){
        Serial.printf(F("L %d,%f,%f,%d,%d\n"),fftLowRR[i].cqtBin,fftLowRR[i].startFrequency,fftLowRR[i].stopFrequency,fftLowRR[i].startBin,fftLowRR[i].stopBin);
        Serial.printf(F("H %d,%f,%f,%d,%d\n"),fftHighRR[i].cqtBin,fftHighRR[i].startFrequency,fftHighRR[i].stopFrequency,fftHighRR[i].startBin,fftHighRR[i].stopBin);
      }
      Serial.flush();
      */
      rt_calls = 0;
      update_calls = 0;
      AudioNoInterrupts();
      fft_buffer_serial_transmit_elapsed = 0;
      cqt_serial_transmit_elapsed = 0;
      fft->enableFFT(true);
      fft2->enableFFT(true);
      AudioInterrupts();
    }; 
  protected:
    double rt_calls;
    double update_calls;
    elapsedMillis fft_buffer_serial_transmit_elapsed;
    elapsedMillis cqt_serial_transmit_elapsed;
    uint16_t highRange; 
    erisAudioSynthWaveform* osc[OSC_BANK_SIZE];
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    FFTReadRange fftRVal __attribute__ ((aligned (4)));
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));
    FFTReadRange oscBank[OSC_BANK_SIZE] __attribute__ ((aligned (4)));
    void update(){
      update_calls++;
      //draw the cqt bins
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        uint16_t nx;
        float signal;
        float amp;
        float im = (width-1)/(float)(sizeof(note_freq)/sizeof(note_freq[0]));
        amp = fftHighRR[i].avgValueFast;//fft->read(&fftHighRR[i]);
        signal = log(amp*0.707) * height/10;
        nx = (uint16_t)(im*fftHighRR[i].cqtBin);
        tft.fillRoundRect(origin_x+nx,origin_y,2,(uint16_t)signal,1,ILI9341_ORANGE); 
        amp = fftLowRR[i].avgValueFast;//fft2->read(&fftLowRR[i]);
        signal = log(amp*0.707) * height/10;
        nx = (uint16_t)(im*fftLowRR[i].cqtBin);
        tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_MAGENTA);
      }
      //draw the border
      tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
      
      if (fftHighRR[0].peakValue > fftLowRR[0].peakValue) {
        fftRVal = fftHighRR[0];
        tft.setTextColor(ILI9341_DARKCYAN);
      } else{
        fftRVal = fftLowRR[0];
        tft.setTextColor(ILI9341_MAGENTA);
      }
    }; //called only when the app is active
    
    void updateRT(){
      rt_calls++;
      if (rt_calls < 10000) return;    
      //AudioNoInterrupts();
      if (fft->capture()){
        fft->analyze();
        updateOscillatorBank(false);
      } 
      if (fft2->capture()){
        fft2->analyze();
        updateOscillatorBank(true);
      }
      //AudioInterrupts();
      
      if (fft_buffer_serial_transmit_elapsed > 250){
        fft_buffer_serial_transmit_elapsed = 0;
        Serial.flush();
        Serial.printf("S 1024"); 
        for(int i = 0; i < 1024; i+=1){
          Serial.printf(F(",%d"),128+ (int)(255.0 * (0.01 * fft->output[i])));
        }
        Serial.println("");
      }
      //Serial.flush();
    }; //allways called even if app is not active
    
    void onFocus(){};   //called when given focus
    void onFocusLost(){}; //called when focus is taken
    void onTouch(uint16_t x, uint16_t y){
        //check if touch point is within the application bounding box
        if (x > origin_x && x < (origin_x + width) && y > origin_y && y < (origin_y + height)){
            //is touched
        }
    };
    void onTouchRelease(uint16_t x, uint16_t y){
    };

    void updateOscillatorBank(bool low_range_switch){
      bool found;
      float floor;
      floor = 0.0001;

      if (low_range_switch){
        erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      } else erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      
      for (uint16_t i=0; i < NOTE_ARRAY_LENGTH; i++){
        if (low_range_switch){fft2->read(&fftLowRR[i]); 
        } else fft->read(&fftHighRR[i]);
      }

      //STEP 1 - UPDATE current oscillators
      for(uint16_t i=0; i < OSC_BANK_SIZE; i++){
          if (oscBank[i].cqtBin >= highRange && !low_range_switch){oscBank[i] = fftHighRR[oscBank[i].cqtBin];
          } else if ((oscBank[i].cqtBin > 5) && (oscBank[i].cqtBin < highRange) && low_range_switch) {
            oscBank[i] = fftLowRR[oscBank[i].cqtBin];
          }
      }

      //sort the updated cqt bins by peakValue
      if (low_range_switch){erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      } else erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);

      erisAudioAnalyzeFFT1024::sort_fftrr_by_value(oscBank,OSC_BANK_SIZE);
      
      //STEP 2 - Add the new osc settings
      //low range
      if (low_range_switch){
        for(uint16_t i=0; i < OSC_BANK_SIZE/2; i++){
          found = false;
          for (uint16_t j=0; j < OSC_BANK_SIZE; j++){
            if (oscBank[j].cqtBin == fftLowRR[i].cqtBin){found = true;break;}
          }
          if(!found){
            for(uint16_t k= OSC_BANK_SIZE - 1; k > 0; k--){
              if ((fftLowRR[i].peakValue > floor) && (oscBank[k].peakValue < fftLowRR[i].peakValue)){
                oscBank[k] = fftLowRR[i];
                break;
              } 
            }
          }
        }
      } else for(uint16_t i=0; i < OSC_BANK_SIZE/2; i++){ //high range
        found = false;
        for (uint16_t j=0; j < OSC_BANK_SIZE; j++){
          if (oscBank[j].cqtBin == fftHighRR[i].cqtBin){found = true;break;}
        }
        if(!found){
          for(uint16_t k= OSC_BANK_SIZE - 1; k > 0; k--){
            if ((fftHighRR[i].peakValue > floor) && (oscBank[k].peakValue < fftHighRR[i].peakValue)){
               oscBank[k] = fftHighRR[i];
               break;
            } 
          }
        }
      }
      
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(oscBank,OSC_BANK_SIZE);
      
      //STEP 3 - now actually set the oscilators
      //AudioNoInterrupts();
      for(int16_t i=0; i < OSC_BANK_SIZE; i++){
        float a;    
        if( ( (oscBank[i].cqtBin < highRange) && (low_range_switch == true)) || ((oscBank[i].cqtBin >= highRange) && (low_range_switch == false))){
          //osc[i]->frequency(note_freq[oscBank[i].cqtBin]); 
          osc[i]->frequency(oscBank[i].peakFrequency/2.0);
          a = log(sqrt(oscBank[i].peakValue))/300.0;
          osc[i]->amplitude(a);
          osc[i]->phase(oscBank[i].phase/4.0);
        }
      }
      //AudioInterrupts();

      if (cqt_serial_transmit_elapsed > 100){
        cqt_serial_transmit_elapsed = 0;   
        Serial.flush(); 
        for (uint16_t i=0;i< (OSC_BANK_SIZE);i++){
          if (oscBank[i].cqtBin < highRange) Serial.printf(F("CQT_L %d,%.0f,%.0f,%.0f,%.2f,%d\n"),oscBank[i].cqtBin,oscBank[i].estimatedFrequency,oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].peakValue,oscBank[i].stopBin - oscBank[i].startBin);
          if (oscBank[i].cqtBin >= highRange) Serial.printf(F("CQT_H %d,%.0f,%.0f,%.0f,%.2f,%d\n"),oscBank[i].cqtBin,oscBank[i].estimatedFrequency,oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].peakValue,oscBank[i].stopBin - oscBank[i].startBin);
        }
        Serial.printf(F("CQT_EOF \n"));
        //Serial.flush();
      }

      //resort so we leave the arrays in order by cqt bin
      if (low_range_switch) {erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);}
      else erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);

      return;
    }
};