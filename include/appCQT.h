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

#define CQT_HIGHRANGE_SPLIT_AT_BIN 47

/**
 * @brief OSC_BANK_SIZE defins the MAX number of 'voices' used to resynthisize the input signal (LIMIT OF 16!)
 * 
 */
#define OSC_BANK_SIZE 15

/**
 * @brief periodically transmit the fft output buffer to the serial port
 * 
 */
//#define TX_PERIODIC_FFT

//transmit period in msec
#define TX_PERIOD 500

/**
 * @brief the period at which the some quantized voice data is sent to the serial port
 * 
 */
#define TX_CQT_PERIOD 250


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
      char buffer[16]; //used to build the stream names
      sprintf(name, "AppCQT"); //set the applications name
      for (int16_t i=0; i < osc_bank_size; i++){
        sprintf(buffer, "waveform_%d", i);
        //request the object from the audio director
        osc[i] = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        //init the object to the default state
        osc[i]->begin(0.0, 0, WAVEFORM_SINE);
        AudioInterrupts();
      }

      //take care to downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange(); //switch to low range

      //zero out the data variables
      memset(&fftRVal,0,sizeof(FFTReadRange));
      memset(&oscBank,0,sizeof(FFTReadRange)* OSC_BANK_SIZE);

      //init the QCT bins by loading the frequency ranges for each music note using a look up table
      float flow;
      float fhigh;
      highRange = CQT_HIGHRANGE_SPLIT_AT_BIN;

      for (uint16_t i=0;i < NOTE_ARRAY_LENGTH;i++){
        flow = 0;
        fhigh = 0;
        if (i != 0 && i != NOTE_ARRAY_LENGTH-2){
          //calculate the high and low frequencys for the given note
          //this is done by splitting the frequency differences from the music note above and below 
          flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
          fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
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
          fftHighRR[i].cqtBin =0;
          fftLowRR[i].cqtBin =i;
          
          fftHighRR[i].startFrequency =0;
          fftLowRR[i].startFrequency =flow;
          
          fftHighRR[i].stopFrequency =0;
          fftLowRR[i].stopFrequency =fhigh;            
        }
       }
      
      rt_calls = 0;
      update_calls = 0;
      
      //enable the fft blocks
      AudioNoInterrupts();
      fft_buffer_serial_transmit_elapsed = 0;
      cqt_serial_transmit_elapsed = 0;
      fft->enableFFT(true);
      fft2->enableFFT(true);
      AudioInterrupts();
      isActive = true;
    }; 
  protected:
    bool isActive;
    double rt_calls;
    double update_calls;
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
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));
    FFTReadRange oscBank[OSC_BANK_SIZE] __attribute__ ((aligned (4)));
    void update(){
      if (!isActive) return;
      update_calls++;
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      tft.fillRoundRect(origin_x,origin_y,width,height,3,CL(12,0,20));
      //draw the cqt bins
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        uint16_t nx;
        float signal;
        float amp;
        float im = (width-1)/(float)(sizeof(note_freq)/sizeof(note_freq[0]));
        amp = fftHighRR[i].avgValueFast;//fft->read(&fftHighRR[i]);
        signal = 5*(log(1.0 + (amp))/1.04139268516) * height;
        nx = (uint16_t)(im*fftHighRR[i].cqtBin);
        //tft.fillRoundRect(origin_x+nx,origin_y,2,(uint16_t)signal,1,ILI9341_ORANGE); 
        tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_CYAN);
        amp = fftLowRR[i].avgValueFast;//fft2->read(&fftLowRR[i]);
        signal = 5*(log(1.0 + (amp))/1.04139268516) * height;
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
      
      #ifdef TX_PERIODIC_FFT
      if (fft_buffer_serial_transmit_elapsed > TX_PERIOD){
        fft_buffer_serial_transmit_elapsed = 0;
        Serial.flush();
        Serial.printf("S 1024"); 
        for(int i = 0; i < 1024; i+=1){
          Serial.printf(F(",%d"),128+ (int)(255.0 * (0.01 * fft->output[i])));
        }
        Serial.println("");
      }
      #endif
      //Serial.flush();
    }; //allways called even if app is not active
    
    void onFocus(){ //called when given focus
      fft->enableFFT(true);
      fft2->enableFFT(true);
      isActive = true;
    };

    void onFocusLost(){ //called when focus is taken
      fft->enableFFT(false);
      fft2->enableFFT(false);
      isActive = false;
    };

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
      floor = 0.00010;
      
      for (uint16_t i=0; i < NOTE_ARRAY_LENGTH; i++){
        if (low_range_switch){fft2->read(&fftLowRR[i]); 
        } else fft->read(&fftHighRR[i]);
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
            for(uint16_t k= osc_bank_size-1; k > 0;k--){
              if ((fftLowRR[i].transientValue > 0.00005 ) && (fftLowRR[i].avgValueFast > floor) && (oscBank[k].avgValueFast < (0.20 * fftLowRR[i].avgValueFast))){
                fftLowRR[i].phase = 0;
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
          for(uint16_t k= osc_bank_size-1; k > 0;k--){
            if ((fftHighRR[i].transientValue > 0.00005) && (fftHighRR[i].peakValue > floor) && (oscBank[k].avgValueFast < (0.20 * fftHighRR[i].avgValueFast))){
              fftHighRR[i].phase = 0;
              pll_p = 0.0;
              pll_f = 1.0;
              oscBank[k] = fftHighRR[i];
              break;
            } 
          }
        }
      }
      

      //pll
      //pll_p =0;

      //
      if(AppManager::getInstance()->data.read("DOT") <40000 && abs(AppManager::getInstance()->data.read("EDGE_DELTA")) < 20 ){
        if((AppManager::getInstance()->data.read("EDGE_DELTA")) < -2 ) {
          pll_f += 0.00001 * abs(AppManager::getInstance()->data.read("EDGE_DELTA"));
          pll_p =0;
        }else if((AppManager::getInstance()->data.read("EDGE_DELTA")) > 2) {
          pll_f -= 0.00001 * abs(AppManager::getInstance()->data.read("EDGE_DELTA"));
          pll_p =0;
        }else if((AppManager::getInstance()->data.read("DOT_DELTA") < 1000)){
          pll_p -= 0.0008;// * AppManager::getInstance()->data.read("DOT_AVG") ;    
        }else if((AppManager::getInstance()->data.read("DOT_DELTA") > -1000)){
          pll_p += 0.0008;
        }else{
          if (pll_p < 180) {pll_p -= 0.00020;
          }else pll_p += 0.00020;
        }
      }
      /*
      if((AppManager::getInstance()->data.read("DOT_MACD")) < 0 ) {
        pll_p -= 0.0001;
      }else if((AppManager::getInstance()->data.read("DOT_MACD")) > 0 ) {
        pll_p += 0.0001;
      }
      */

      
      /*
      if((AppManager::getInstance()->data.read("EDGE_DELTA_MACD")) < 0 ) {
        pll_f += 0.00000005 
      }else if((AppManager::getInstance()->data.read("EDGE_DELTA_MACD")) > 0 ) {
        pll_f += 0.00000005 * AppManager::getInstance()->data.read("EDGE_DELTA_MACD");
      }
      */

      /*
      if(AppManager::getInstance()->data.read("DOT_AVG") < 0){
        pll -= 0.000001;
      }else if(AppManager::getInstance()->data.read("DOT_AVG") > 0){
        pll += 0.000001;
      }
      */
      if (pll_p>=360.0){pll_p -= 360.0;
      }else if (pll_p <= 0.0)pll_p += 360.0;

      //pll_p = 0;

      AppManager::getInstance()->data.update("PLL_P",(int32_t)(pll_p*1000000));
      AppManager::getInstance()->data.update("PLL_F",(int32_t)(pll_f*1000000));

      //actually update the oscilators
      AudioNoInterrupts();
      for(int16_t i=0; i < osc_bank_size; i++){
        float a,f;    
        if( ( (oscBank[i].cqtBin < highRange) && (low_range_switch == true)) || ((oscBank[i].cqtBin >= highRange) && (low_range_switch == false))){
          if (oscBank[i].peakFrequency > 30.0){
            //osc[i]->frequency(note_freq[oscBank[i].cqtBin]);
            f = oscBank[i].peakFrequency;

            osc[i]->frequency(pll_f * f * octave_up[0]);
            a = 0.707 * oscBank[i].avgValueFast/(osc_bank_size);
            if (f < 20.0) {f = 20.0;a=0;}
            if (a < floor) a = 0.0;
            if(!isnan(a)) osc[i]->amplitude(a);
            osc[i]->phase(osc[i]->getPhase() + pll_p);
          }
        }
      }
      AudioInterrupts();

      if (cqt_serial_transmit_elapsed > TX_CQT_PERIOD){
        cqt_serial_transmit_elapsed = 0;   
        
        for (uint16_t i=0;i < osc_bank_size;i++){
          if (oscBank[i].cqtBin < highRange) Serial.printf(F("CQT_L %d,%s,%.0f,%.0f,%.2f,%.5f,%.5f\n"),oscBank[i].cqtBin,note_name[oscBank[i].cqtBin],oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].phase,oscBank[i].avgValueFast,oscBank[i].transientValue);
          if (oscBank[i].cqtBin >= highRange)Serial.printf(F("CQT_H %d,%s,%.0f,%.0f,%.2f,%.5f,%.5f\n"),oscBank[i].cqtBin,note_name[oscBank[i].cqtBin],oscBank[i].peakFrequency,note_freq[oscBank[i].cqtBin],oscBank[i].phase,oscBank[i].avgValueFast,oscBank[i].transientValue);
        }
        Serial.printf(F("CQT_EOF \n"));
        //Serial.flush();
      }

      //resort so we leave the arrays in order by cqt bin
      //if (low_range_switch) {erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);}
      //else erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);

      return;
    }

  void MessageHandler(AppBaseClass *sender, const char *message){   
    if(sender->isName("SCI")){
      Serial.print(F("M appCQT::MessageHandler SCI param: "));
      Serial.println(message);
      Serial.flush();
      if (strcmp(message,"CQT_INFO")==0){
        //sort the bins to transmit them in order
        erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
        erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
        for (uint16_t i=1;i< NOTE_ARRAY_LENGTH - 1;i++){
          Serial.printf(F("L %d,%f,%f,%d,%d,%d\t\t\t"),fftLowRR[i].cqtBin,fftLowRR[i].startFrequency,fftLowRR[i].stopFrequency,fftLowRR[i].startBin,fftLowRR[i].stopBin,fftLowRR[i].stopBin-fftLowRR[i].startBin);
          Serial.printf(F("H %d,%f,%f,%d,%d,%d\n"),fftHighRR[i].cqtBin,fftHighRR[i].startFrequency,fftHighRR[i].stopFrequency,fftHighRR[i].startBin,fftHighRR[i].stopBin,fftHighRR[i].stopBin-fftHighRR[i].startBin);
        }
        Serial.flush();
      }
    }
  }
};