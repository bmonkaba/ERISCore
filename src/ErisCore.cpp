//char dummy[32000];
//#pragma GCC optimize ("Ofast")
#define BUILTIN_SDCARD 254
#include <Arduino.h>

#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include "HSI.h"
#include "SPI.h"
#include "appTemplate.h"
#include "appExample.h"
#include "appReprogram.h"
#include "appSerialCommandInterface.h"
#include "PCM1863.h"

uint16_t inc;
MyAppExample app;
AppSerialCommandInterface appSCI;
DMAMEM AppReprogram appReprogram;
//AppTemplate appTemplate;

void PROGMEM setup() {
  //stack fill test
  //char stackfill[50000];
  //memset(stackfill,0x5A,sizeof(stackfill));
  //////////////////////////////////////////////////////////////////////////////////////
  //always run this first to ensure programming mode can be entered through the hmi
  //as access to the physical reset button may be restricted in an integrated application.
  //
  //power on/reset bootrequest check
  pinMode(TAP_INPUT, INPUT);
  pinMode(SW_D, INPUT);
  delay(500);
  Serial.begin(3000000);//baud rate is ignored by the library as it's fixed at max USB speed
  if (digitalRead(TAP_INPUT) == LOW && digitalRead(SW_D) == LOW){
            Serial.println(F("setup: Power on reset request to enter programming mode in 5 seconds."));
            delay(1000);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);
  }
  while(!Serial); //DEBUG - wait for serial connection
  delayNanoseconds(500);
  //////////////////////////////////////////////////////////////////////////////////////
  //reset the i2c bus and config the external ADC
  Serial.println(F("Setup: Initalizing"));
  Serial.println(F("Setup: Configuring Audio Hardware"));
  I2CReset();
  ExtADCConfig();
  ExtADCPrintStatus();
  //I2CBusScan();
  
  Serial.println(F("Setup: Loading Applications"));
  //app = new MyAppExample();    //note: The AppBaseClass constructor self registers with the app manager
  //appReprogram = new AppReprogram();
  //appSCI = new AppSerialCommandInterface();
  //AppManager::getInstance()->getFocus(app->getId()); //focus is requested by obj id
  Serial.println(F("Setup: Setting App Focus"));
  app.getFocus();
  Serial.println(F("Setup: Init Complete"));
  //delay(5000);

}

void loop(void) {
  //The appManager will... 
  //call the handlers of the active app for any triggered events,
  //calls update for the active app
  //calls updateRT for all apps
  AppManager::getInstance()->update();                           
}
