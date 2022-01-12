//#pragma GCC optimize ("Ofast")
#define BUILTIN_SDCARD 254
#include <Arduino.h>
#include <TimeLib.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include "HSI.h"
#include "SPI.h"
#include "appTemplate.h"
#include "AppAudioToPolyphonic.h"
#include "AppWren.h"
#include "appReprogram.h"
#include "svcSerialCommandInterface.h"
#include "PCM1863.h"

AudioDirector _ad;
AppAudioToPolyphonic appPoly;
SvcSerialCommandInterface sci;
AppReprogram appReprogram;
AppWren* appWren;


void setup() {
  //////////////////////////////////////////////////////////////////////////////////////
  //always run this first to ensure programming mode can be entered through the hmi
  //as access to the physical reset button may be restricted in an integrated application.
  //
  //power on/reset bootrequest check
  pinMode(TAP_INPUT, INPUT);
  pinMode(SW_D, INPUT);
  delay(500);
  Serial.begin(3000000);//baud rate is ignored by the library as it's fixed at max USB speed
  SerialUSB1.begin(115200);
  if (digitalRead(TAP_INPUT) == LOW && digitalRead(SW_D) == LOW){
            Serial.println(F("setup: Power on reset request to enter programming mode in 5 seconds."));
            delay(1000);
            __asm__ volatile ("bkpt #251"); //enter the bootloader
            while(1);
  }
  while(!Serial && millis() < 5000); //DEBUG - wait for serial connection
  //delay(500);
  //////////////////////////////////////////////////////////////////////////////////////
  //reset the i2c bus and config the external ADC
  Serial.println(F("M Setup: Initalizing"));
  Serial.println(F("M Setup: Configuring Audio Hardware"));
  I2CReset();
  ExtADCConfig();
  ExtADCPrintStatus();
  //I2CBusScan();  
  Serial.println(F("M Setup: Loading Applications"));
  //Reset PSRAM clock to 132 Mhz
	  CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_OFF);
	  CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_FLEXSPI2_PODF_MASK | CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK))
		  | CCM_CBCMR_FLEXSPI2_PODF(4) | CCM_CBCMR_FLEXSPI2_CLK_SEL(2); // 528/5 = 132 MHz
	  CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_ON);

  //app = new MyAppExample();    //note: The AppBaseClass constructor self registers with the app manager
  //appReprogram = new AppReprogram();
  //appSCI = new AppSerialCommandInterface();
  //AppManager::getInstance()->getFocus(app->getId()); //focus is requested by obj id
  appWren = new AppWren();
  appWren->setParent(&appPoly);
  //give the audio director a pointer to the sci class
  _ad.setSCI(&sci);
  Serial.println(F("M Setup: Setting App Focus"));
  appPoly.getFocus();
  Serial.println(F("M Setup: Init Complete"));
  Serial.println(F("M Setup: Checking for crash report"));
  if (CrashReport){
      Serial.print(CrashReport);
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
