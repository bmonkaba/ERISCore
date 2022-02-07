#pragma GCC optimize ("Ofast")
#define BUILTIN_SDCARD 254
#include <Arduino.h>
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include "HSI.h"
#include "SPI.h"
#include "AppTemplate.h"
#include "AppManager.h"
//Compiled in ErisCore Apps include files:
#include "AppAudioToPolyphonic.h"
#include "AppWren.h"
#include "AppReprogram.h"
#include "SvcSerialCommandInterface.h"
#include "SvcErisAudioParameterController.h"
#include "AppAIfES.h"

AudioDirector DMAMEM _ad;
AppAudioToPolyphonic EXTMEM appPoly;
SvcSerialCommandInterface FASTRUN sci;
SvcErisAudioParameterController EXTMEM apc;
AppReprogram EXTMEM appReprogram;
AppWren* FASTRUN appWren;
AppAIfES EXTMEM ai;

void setup() {
  //////////////////////////////////////////////////////////////////////////////////////
  //always run this first to ensure programming mode can be entered through the hmi
  //as access to the physical reset button may be restricted in an integrated application.
  //
  //power on/reset bootrequest check
  pinMode(TAP_INPUT, INPUT);
  pinMode(SW_D, INPUT);
 //delay(500);
  Serial.begin(3000000);//baud rate is ignored by the library as it's fixed at max USB speed
  if (digitalRead(TAP_INPUT) == LOW && digitalRead(SW_D) == LOW){
            Serial.println(F("setup: Power on reset request to enter programming mode in 5 seconds."));
            delay(1000);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);
  }
  while(!Serial && millis() < 5000); //DEBUG - wait for serial connection
  //delay(2500);
  AppManager::setup();

  Serial.println(F("M Setup: Loading Applications"));
  appWren = new AppWren();//note: The AppBaseClass constructor self registers with the app manager
  appWren->setParent(&appPoly);
  _ad.setSCI(&sci);//give the audio director a pointer to the sci class
  _ad.setAPC(&apc);//give the audio director a pointer to the apc class
  appPoly.getFocus();
  Serial.println(F("M Setup: Init Complete"));
  Serial.println(F("M Setup: Checking for crash report"));
  if (CrashReport){
      //Serial.print(CrashReport);
  }
  Serial.println(F("M Setup: Done"));
}

void loop(void) {
  //The appManager will... 
  //call the handlers of the active app for any triggered events,
  //calls update for the active app
  //calls updateRT for all apps
  AppManager::getInstance()->update();
}
