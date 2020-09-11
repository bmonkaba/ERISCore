#include "AudioUtilities.h"
#include "AppManager.h"
// Constant Q Transform App
//

class AppCQT:public AppBaseClass {
  public:
    AppCQT():AppBaseClass(){
        
      //must downcast fetched objects to the correct type!
      fft = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_1"));
      fft->enableFFT(true);

      fft2 = (erisAudioAnalyzeFFT1024*) (ad.getAudioStreamObjByName("fft1024_2"));
      fft2->toggleActiveRange();
      fft2->enableFFT(true);
      fft2AutoOffset = 0;
    }; 
    //define event handlers
  protected:
    erisAudioAnalyzeFFT1024* fft;
    erisAudioAnalyzeFFT1024* fft2;
    int16_t fft2AutoOffset;
    void update(){
      //draw cqt
      uint16_t highRange = 125;
      int16_t iPeakHigh = 0;
      int16_t iPeakLow = 0;
      float peakHigh = 0;
      float peakLow = 0;
      
      for (uint16_t i=0;i< sizeof(note_freq)/sizeof(note_freq[0])-1;i++){
        uint16_t nx;
        uint16_t ny;
        float signal;
        float signal_low;
        float n_low;
        float n;

        float im = (width-1)/(sizeof(note_freq)/sizeof(note_freq[0]));
        nx = (uint16_t)(im*i);

        float flow = note_freq[i] - (note_freq[i] - note_freq[i-1])/2.0;
        float fhigh = note_freq[i] + (note_freq[i+1] - note_freq[i])/2.0;
        float flow_offset = note_freq[i+fft2AutoOffset] - (note_freq[i+fft2AutoOffset] - note_freq[i+fft2AutoOffset-1])/2.0;
        float fhigh_offset = note_freq[i+fft2AutoOffset] + (note_freq[i+1+fft2AutoOffset] - note_freq[i+fft2AutoOffset])/2.0;

        n = fft->read(flow,fhigh);
        if (n > peakHigh) {peakHigh=n;iPeakHigh=i;}

        signal = log(n*200)*50/8;
        tft.fillRoundRect(origin_x+nx,origin_y,2,(uint16_t)signal,1,ILI9341_DARKCYAN); 

        n_low = fft2->read(flow_offset,fhigh_offset);
        if (n_low > peakLow) {peakLow=n;iPeakLow=i;}
        
        signal_low = log(n_low*200)*50/8;

        /*
        Serial.print(flow);Serial.print(F(","));
        Serial.print(fhigh);Serial.print(F(","));
        Serial.println(signal);
        */
       
        ny = 5;// (uint16_t)origin_y-signal/2;
        if (i < highRange) tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal,2,(uint16_t)signal,1,ILI9341_MAGENTA);
        if (i < highRange) tft.fillRoundRect(origin_x+nx,origin_y+height - (uint16_t)signal_low,2,(uint16_t)signal_low,1,ILI9341_CYAN);
        
        //fft2AutoOffset = iPeakHigh - iPeakLow;
        if (fft2AutoOffset>10) fft2AutoOffset = 10;
        if (fft2AutoOffset<-10) fft2AutoOffset = -10;

        tft.drawRoundRect(origin_x,origin_y,width,height,4,ILI9341_MAGENTA);
      }
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