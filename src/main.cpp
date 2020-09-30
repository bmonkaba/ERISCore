#include <Arduino.h>
#define BUILTIN_SDCARD 254
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat-beta.h>
#include "HSI.h"
#include "SPI.h"
#include "appTemplate.h"
#include "appExample.h"
#include "appReprogram.h"
#include "PCM1863.h"

uint16_t inc;
MyAppExample app;
AppReprogram *appReprogram;
AppTemplate appTemplate;

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
  //reset the i2c bus and config the external ADC
  Serial.begin(256000);//9600
  Serial.println(F("Setup: Initalizing"));
  Serial.println(F("Setup: Configuring Audio Hardware"));
  I2CReset();
  ExtADCConfig();
  //I2CBusScan();
  touch.setCalibrationInputs(452,374,3830,3800); //inital cal values; app manager will monitor and update
  touch.setRotation(3);
  Serial.println(F("Setup: Loading Applications"));
  //app = new MyAppExample;    //note: The AppBaseClass constructor self registers with the app manager
  appReprogram = new AppReprogram();
  AppManager::getInstance()->switchAppFocus(app.getId()); //focus is requested by obj id
  // set the connection goup
  ad.activateConnectionGroup(0);
  Serial.println(F("Setup: Configuring the sw audio block connections"));
  Serial.println(F("Setup: Init Complete"));
  Serial.print(F("Ext ADC Operating State (15:RUNNING): "));
  Serial.println(ExtADCReadReg(114));
  Serial.print(F("Ext ADC Sample Freq: "));
  Serial.println(ExtADCReadReg(115));
  Serial.print(F("Ext ADC BCLK Ratio: "));
  Serial.println((ExtADCReadReg(116) >> 4) & 0x7);
  Serial.print(F("Ext ADC SCLK Ratio: "));
  Serial.println(ExtADCReadReg(116) & 0x7);
  Serial.print(F("Ext ADC CLK Error Status (0:OK): "));
  Serial.println(ExtADCReadReg(117));
  Serial.print(F("Ext ADC Voltage Status (7:OK): "));
  Serial.println(ExtADCReadReg(120));
  for (int i =0; i < 12;i++){
    Serial.print(F("Ext ADC CONF REG VALUE"));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.println(ExtADCReadReg(i),HEX);
  }
}

elapsedMillis loop_cycle_time;
void loop(void) {
  //The appManager will... 
  //call the handlers of the active app for any triggered events,
  //calls update for the active app
  //calls updateRT for all apps
  loop_cycle_time = 0; 
  AppManager::getInstance()->update();
  //Serial.println(loop_cycle_time);                                     
}
