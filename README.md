# ERISCore

[![Build Status](https://app.travis-ci.com/bmonkaba/ERISCore.svg?branch=master)](https://app.travis-ci.com/bmonkaba/ERISCore)
<a href="https://scan.coverity.com/projects/bmonkaba-eriscore"></a>
<img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/21765/badge.svg">
<br>
# What is Eris Core:

Eris core is a audio focused development & test framework realized as a hardware/software subsystem component intended for further integration into projects & products.
<img src="https://github.com/bmonkaba/ERISCore/blob/master/pcb/Screenshot%202022-01-04%20005753.png?raw=true" width="300">

# Hardware Features:

* 192khz@32bit stereo I/O\* (hardware capable)
* Low noise balanced thermal power supply design
* TFT touch screen interface
* ADIO connector for interfacing for analog inputs, digital i/o & encoder inputs
* Serial connector for interfacing external I2C & SPI devices
* The ADC's audio front end supports single-ended input levels from small-mV microphone inputs to 2.1-VRMS line inputs
* The ADC's front-end mixer (MIX), multiplexer (MUX), and PGA also support differential (Diff), pseudo-differential, and single-ended (SE) inputs.

<br>
## Software Features: (subject to change)

* Custom single threaded c**ooperative multitasking system / Framework**
* **Coperative scheduler with granular realtime priority control**
* **String based messaging, no enums = easy to add/use new functionality across C++, serial port or the virtual machine**
* Wraps the standard Teensy 4.1 Audio library components
* 32 bit FFT Audio Block
* 2xFFT CQT Polyphonic app example
* Refactored the Teensy 4.1 Audio library to allow full dynamic control of audio connections
* Seamless LZ4 compression/decompression between the device and PC through the web socket interface (wsi)
* Integrated Wren compiler & virtual machine
* Animated wallpaper
* Single frame buffer. Secondary buffers implemented as surfaces
* Image blt to the frame buffer from sd or surface, or from surface to surface
* Web based development tool provides:
    * serial command interface
    * data dictionary monitor
    * audiostream flow monitor/editor
    * ram dump
    * jitter monitor
    * FFT & CQT visualization
    * scripting editor / dedicated VM serial terminal
    * and more...

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
* Low noise thermaly balanced serial power supply design.

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

## Display

* Various
* 320x240 TFT w/ Touch
* Chipset: ILI9341
* Serial Interface

# mvp-01

The reference product design codename "mvp-01" (minimum viable product) is a single chanel audio i/o *device* with two footswitches, four potentiometers, touch screen and MIDI I/O.
<img src="https://github.com/bmonkaba/ERISCore/blob/master/housing/ERIS%20CORE%20angle%20v116.png?raw=true" width="300">