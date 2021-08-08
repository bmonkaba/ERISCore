/**
 * @file AnalogInputs.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _analoginputs_
#define _analoginputs_
#include <Arduino.h>
#include "HSI.h"

#define MAX_POLLING_RATE 100
#define TRIGGER_DELTA 5

/**
 * @brief Used for filtering the analog inputs and change detection
 * 
 */
class AnalogInputs {
    private:
        int16_t ai[4];
        elapsedMillis lastUpdate;
    public:
        AnalogInputs(){
            analogReadAveraging(64);
            analogReadResolution(10);
            ai[0]=0;
            ai[1]=0;
            ai[2]=0;
            ai[3]=0;
        };
        

        /**
         * @brief 
         * 
         * update returns true if a change is detected; false otherwise
         * update is rate limited. additional calls will always return false
         * @return true 
         * @return false 
         */
        bool update(){
            //rate limit the updates
            if (lastUpdate < MAX_POLLING_RATE) return false;
            lastUpdate = 0;
            int16_t old_ai[4] = {ai[0],ai[1],ai[2],ai[3]};
            ai[0] = (ai[0] * 0.80) + (analogRead(AN1) * 0.20);
            ai[1] = (ai[1] * 0.80) + (analogRead(AN2) * 0.20);
            ai[2] = (ai[2] * 0.80) + (analogRead(AN3) * 0.20);
            ai[3] = (ai[3] * 0.80) + (analogRead(AN4) * 0.20);
            if (abs(ai[0] - old_ai[0])>TRIGGER_DELTA) return true;
            if (abs(ai[1] - old_ai[1])>TRIGGER_DELTA) return true;
            if (abs(ai[2] - old_ai[2])>TRIGGER_DELTA) return true;
            if (abs(ai[3] - old_ai[3])>TRIGGER_DELTA) return true;
            return false;
        }
        //return ad counts
        uint16_t readAN1(){return (uint16_t )ai[0];};
        uint16_t readAN2(){return (uint16_t )ai[1];};
        uint16_t readAN3(){return (uint16_t )ai[2];};
        uint16_t readAN4(){return (uint16_t )ai[3];};

        //return 10bit scaled 0-1 range
        float freadAN1(){return ai[0]/1024.0;};
        float freadAN2(){return ai[1]/1024.0;};
        float freadAN3(){return ai[2]/1024.0;};
        float freadAN4(){return ai[3]/1024.0;};
};

#endif