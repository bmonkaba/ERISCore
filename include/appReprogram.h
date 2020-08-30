#include <core_pins.h>
#include "AppManager.h"
// Enter Programming Mode
//

class AppReprogram:public AppBaseClass {
  public:
    AppReprogram():AppBaseClass(){
      id = 2;
      pinMode(TAP_INPUT, INPUT);
      pinMode(SW_D, INPUT);
    } 
    //define event handlers
    void updateRT(){
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