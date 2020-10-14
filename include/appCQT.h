#include "AudioUtilities.h"
#include "AppManager.h"

#define OSC_BANK_SIZE 16

// Approximates atan2(y, x) normalized to the [0,4] range
// with a maximum error of 0.1620 degrees
float normalized_atan2( float y, float x )
{
    //return 0;
    static const uint32_t sign_mask = 0x80000000;
    static const float b = 0.596227f;

    // Extract the sign bits
    uint32_t ux_s  = sign_mask & (uint32_t &)x;
    uint32_t uy_s  = sign_mask & (uint32_t &)y;

    // Determine the quadrant offset
    float q = (float)( ( ~ux_s & uy_s ) >> 29 | ux_s >> 30 ); 

    // Calculate the arctangent in the first quadrant
    float bxy_a = ::fabs( b * x * y );
    float num = bxy_a + y * y;
    float atan_1q =  num / ( x * x + bxy_a + num );

    // Translate it to the proper quadrant
    uint32_t uatan_2q = (ux_s ^ uy_s) | (uint32_t &)atan_1q;
    return q + (float &)uatan_2q;
} 

// Constant Q Transform App
//

class AppCQT:public AppBaseClass {
  public:
    AppCQT():AppBaseClass(){
      char buffer[32];
      sprintf(name, "AppCQT");
      for (int16_t i=0; i < 16; i++){
        sprintf(buffer, "waveform_%d", i);
        osc[i] = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        osc[i]->begin(0.0, 440, WAVEFORM_SINE);
        AudioInterrupts();
      }

      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft->enableFFT(true);

      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange();
      fft2->enableFFT(true);

      memset(&fftRVal,0,sizeof(FFTReadRange));
      memset(&oscBank,0,sizeof(FFTReadRange)* OSC_BANK_SIZE);

      //init the QCT bins
      float flow;
      float fhigh;
      
      for (uint16_t i=1;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        flow = 0;
        fhigh = 0;
        if (i != 0){
          flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
          fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
        }
        memset(&fftHighRR[i],0,sizeof(FFTReadRange));
        memset(&fftLowRR[i],0,sizeof(FFTReadRange));
        
        fftHighRR[i].cqtBin =i;
        fftLowRR[i].cqtBin =i;
        
        fftHighRR[i].startFrequency =flow;
        fftLowRR[i].startFrequency =flow;
        
        fftHighRR[i].stopFrequency =fhigh;
        fftLowRR[i].stopFrequency =fhigh;  
      }

      rt_calls = 0;
      update_calls = 0;
      highRange = 64;
    }; 
  protected:
    double rt_calls;
    double update_calls;
    uint16_t highRange; 
    erisAudioSynthWaveform* osc[OSC_BANK_SIZE];
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    FFTReadRange fftRVal;
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));;
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));;
    FFTReadRange oscBank[OSC_BANK_SIZE] __attribute__ ((aligned (4)));;
    void update(){
      update_calls++;
      if (!fft2->available()) return;
      updateOscillatorBank();
      //draw the cqt bins
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        uint16_t nx;
        float signal;
        float amp;
        float im = (width-1)/(float)(sizeof(note_freq)/sizeof(note_freq[0]));
        amp = fftHighRR[i].avgValueFast;//fft->read(&fftHighRR[i]);
        signal = log(amp*100.0) * height/2;
        nx = (uint16_t)(im*fftHighRR[i].cqtBin);
        tft.fillRoundRect(origin_x+nx,origin_y,2,(uint16_t)signal,1,ILI9341_DARKCYAN); 
        amp = fftLowRR[i].avgValueFast;//fft2->read(&fftLowRR[i]);
        signal = log(amp*100.0) * height/2;
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
      //if (!fft2->available()) return;
      //updateOscillatorBank();
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

    void updateOscillatorBank(){
      float d_min,d;
      uint16_t j_min;

      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        fft->read(&fftHighRR[i]);
        fft2->read(&fftLowRR[i]);
      }
      //sort the updated cqt bins by peakValue
      erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);
      //sort the top 16 by cqt bin
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,8);
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,8);
     
      //update the oscillators:
      //step 1 - clear the peak value
      for(uint16_t i=0; i < OSC_BANK_SIZE; i++) oscBank[i].peakValue = 0;
      
      //for each top cqt bin find the closest osc freq match with a peak value = 0
      for(uint16_t i=0; i < OSC_BANK_SIZE/2; i++){
        d_min = 15000.0;
        j_min = 0;
        for (uint16_t j=0; j < OSC_BANK_SIZE/2; j++){
           d = abs(oscBank[j].peakFrequency - fftHighRR[i].peakFrequency);
           if ((d < d_min) && (oscBank[j].peakValue == 0)){
            d_min=d;
            j_min=j;  
           }
        }
        //assign the oscillator
        oscBank[j_min] = fftHighRR[i]; 
      }
      //repeat for the second cqt
      for(uint16_t i=0; i < OSC_BANK_SIZE/2; i++){
        d_min= 15000.0;
        j_min = 0;
        for (uint16_t j=0; j < OSC_BANK_SIZE/2; j++){
           d = abs(oscBank[j+8].peakFrequency - fftLowRR[i].peakFrequency);
           if ((d < d_min) && (oscBank[j+8].peakValue == 0)){
            d_min=d;
            j_min=j;
           }
        }
        //copy closest to the osc bank
        oscBank[j_min+8] = fftLowRR[i]; 
      }

      //now actually set the oscilators
      //AudioNoInterrupts();
      for(int16_t i=0; i < OSC_BANK_SIZE; i++){
          //osc[i]->frequency(note_freq[oscBank[i].cqtBin]);
          osc[i]->frequency(oscBank[i].estimatedFrequency);
          if (i < 8){osc[i]->amplitude(oscBank[i].avgValueSlow/2);}//0.1 * sin(((fftLowRR[i].peakValue-0.05)/0.095) * PI)
          else osc[i]->amplitude(oscBank[i].peakValue/4);
          Serial.print(oscBank[i].estimatedFrequency);Serial.print(",");
      }
      Serial.println();
      //AudioInterrupts();
      //restore the cleared values
      for(uint16_t i=0; i < OSC_BANK_SIZE/2; i++){
        //fftHighRR[oscBank[i].cqtBin].peakValue = oscBank[i].peakValue;
        //fftLowRR[oscBank[i+8].cqtBin].peakValue = oscBank[i+8].peakValue;
      }
      //resort so we leave it in order by cqt bin
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);
      

      /*
      //set the oscillators
      for (int16_t i=0; i < 8; i++){
        if (fftHighRR[i].cqtBin > highRange){
          float phase = normalized_atan2((float)(0xFFFF & fftHighRR[i].peakFFTResult),(float)(0xFFFF & (fftHighRR[i].peakFFTResult>>16)))*180/PI;
          //AudioNoInterrupts();
          //osc[i]->frequency(note_freq[fftHighRR[i].cqtBin]);
          osc[i]->frequency(fftHighRR[i].estimatedFrequency);
          osc[i]->amplitude(4*fftHighRR[i].avgValueSlow);//0.1 * sin(((fftLowRR[i].peakValue-0.05)/0.095) * PI)
          osc[i]->phase(phase);
          AudioInterrupts();
        } else osc[i]->amplitude(0.0);
      }
      //
      for (int16_t i=0; i < 8; i++){
        if ((fftLowRR[i].cqtBin < highRange) && (fftLowRR[i].cqtBin > 12)){
          float phase = normalized_atan2((float)(0xFFFF & fftLowRR[i].peakFFTResult),(float)(0xFFFF & (fftLowRR[i].peakFFTResult>>16)))*180/PI;
          //AudioNoInterrupts();
          //osc[i]->frequency(note_freq[fftLowRR[i].cqtBin]);
          osc[i+8]->frequency(2* fftLowRR[i].estimatedFrequency);
          osc[i+8]->amplitude(4*fftLowRR[i].avgValueFast);
          osc[i+8]->phase(phase);
          AudioInterrupts();
        } else osc[i+8]->amplitude(0.0);
      }
      */    
      return;
    }
};