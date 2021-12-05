/**
 * @file appReprogram.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <core_pins.h>
#include "AppManager.h"
// Enter Programming Mode
//

class AppReprogram:public AppBaseClass {
  public:
    AppReprogram():AppBaseClass(){
      sprintf(name,"AppReprogram");
      pinMode(TAP_INPUT, INPUT);
      pinMode(SW_D, INPUT);
    } 
    //define event handlers
    //void update(){};
    void updateRT (){
        updateRT_priority = 10000;
        //Enter program mode if both TAP_INPUT and SW_D 
        //switches are pressed at the same time.
        if (digitalRead(TAP_INPUT) == LOW && digitalRead(SW_D) == LOW){
            Serial.println("AppReprogram:updateRT Entering Programming Mode");
            delay(100);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);//wait for reset
        }
    }
};