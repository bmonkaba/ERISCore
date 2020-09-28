#include "AudioUtilities.h"
#include "AppManager.h"
// Approximates atan2(y, x) normalized to the [0,4) range
// with a maximum error of 0.1620 degrees

float normalized_atan2( float y, float x )
{
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
      for (int16_t i=0; i < 16; i++){
        sprintf(buffer, "waveform_%d", i);
        osc[i] = (erisAudioSynthWaveform*) (ad.getAudioStreamObjByName(buffer));
        AudioNoInterrupts();
        osc[i]->begin(0.0, 440, WAVEFORM_SINE);
        AudioInterrupts();
      }

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


    }; 
    //define event handlers
  protected:
    erisAudioSynthWaveform* osc[16];
    erisAudioSynthWaveform* sigGen; 
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    int16_t fft2AutoOffset;
    int16_t iPeakCQTBin;
    FFTReadRange fftRVal;
    FFTReadRange fftPeakHighRR;
    FFTReadRange fftPeakLowRR;
    FFTReadRange fftHighRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));;
    FFTReadRange fftLowRR[NOTE_ARRAY_LENGTH] __attribute__ ((aligned (4)));;
    void update(){
      //draw cqt
      uint16_t highRange = 45;
      int16_t iPeakHigh = 0;
      int16_t iPeakLow = 0;
      float peakHigh = 0;
      float peakLow = 0;
      


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
      
      if((peakHigh> peakLow)&&(iPeakHigh >= highRange)) {
        if (iPeakCQTBin != iPeakHigh){
          //AudioNoInterrupts();
          sigGen->frequency(fftPeakHighRR.estimatedFrequency);  //sigGen->frequency(note_freq[iPeakHigh]*2);
          sigGen->amplitude(fftPeakHighRR.peakValue);
          AudioInterrupts();
        }
        iPeakCQTBin = iPeakHigh;
        tft.setTextColor(ILI9341_MAGENTA);
        fftRVal = fftPeakHighRR;
        //Serial.println(fftPeakHighRR.peakFrequency);
      } else{
        if (iPeakCQTBin != iPeakLow){
          //AudioNoInterrupts();
          //sigGen->frequency(fftPeakLowRR.estimatedFrequency); //sigGen->frequency(note_freq[iPeakLow]*2);
          //sigGen->amplitude(fftPeakLowRR.peakValue);
          AudioInterrupts();
        }
        iPeakCQTBin = iPeakLow;
        tft.setTextColor(ILI9341_CYAN);
        fftRVal = fftPeakLowRR;
        //Serial.println(fftPeakLowRR.peakFrequency);
      }
      
      
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
      
      //sort the cqt bins by peakValue
      erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_value(fftHighRR,NOTE_ARRAY_LENGTH);

      //set the oscillators
      
      //AudioNoInterrupts();
      for (int16_t i=0; i < 8; i++){
        if (fftHighRR[i].cqtBin > highRange){ 
          osc[i]->frequency(note_freq[fftHighRR[i].cqtBin]);
          osc[i]->amplitude(fftHighRR[i].avgValueFast);//0.1 * sin(((fftLowRR[i].peakValue-0.05)/0.095) * PI)
          //osc[i]->phase(normalized_atan2((float)(0xFFFF & fftHighRR[i].peakFFTResult),(float)(0xFFFF & (fftHighRR[i].peakFFTResult>>16)))*180/PI);
        } else osc[i]->amplitude(0.0);
      }
      
      for (int16_t i=8; i < 16; i++){
        if ((fftLowRR[i].cqtBin < highRange) && (fftLowRR[i].cqtBin > 12)){
          osc[i]->frequency(note_freq[fftLowRR[i].cqtBin]);
          osc[i]->amplitude(fftLowRR[i].avgValueSlow);
          //osc[i]->phase(normalized_atan2((float)(0xFFFF & fftLowRR[i].peakFFTResult),(float)(0xFFFF & (fftLowRR[i].peakFFTResult>>16)))*180/PI);
        }
      }
      
      AudioInterrupts();

    };    //called only when the app is active
    void updateRT(){
      //sort by cqt bin 
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftLowRR,NOTE_ARRAY_LENGTH);
      erisAudioAnalyzeFFT1024::sort_fftrr_by_cqt_bin(fftHighRR,NOTE_ARRAY_LENGTH);

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
};