#include <Arduino.h>
#define BUILTIN_SDCARD 254
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat-beta.h>
#include "HSI.h"
#include "SPI.h"
#include "appExample.h"
#include "appReprogram.h"
#include "PCM1863.h"

uint16_t inc;
MyAppExample *app;
AppReprogram *appReprogram;

void setup() {
  //////////////////////////////////////////////////////////////////////////////////////
  //always run this first to ensure programming mode can be entered through the hmi
  //as access to the physical reset button may be restricted in an integrated application.
  //
  //power on/reset bootrequest check
  pinMode(TAP_INPUT, INPUT);
  pinMode(SW_D, INPUT);
  delay(200);
  if (digitalRead(TAP_INPUT) == LOW && digitalRead(SW_D) == LOW){
            Serial.println("setup: Power on reset request to enter programming mode in 5 seconds.");
            delay(5000);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);
  }
  //////////////////////////////////////////////////////////////////////////////////////

  Serial.begin(256000);//9600
  Serial.println("ERIS CORE: Initalizing");
  touch.setCalibrationInputs(452,374,3830,3800); //inital cal values; app manager will monitor and update
  Serial.println("Loading Applications");
  app = new MyAppExample;    //note: The AppBaseClass constructor self registers with the app manager
  appReprogram = new AppReprogram();
  AppManager::getInstance()->switchAppFocus(app->getId()); //focus is requested by obj id
  //reset the i2c bus and config the external ADC
  Serial.println("Configuring Audio Hardware");
  I2CReset();
  ExtADCConfig();
  //run a quick 12c bus scan
  I2CBusScan();
  Serial.println("Init Complete");
}

void loop(void) {
  //The appManager will... 
  //call the handlers of the active app for any triggered events,
  //calls update for the active app
  //calls updateRT for all apps 
  AppManager::getInstance()->update();
}
