#include "AudioUtilities.h"
#include "AppManager.h"

// Constant Q Transform App
//

class AppCQT:public AppBaseClass {
  public:
    AppCQT():AppBaseClass(){
      
      sigGen = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName("waveform_2"));
      AudioNoInterrupts();
      sigGen->begin(0.8, 440, WAVEFORM_SINE);
      AudioInterrupts();
      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft->enableFFT(true);

      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange();
      fft2->enableFFT(true);
      fft2AutoOffset = 0;
      iPeakCQTBin = 0;

      memset(&fftRVal,0,sizeof(FFTReadRange));
      memset(&fftPeakHighRR,0,sizeof(FFTReadRange));
      memset(&fftPeakLowRR,0,sizeof(FFTReadRange));

      //init the QCT bins
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        float flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
        float fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
        
        memset(&fftHighRR[i],0,sizeof(FFTReadRange));
        memset(&fftLowRR[i],0,sizeof(FFTReadRange));

        fftHighRR[i].cqtBin =i;
        fftLowRR[i].cqtBin =i;
        
        fftHighRR[i].startFrequency =flow;
        fftLowRR[i].startFrequency =flow;
        
        fftHighRR[i].stopFrequency =fhigh;
        fftLowRR[i].stopFrequency =fhigh;  
      }

    }; 
    //define event handlers
  protected:
    erisAudioSynthWaveform* sigGen; 
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    int16_t fft2AutoOffset;
    int16_t iPeakCQTBin;
    FFTReadRange fftRVal;
    FFTReadRange fftPeakHighRR;
    FFTReadRange fftPeakLowRR;
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH];
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH];
    void update(){
      //draw cqt
      uint16_t highRange = 45;
      int16_t iPeakHigh = 0;
      int16_t iPeakLow = 0;
      float peakHigh = 0;
      float peakLow = 0;
      //memset(&fftRVal,0,sizeof(FFTReadRange));
      //sort by cqt bin 
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);

      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        uint16_t nx;
        float signal;
        float signal_low;
        float n_low;
        float n;

        float im = (width-1)/(float)(sizeof(note_freq)/sizeof(note_freq[0]));
        nx = (uint16_t)(im*i);

        //float flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
        //float fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;

        //n = fft->read(flow,fhigh,&fftRVal);
        n = fft->read(&fftHighRR[i]);
        // fftHighRR[i] = fftRVal;
        if (n > peakHigh) {
          peakHigh=n;
          iPeakHigh=fftHighRR[i].cqtBin;
          //fftPeakHighRR = fftRVal;
          fftPeakHighRR = fftHighRR[i];
        }

        signal = log(n*100)*0.707 * height/5;
        tft.fillRoundRect(origin_x+nx,origin_y,2,(uint16_t)signal,1,ILI9341_DARKCYAN); 

        //n_low = fft2->read(flow,fhigh,&fftRVal);
        n_low  = fft->read(&fftLowRR[i]);
        //fftLowRR[i] = fftRVal;
        if (n_low > peakLow) {
          peakLow=n_low;
          iPeakLow=fftLowRR[i].cqtBin;
          //fftPeakLowRR = fftRVal;
          fftPeakLowRR = fftLowRR[i];
        }
        signal_low = log(n_low*100)*0.707 * height/5;
        
        tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_MAGENTA);
        if (i < highRange) tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal_low,2,(uint16_t)signal_low,1,ILI9341_CYAN);
        /*
        Serial.print(flow);Serial.print(F(","));
        Serial.print(fhigh);Serial.print(F(","));
        Serial.println(signal);
        */
      }

      tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
      AudioNoInterrupts();
      if((peakHigh> peakLow)&&(iPeakHigh >= highRange)) {
        if (iPeakCQTBin != iPeakHigh) sigGen->frequency(fftPeakHighRR.estimatedFrequency);  //sigGen->frequency(note_freq[iPeakHigh]*2);
        iPeakCQTBin = iPeakHigh;
        tft.setTextColor(ILI9341_MAGENTA);
        fftRVal = fftPeakHighRR;
        //Serial.println(fftPeakHighRR.peakFrequency);
      } else{
        if (iPeakCQTBin != iPeakLow) sigGen->frequency(fftPeakLowRR.estimatedFrequency); //sigGen->frequency(note_freq[iPeakLow]*2);
        iPeakCQTBin = iPeakLow;
        tft.setTextColor(ILI9341_CYAN);
        fftRVal = fftPeakLowRR;
        //Serial.println(fftPeakLowRR.peakFrequency);
      }
      AudioInterrupts();
      
      //sort the cqt bins by peakValue
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      //erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);

      
      float error = 100.0 * abs(fftRVal.estimatedFrequency - note_freq[iPeakCQTBin]) / note_freq[iPeakCQTBin];

      tft.fillRect(origin_x + width - 100,origin_y+5,98,110,ILI9341_BLACK);
      tft.setCursor(origin_x + width - 100,origin_y+5);
      tft.print("start: ");
      tft.print(fftRVal.startFrequency);
      
      tft.setCursor(origin_x + width - 100,origin_y+20);
      tft.print("peak: ");
      tft.print(fftRVal.peakFrequency);
      
      tft.setCursor(origin_x + width - 100,origin_y+35);
      tft.print("stop: ");
      tft.println(fftRVal.stopFrequency);
      
      tft.setCursor(origin_x + width - 100,origin_y+50);
      tft.print("est: ");
      tft.print(fftRVal.estimatedFrequency);
      
      tft.setTextColor(ILI9341_ORANGE);
      tft.setCursor(origin_x + width - 100,origin_y+65);
      tft.print("note: ");
      tft.print(note_name[iPeakCQTBin]);
      tft.setCursor(origin_x + width - 100,origin_y+80);
      tft.print("error: ");
      tft.print(error);
      tft.setCursor(origin_x + width - 100,origin_y+95);
      tft.print("fft bins: ");
      tft.print((fftRVal.stopBin - fftRVal.startBin) +1);
      
      tft.setCursor(origin_x + width - 100,origin_y+110);
      tft.print("fftLowRR: ");
      tft.print(fftLowRR[0].startBin);
      
      /*
      Serial.print(fftRVal.peakFrequency);
      Serial.print(",");
      Serial.println(fftRVal.estimatedFrequency);
      */
     
      /*
      Serial.print(fftPeakLowRR.peakFrequency);Serial.print(F(","));
      Serial.print(fftPeakLowRR.peakValue);Serial.print(F(","));
      Serial.print(fftPeakHighRR.peakFrequency);Serial.print(F(","));
      Serial.println(fftPeakHighRR.peakValue);
      */

    };    //called only when the app is active
    void updateRT(){}; //allways called even if app is not active
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
};