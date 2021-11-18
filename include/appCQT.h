/**
 * @file appCQT.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <float.h>
#include "AudioUtilities.h"
#include "AppManager.h"

/**
 * @brief CQT_HIGHRANGE_SPLIT_AT_BIN defines the cqt bin at which the high range fft will begin to be read from
 * 
 */

#define CQT_HIGHRANGE_SPLIT_AT_BIN 48

/**
 * @brief OSC_BANK_SIZE defins the MAX number of 'voices' used to resynthisize the input signal (LIMIT OF 16!)
 * 
 */
#define OSC_BANK_SIZE 16

/**
 * @brief periodically transmit the fft output buffer to the serial port
 * 
 */
#define TX_PERIODIC_FFT


//periods below selected from primes https://en.wikipedia.org/wiki/Periodical_cicadas

//transmit period in msec
#define TX_PERIOD 731

/**
 * @brief the period at which the some quantized voice data is sent to the serial port
 * 
 */
#define TX_CQT_PERIOD 567

// Constant Q Transform App
//

/**
 * @brief Implements the AppCQT class
 * 
 */
class AppCQT:public AppBaseClass {
  public:
    AppCQT():AppBaseClass(){
      osc_bank_size = OSC_BANK_SIZE;
      char buffer[32]; //used to build the stream names
      sprintf(name, "AppCQT"); //set the applications name
      AudioNoInterrupts();
      for (int16_t i=1; i < osc_bank_size+1; i++){
        sprintf(buffer, "waveform_%d", i);
        //request the object from the audio director
        osc[i] = (erisAudioSynthWaveform*) (ad->getAudioStreamObjByName(buffer));
        //init the object to the default state
        osc[i]->begin(0.0, 0, WAVEFORM_SINE);
      }
      AudioInterrupts();
      //take care to downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName("fft1024_1"));
      fft2 = (erisAudioAnalyzeFFT1024*) (ad->getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange(); //switch to low range
      //zero out the data variables
      memset(&fftRVal,0,sizeof(FFTReadRange));
      memset(&oscBank,0,sizeof(FFTReadRange)* OSC_BANK_SIZE);
      //init the data dictionary value(s)
      am->data->create("OCTAVE_DOWN_INTERVAL",0);
      //init the QCT bins by loading the frequency ranges for each music note using a look up table
      float flow;
      float fhigh;
      highRange = CQT_HIGHRANGE_SPLIT_AT_BIN;
      for (uint16_t i=0;i < NOTE_ARRAY_LENGTH;i++){
        flow = 0;
        fhigh = 0;
        if (i > 0 && i < NOTE_ARRAY_LENGTH){
          //calculate the high and low frequencys for the given note
          //this is done by splitting the frequency differences from the music note above and below 
          flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
          fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
        }else{
          flow = 0;
          fhigh = 0;
        }
        //zero out the destination within the array
        memset(&fftHighRR[i],0,sizeof(FFTReadRange));
        memset(&fftLowRR[i],0,sizeof(FFTReadRange));
        //write the ranges to the bins, split between the high and low range fft
        if (i >= highRange){
          fftHighRR[i].cqtBin =i;
          fftLowRR[i].cqtBin =i;
          
          fftHighRR[i].startFrequency = flow;
          fftLowRR[i].startFrequency =0;
          
          fftHighRR[i].stopFrequency =fhigh;
          fftLowRR[i].stopFrequency =0;  

        }
        else{
          fftHighRR[i].cqtBin =i;
          fftLowRR[i].cqtBin =i;
          
          fftHighRR[i].startFrequency =0;
          fftLowRR[i].startFrequency =flow;
          
          fftHighRR[i].stopFrequency =0;
          fftLowRR[i].stopFrequency =fhigh;            
        }
       }
      
      rt_calls = 0;
      update_calls = 0;
      pll_p=0.0;
      pll_f=1.0;
      //enable the fft blocks
      fft_buffer_serial_transmit_elapsed = 0;
      fft_buffer_select_for_serial_transmit = 0;
      cqt_serial_transmit_elapsed = 0;
      isActive = true;
      AudioNoInterrupts();
      fft->enableFFT(true);
      fft2->enableFFT(true);
      AudioInterrupts();
    }; 
  protected:
    bool isActive;
    double rt_calls;
    double update_calls;
    uint8_t fft_buffer_select_for_serial_transmit;
    elapsedMillis fft_buffer_serial_transmit_elapsed;
    elapsedMillis cqt_serial_transmit_elapsed;
    int16_t osc_bank_size;
    uint16_t highRange;
    float64_t pll_p;
    float64_t pll_f;
    erisAudioSynthWaveform* osc[OSC_BANK_SIZE];
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    FFTReadRange fftRVal __attribute__ ((aligned (4)));
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (32)));
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (32)));
    FFTReadRange oscBank[OSC_BANK_SIZE] __attribute__ ((aligned (32)));
    void update(){
      if (!isActive) return;
      update_calls++;
      #ifdef TX_PERIODIC_FFT
        if (fft_buffer_serial_transmit_elapsed > TX_PERIOD && Serial.availableForWrite() > 4000){
          fft_buffer_serial_transmit_elapsed = 0;
          if(fft_buffer_select_for_serial_transmit < 2){
            Serial.printf("S 512"); 
            for(int i = 0; i < 512; i+=1){
              //transmitt low range then high range
              if(Serial.availableForWrite() < 1000){delayMicroseconds(10000);}
              if(fft_buffer_select_for_serial_transmit == 0) Serial.printf(F(",%d"),(int)(100.0*fft2->output[i])); 
              if(fft_buffer_select_for_serial_transmit == 1) Serial.printf(F(",%d"),(int)(100.0*fft->output[i]));
              //if ((i % 127)==0) Serial.flush();
            }
            Serial.println("");
          }
          fft_buffer_select_for_serial_transmit += 1;
          if(fft_buffer_select_for_serial_transmit >= 2) 
          {
            fft_buffer_select_for_serial_transmit = 0;
            Serial.println("S FIN"); // end of series data
          }
          //Serial.flush();
        } 
      #endif
      if (cqt_serial_transmit_elapsed > TX_CQT_PERIOD && Serial.availableForWrite() > 2000){
        cqt_serial_transmit_elapsed = 0;   
        
        for (uint16_t i=0;i < osc_bank_size;i++){
          if (oscBank[i].cqtBin < highRange) Serial.printf(F("CQT_L %d,%s,%.0f,%.0f,%.2f,%.3f,%.3f\n"),oscBank[i].cqtBin,note_name[oscBank[i].cqtBin],oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].phase,oscBank[i].avgValueFast*10.0,oscBank[i].transientValue*100.0);
          if (oscBank[i].cqtBin >= highRange)Serial.printf(F("CQT_H %d,%s,%.0f,%.0f,%.2f,%.3f,%.3f\n"),oscBank[i].cqtBin,note_name[oscBank[i].cqtBin],oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].phase,oscBank[i].avgValueFast*10.0,oscBank[i].transientValue*100.0);
        }
        Serial.printf(F("CQT_EOF \n"));
        //Serial.flush();
      }
      
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(oscBank,osc_bank_size);
      draw->fillRoundRect(x,y,w,h,3,CL(0x07,0x00,0x10));
      //draw the scale lines
      draw->setFont(Arial_8);
      draw->setTextColor(ILI9341_DARKGREY);
      for(uint8_t i=1;i < 18; i++){
        draw->drawLine(x,y +  (h - (log1p((0.1 * i))) * h),x+w,y + (h - (log1p((0.1 * i))) * h),ILI9341_DARKCYAN);
        if (has_pop){
          draw->setCursor(x + i * 17,y+ (h - (log1p((0.1 * i))) * h) - 9);
          if (i < 17) draw->print((log1p((0.1 * i))));
        }
      }

      //draw the cqt bins
      const float im = (w-1)/(float)(sizeof(note_freq)/sizeof(note_freq[0]));
      uint16_t nx;
      float signal;
      float amp;
        
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){  
        amp = fftHighRR[i].peakValue;//fft->read(&fftHighRR[i]);
        signal = (log1p((amp))*10.0) * h;
        if (signal>(h-1)) signal = h-1;
        if (signal<0) signal = 0;
        nx = (uint16_t)(im*fftHighRR[i].cqtBin);
        //draw->fillRoundRect(x+nx,y,2,(uint16_t)signal,1,ILI9341_ORANGE); 
        draw->fillRoundRect(x+nx,y+h - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_CYAN);
        amp = fftLowRR[i].avgValueFast;//fft2->read(&fftLowRR[i]);
        signal = (log1p((amp))*10.0) * h;
        if (signal>(h-1)) signal = h-1;
        if (signal<0) signal = 0;
        nx = (uint16_t)(im*fftLowRR[i].cqtBin);
        draw->fillRoundRect(x+nx,y+h - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_MAGENTA);
      }

      //make a second pass but only draw the oscillators
      draw->setTextColor(ILI9341_GREENYELLOW);
      for (uint16_t i=0;i < osc_bank_size;i++){
        amp = oscBank[i].avgValueFast;//fft->read(&fftHighRR[i]);
        signal = (log1p((amp))*10.0) * h;
        if (signal<0) signal = 0;
        if (signal>(h-1)) signal = h-1;
        nx = (uint16_t)(im*oscBank[i].cqtBin);
        if (signal > 1) draw->fillRoundRect(x+nx,y+h - (uint16_t)signal,2,4,1,CL(0xFF,0xA0,(uint8_t)(200*oscBank[i].transientValue)));
        if (signal > 10 && has_pop){
          draw->setCursor(x+nx - 5,y+h - (uint16_t)signal - 15);
          draw->print(note_name[oscBank[i].cqtBin]);
          draw->setCursor(x+nx - 5,y+h - (uint16_t)signal - 25);  
          draw->printf("%.0f",oscBank[i].peakFrequency); 
        }
      }
 
      //draw the border
      draw->drawRoundRect(x,y,w,h,4,ILI9341_MAGENTA);
      
      if (fftHighRR[0].peakValue > fftLowRR[0].peakValue) {
        fftRVal = fftHighRR[0];
        draw->setTextColor(ILI9341_DARKCYAN);
      } else{
        fftRVal = fftLowRR[0];
        draw->setTextColor(ILI9341_MAGENTA);
      }
    }; //called only when the app is active
    
    void updateRT(){
      if (!isActive) return;
      rt_calls++;
      //if (rt_calls < 10000) return;    
      if (fft2->capture() && fft->capture()){
        AudioNoInterrupts();
        fft2->analyze();
        fft->analyze();
        AudioInterrupts();
        updateOscillatorBank(true);
        updateOscillatorBank(false);
      }
      //Serial.flush();
    }; //allways called even if app is not active
    
    void onFocus(){ //called when given focus
      fft->enableFFT(true);
      fft2->enableFFT(true);
      isActive = true;
      Serial.println("M ** CQT ON FOCUS");

    };

    void onFocusLost(){ //called when focus is taken
      fft->enableFFT(false);
      fft2->enableFFT(false);
      isActive = false;
      Serial.println("M ** CQT LOST FOCUS");
    };

    void onTouch(uint16_t t_x, uint16_t t_y){
      //check if touch point is within the application bounding box
      if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
          //is touched
           if(!has_pop){
                //getFocus();
                requestPopUp(true);
                
            }else{
                //returnFocus();
                releasePopUp();
            }
      }
    };

    void onTouchRelease(uint16_t x, uint16_t y){
    };

    void updateOscillatorBank(bool low_range_switch){
      bool found;
      float floor;
      floor = 0.001;
      float peak_read=-1000;
      float peak;
      
      //sort the FFTReadRange array by cqt bin number
      if (low_range_switch) {erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);}
      else erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      //read the fft ranges, which stores the results into the FFTReadRange array
      for (uint16_t i=0; i < NOTE_ARRAY_LENGTH; i++){
        if (low_range_switch) peak = fft2->read(&fftLowRR[i]);
        else peak = fft->read(&fftHighRR[i]);
        if (peak>peak_read){
            peak_read = peak;
        }
      }
      //update the oscillator bank with the current values
      for (uint16_t i=0; i < osc_bank_size; i++){
        if (oscBank[i].cqtBin > highRange){
          oscBank[i] = fftHighRR[oscBank[i].cqtBin];
        }else oscBank[i] = fftLowRR[oscBank[i].cqtBin];
        //if values are below a threshold then clear the osc
        if(oscBank[i].transientValue < 0.001 && oscBank[i].avgValueSlow < floor){
          fftLowRR[0].avgValueFast = 0;
          fftLowRR[0].avgValueSlow = 0;
          fftLowRR[0].transientValue = 0;
          oscBank[i] = fftLowRR[0];
        }
      }
      //sort the updated cqt bins by peakValue
      if (low_range_switch){erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      } else erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);
      
      //Add the new osc settings
      //low range
      if (low_range_switch){
        for(uint16_t i=0; i < osc_bank_size; i++){
          found = false;
          for (uint16_t j=0; j < osc_bank_size; j++){
            if (oscBank[j].cqtBin == fftLowRR[i].cqtBin){
              oscBank[j] = fftLowRR[i];
              found = true;break;
            }
          }
          if(!found){
            for(int16_t k= osc_bank_size-1; k >= 0;k--){
              if (((oscBank[k].avgValueFast * 1.2) < fftLowRR[i].avgValueFast) && (fftLowRR[i].transientValue > (oscBank[k].transientValue * 1.2))){
                //fftLowRR[i].phase = 0;
                pll_p = 0.0;
                pll_f = 1.0;
                oscBank[k] = fftLowRR[i];
                break;
              } 
            }
          }
        }
      } else for(uint16_t i=0; i < osc_bank_size; i++){ //high range
          found = false;
          for (uint16_t j=0; j < osc_bank_size; j++){
            if (oscBank[j].cqtBin == fftHighRR[i].cqtBin){
              oscBank[j] = fftHighRR[i];
              found = true;break;
            }
          }
          if(!found){
            for(int16_t k= osc_bank_size-1; k >= 0;k--){
              if (((oscBank[k].avgValueFast * 1.2) < fftHighRR[i].avgValueFast) && (fftHighRR[i].transientValue > (oscBank[k].transientValue * 1.2))){
                //fftHighRR[i].phase = 0;
                pll_p = 0.0;
                pll_f = 1.0;
                oscBank[k] = fftHighRR[i];
                break;
              } 
            }
        }
      }
            
      pll_p = 0.0;
      pll_f = 1.0;

      //sort the updated cqt bins by peakValue
      if (low_range_switch){erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      } else erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);

      //take the phase from the dominant frequency component
      float dominantPhase = 0;
      if (low_range_switch){
          dominantPhase = fftLowRR[0].phase;
      } else dominantPhase = fftHighRR[0].phase;;

      //actually update the oscilators
      AudioNoInterrupts();
      for(int16_t i=0; i < osc_bank_size; i++){
        float a,f;
        float64_t phase_aligner;    
        if( ( (oscBank[i].cqtBin < highRange) && (low_range_switch == true)) || ((oscBank[i].cqtBin >= highRange) && (low_range_switch == false))){
          if (oscBank[i].peakFrequency > 30.0){
            f = oscBank[i].peakFrequency;           
            a = oscBank[i].avgValueFast;//(log1pf(oscBank[i].avgValueFast)/(log1pf(osc_bank_size)));
            if(!isnan(a)){
              if (a < floor) a = 0.0;
              //if (a > (1.0/(float)OSC_BANK_SIZE)) a = (1.0/(float)OSC_BANK_SIZE);
              phase_aligner = ((dominantPhase - oscBank[i].phase)/dominantPhase);
              phase_aligner = (phase_aligner * (float64_t)osc[i]->getPhase()) + pll_p;
              if(!isnan(phase_aligner)) phase_aligner=0;
              f = (pll_f * f * octave_down[am->data->read("OCTAVE_DOWN_INTERVAL")]);
              if (f < 20) f = 20;
              if (f > 20000) f = 20000;
              osc[i]->frequency(f);
              osc[i]->amplitude(log1p(a));
              osc[i]->phase(phase_aligner);
            } else osc[i]->amplitude(0);
          }
        }
      }
      AudioInterrupts();

      //resort so we leave the arrays in order by cqt bin
      //if (low_range_switch) {erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);}
      //else erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);

      return;
    }

  void MessageHandler(AppBaseClass *sender, const char *message){
    if(strcmp(message,"ENABLE")==0){
      onFocus();
    }else if(strcmp(message,"DISABLE")==0){
      onFocusLost();
    }

    if(sender->isName("SCI")){
      Serial.print(F("M appCQT::MessageHandler SCI param: "));
      Serial.println(message);
      //Serial.flush();
      if (strcmp(message,"CQT_INFO")==0){
        //sort the bins to transmit them in order
        erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
        erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
        for (uint16_t i=1;i< NOTE_ARRAY_LENGTH - 1;i++){
          Serial.printf(F("M L %d,%f,%f,%d,%d,%d\t\t\t"),fftLowRR[i].cqtBin,fftLowRR[i].startFrequency,fftLowRR[i].stopFrequency,fftLowRR[i].startBin,fftLowRR[i].stopBin,fftLowRR[i].stopBin-fftLowRR[i].startBin);
          Serial.printf(F("M H %d,%f,%f,%d,%d,%d\n"),fftHighRR[i].cqtBin,fftHighRR[i].startFrequency,fftHighRR[i].stopFrequency,fftHighRR[i].startBin,fftHighRR[i].stopBin,fftHighRR[i].stopBin-fftHighRR[i].startBin);
        }
        //Serial.flush();
      }
    }
  }
};