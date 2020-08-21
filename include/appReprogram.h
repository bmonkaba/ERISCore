#include <core_pins.h>
#include "apps.h"
// Enter Programming Mode
//
// HW pin PROGRAM_MODE must be jumpered to the program pin of the teensy



class AppReprogram:public AppBaseClass {
  public:
    AppReprogram():AppBaseClass(){
      id = 2;
      //config i/o
      /*
      digitalWrite(PROGRAM_MODE, HIGH);
      pinMode(PROGRAM_MODE, OUTPUT);
      */
      pinMode(TAP_INPUT, INPUT);
      pinMode(SW_D, INPUT);
    } 
    //define event handlers
    void updateRT(){
        //Enter program mode if both TAP_INPUT and SW_D 
        //switches are pressed at the same time.
        if (digitalRead(TAP_INPUT) == LOW && digitalRead(TAP_INPUT) == LOW){
            Serial.println("AppReprogram:updateRT Entering Programming Mode");
            delay(100);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);//wait for reset
        };
    }
    
};