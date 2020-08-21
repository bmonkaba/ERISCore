#include <Arduino.h>
//#include <Sd.h>
#define BUILTIN_SDCARD 254
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat-beta.h>
#include "HSI.h"
#include "SPI.h"
#include "appExample.h"
#include "appReprogram.h"
//#include "ILI9341_t3.h"
//#include "ILI9341_t3_ERIS.h"
//#include "touch.h"


//extern ILI9341_t3 tft
//ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);
//ILI9341_t3_ERIS tft = ILI9341_t3_ERIS(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);

//SdFs sd;
//FsFile file;
//Touch touch(CS_TOUCH);

//
uint16_t inc;

MyAppExample *app;
AppReprogram *appReprogram;

void setup() {
  Serial.begin(256000);//9600
  Serial.println("ERIS CORE: Initalizing 7");
  touch.setCalibrationInputs(452,374,3830,3800); //hard coded for now TODO: create a dedicated screen calibration app
  Serial.println("AppManager");
  AppManager::getInstance(); //first call creates the singleton app manager
                            // which is required before any apps are created
  Serial.println("Application");
  app = new MyAppExample;    //note: The AppBaseClass constructor 
                            //self registers the app object with the app manager
  appReprogram = new AppReprogram();
  Serial.println("Setting Focus");
  AppManager::getInstance()->switchAppFocus(app->getId()); //focus is requested by obj id
  
  /*
  if (!sd.begin(SdioConfig(FIFO_SDIO))){
    Serial.println("SD Card Not Found :(");
    sd.initErrorHalt(&Serial);
  } else {Serial.println("SD Card FOUND");}

  tft.setPWMPin(TFT_LED_PWM); //library will control the backlight
  tft.setSD(&sd); //provide a cd pointer to the sd class
  tft.begin();
  tft.println("SD CHECK");
  touch.begin();
  */
  Serial.println("Init Complete");
}

void loop(void) {
  //call the base class handlers of the active app for defined events,
  //calls update for the active app (todo: block on frame sync)
  //calls updateRT for all apps 
  AppManager::getInstance()->update();
}
