//#pragma GCC optimize ("Ofast")
#define BUILTIN_SDCARD 254
#include <Arduino.h>
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
  //app = new MyAppExample();    //note: The AppBaseClass constructor self registers with the app manager
  //appReprogram = new AppReprogram();
  //appSCI = new AppSerialCommandInterface();
  //AppManager::getInstance()->getFocus(app->getId()); //focus is requested by obj id
  appWren = new AppWren();
  //give the audio director a pointer to the sci class
  _ad.setSCI(&sci);
  Serial.println(F("M Setup: Setting App Focus"));
  appPoly.getFocus();
  Serial.println(F("M Setup: Init Complete"));

 
/*
send data TO wren
void wrenSetSlotBool(WrenVM* vm, int slot, bool value);
void wrenSetSlotDouble(WrenVM* vm, int slot, double value);
void wrenSetSlotNull(WrenVM* vm, int slot);
void wrenSetSlotBytes(WrenVM* vm, int slot, const char* bytes, size_t length);
void wrenSetSlotString(WrenVM* vm, int slot, const char* text);

get data FROM wren
bool wrenGetSlotBool(WrenVM* vm, int slot);
double wrenGetSlotDouble(WrenVM* vm, int slot);

get top level variables from wren
void wrenGetVariable(WrenVM* vm, const char* module,
                     const char* name, int slot);


// Load the class into slot 0.
wrenEnsureSlots(vm, 1);
wrenGetVariable(vm, "main", "GameEngine", 0);
WrenHandle* gameEngineClass = wrenGetSlotHandle(vm, 0);

Now, each time we want to call a method on GameEngine, we store that value back in slot zero:

wrenSetSlotHandle(vm, 0, gameEngineClass);

pass variables in slots 1..n
wrenSetSlotDouble(vm, 1, elapsedTime);

//call the method
WrenInterpretResult wrenCall(WrenVM* vm, WrenHandle* method);
*/

  

  

  //delay(5000);
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
