/**
 * @file PCM1863.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Arduino.h>
#include "HSI.h"
#include <Wire.h>


#define I2C_STOP 1
#define I2C_NOSTOP 0

//ADC config functions

void I2CReset(){
  //MANUAL RESET OF I2C BUS
  pinMode(I2C_CLK, OUTPUT);
  pinMode(I2C_DATA, OUTPUT);
  digitalWrite(I2C_CLK,1);
  delay(5);
  for (int i = 0; i < 32; i++){
    digitalWrite(I2C_CLK,0);
    delayMicroseconds(100);
    digitalWrite(I2C_CLK,1);
    delayMicroseconds(100);
  }
  digitalWrite(I2C_CLK,0);
  delayMicroseconds(100);
  digitalWrite(I2C_DATA,0);
  delayMicroseconds(100);
  digitalWrite(I2C_CLK,1);
  delayMicroseconds(100);
  digitalWrite(I2C_DATA,1);
  delay(5);
  //END MANUAL I2C BUS RESET
  // Setup I2C for Master mode, pins 18/19, external pullups, 400k, 200ms timeout
  Wire.begin();//I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  //Wire.setDefaultTimeout(200000); // 200ms
}

void ExtADCConfig(){
  //AUDIO FORMAT - i2s
  Wire.beginTransmission(74);Wire.write(11);
  Wire.endTransmission(I2C_NOSTOP);
  Wire.requestFrom(74, 1);
  //Serial.print(Wire.read()); //read current value
  //Serial.print("\n");
  Wire.beginTransmission(74);Wire.write(11);Wire.write(76); //write config
  Wire.endTransmission(I2C_STOP);
  delay(100);
  Wire.beginTransmission(74);Wire.write(11);
  Wire.endTransmission(I2C_NOSTOP);
  Wire.requestFrom(74, 1);
  //Serial.print(Wire.read()); //read new value
  //Serial.print("\n");

  //Set ADC clock mode to internal PLL
  //Wire.beginTransmission(74);Wire.write(32);Wire.write(14);
  //Wire.endTransmission(I2C_STOP);

  //ADC 1 Input Channel Select (ADC1L):
  // 01xx xxxx
  // 0100 0001: VINL1[SE] 41 (default)
  // 0100 0010: VINL2[SE] 42
  // 0100 0100: VINL3[SE] 44
  // 0100 1000: VINL4[SE] 48 <-----
  Wire.beginTransmission(74);Wire.write(6);Wire.write(0x42); //68
  Wire.endTransmission(I2C_STOP);

  //ADC 1 Input Channel Select (ADC1R):
  //0100 0001: VINR1[SE] 41 (default)
  //0100 0010: VINR2[SE] 42
  //0100 0100: VINR3[SE] 44
  //0100 1000: VINR4[SE] 48 <-----
  Wire.beginTransmission(74);Wire.write(7);Wire.write(0x41); //68
  Wire.endTransmission(I2C_STOP);

  //ADC 2 Input Channel Select (ADC2L)
  //0100 0001: VINL1[SE] 41 (default)
  //0100 0010: VINL2[SE] 42  <-----
  //0100 0100: VINL3[SE] 44
  //0100 1000: VINL4[SE] 48
  Wire.beginTransmission(74);Wire.write(8);Wire.write(0x42); //72
  Wire.endTransmission(I2C_STOP);

  //ADC 2 Input Channel Select (ADC2R)
  //0100 0001: VINR1[SE] (default)
  //0100 0010: VINR2[SE] <-----
  //0100 0100: VINR3[SE]
  //0100 1000: VINR4[SE]
  Wire.beginTransmission(74);Wire.write(9);Wire.write(0x41);//72
  Wire.endTransmission(I2C_STOP);

  //PGA
  //Wire.beginTransmission(74);Wire.write(0);Wire.write(0);
  //Wire.endTransmission(I2C_STOP);
  Wire.beginTransmission(74);Wire.write(1);Wire.write(3); //6 * 0.5db = 3db gain
  Wire.endTransmission(I2C_STOP);
  Wire.beginTransmission(74);Wire.write(2);Wire.write(3);
  Wire.endTransmission(I2C_STOP);
  Wire.beginTransmission(74);Wire.write(3);Wire.write(3);
  Wire.endTransmission(I2C_STOP);
  Wire.beginTransmission(74);Wire.write(4);Wire.write(3);
  Wire.endTransmission(I2C_STOP);
}

void I2CBusScan()
{
  byte error, address;
  int nDevices;

  Serial.println("I2CBusScan: Scanning");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2CBusScan: I2C device found at address "));
      if (address<16)
        Serial.print(F("0"));
      //Serial.print(address,HEX);
      Serial.print(address);

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print(F("I2CBusScan: Unknown error at address 0x"));
      if (address<16)
        Serial.print(F("0"));
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println(F("I2CBusScan: No I2C devices found\n"));
}

uint8_t ExtADCReadReg(uint8_t control_register){
    Wire.beginTransmission(74);Wire.write(control_register);
    Wire.endTransmission(I2C_NOSTOP);
    Wire.requestFrom(74, 1);
    return Wire.read();
}