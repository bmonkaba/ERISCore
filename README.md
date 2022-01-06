# ERISCore

[![Build Status](https://app.travis-ci.com/bmonkaba/ERISCore.svg?branch=master)](https://app.travis-ci.com/bmonkaba/ERISCore)
<a href="https://scan.coverity.com/projects/bmonkaba-eriscore"></a>
<img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/21765/badge.svg">
<br>
# What is Eris Core:

Eris core is a audio focused hardware/software subsystem component intended for further integration into projects & products

# Features:

* No Operating System - No boot, instant on
* Capable of 32bit @ 192khz audio IO
* Low noise linear ganged power supplys
* TFT touch screen interface
* ADIO connector interface for analog inputs, digital inputs & encoder inputs expansion
* Serial connector interface for I2C & SPI expansion
* The ADC's audio front end supports single-ended input levels from small-mV microphone inputs to 2.1-VRMS line inputs
* The ADC's front-end mixer (MIX), multiplexer (MUX), and PGA also support differential (Diff), pseudo-differential, and single-ended (SE) inputs, making these devices an ideal interface for products that require interference suppression.

<br>
# Hardware Specs:

## Microcontroller:

* NXP IMXRT1060
* ARM Cortex-M7 600 MHz w/overclock to 720 Mhz (within design enclosure)
* Float point math unit, 64 & 32 bits
* 7936K Flash, 1024K RAM (512K tightly coupled), 4K EEPROM (emulated)
* 16MB PSRAM
* 32 general purpose DMA channels
* Cryptographic Acceleration & Random Number Generator
* Programmable FlexIO
* Pixel Processing Pipeline

## Mass Storage:

* 32GB SD Card

## Power:

* 9v DC center negative power jack
* Low noise power supply design. Independent and internaly ganged 5V & 3.3V LDO linear regulators

## Analog to Digital Converter:

* TI PCM1864
* Resolution (Max)(Bits)	32
* Control interface	I2C
* Sampling rate (Max) (kHz)	192
* ADC SNR (Typ) (dB)	103
* Power Dissipation < 145mW

## Digital to Audio Converter:

* TI PCM5100A
* Architecture	Delta Sigma with line driver
* Number of DAC channels (#)	2
* Analog outputs	2
* DAC SNR (Typ) (dB)	106
* Sampling rate (Max) (kHz)	384
* Control interface	H/W
* Resolution (Max)(Bits)	32
* Software (SW) Control
* SW-Controlled Gain: –12 dB to +32 dB
* Power Dissipation < 145mW

## MIDI

* MIDI In/Out Type-A connectors

## Display:

* Various
* 320x240 TFT w/ Touch
* Chipset: ILI9341
* Serial Interface