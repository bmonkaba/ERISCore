# ERISCore

[![Build Status](https://app.travis-ci.com/bmonkaba/ERISCore.svg?branch=master)](https://app.travis-ci.com/bmonkaba/ERISCore)
<a href="https://scan.coverity.com/projects/bmonkaba-eriscore"></a>
<img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/21765/badge.svg">
<br>
# What is Eris Core:

Eris core is a audio focused operating system and rapid application development & test framework realized as a hardware/software subsystem component and pc interface software intended for further integration into projects & products.
<img src="https://github.com/bmonkaba/ERISCore/blob/master/pcb/Screenshot%202022-01-04%20005753.png?raw=true" width="300">

# Hardware Features:

* 192khz@32bit stereo I/O\* (hardware capable)
* Low noise linear power supply section with a balanced thermal design
* TFT screen with touch interface
* Analog/Digital (ADIO) connector for interfacing external analog inputs, digital i/o & encoder inputs
* 8 pin Multi-format Serial Data (SDIO) connector for interfacing external I2C & SPI devices.
* The audio front end supports single-ended input levels from small-mV microphone inputs to 2.1-VRMS line inputs
* The front-end mixer (MIX), multiplexer (MUX), and programable gain amplifier (PGA) also supports differential (Diff), pseudo-differential, and single-ended (SE) inputs.

<br>
## Software Features: (subject to change)

* Custom single threaded cooperative multitasking system / Framework
* Coperative scheduler with granular realtime priority control
* Universal 'human readable' string based messaging. No enums to maintain across development environments = easy to add/use new functionality across software components; whether native C/C++, a pc communicating across the serial port or with a script running in the virtual machine.
* Shared Data Dictionary - int32\_t / float32\_t hash based key value store with key buffer storage (again 'human readable'). Interfaces support either anonymous global r/w access or owner based r/w control for each key store.
* Wraps the standard Teensy 4.1 Audio library components.
* (New) Sliding window 32 bit FFT Audio Block with subsampling control
* 2xFFT CQT Polyphonic application example
* Refactored the Teensy 4.1 Audio library to allow full dynamic control of audio connections
* Serial communication LZ4 compression/decompression transport layer between the device and the web socket interface (wsi) can improve both latency and and bandwith.
* Integrated Wren compiler & virtual machine (VM)
* [TODO] VM file system is sandboxed. the search path is the script location, then the standard system asset paths. system assets are always read only. No paths required, or accepted, for file access.
* VM extention allows scripts to 'request' a reboot of the VM at a safe time and pass control to another script. This provides the possibility of limitless program size given each sw unit (or whatever the sw paritioning concept deployed could be) stays within the memory allocated to the VM. Default: 32K for the extended ram variant or 16K for the no extended ram variant
* Configurable animated or static wallpaper automatically rendered at the start of each frame by the system. 
* Single frame buffer. Secondary buffers implemented as surfaces.
* Multiple source/destination image block transfer (blt) interfaces:
    * SD card to framebuffer
    * SD card to surface
    * Surface to surface
    * Surface to framebuffer
* Web based development tool provides:
    * Serial command interface
    * Data dictionary monitor
    * Audiostream flow monitor/editor
    * Ram dump
    * Application jitter monitor
    * FFT & CQT visualization
    * Scripting editor & dedicated VM serial terminal
    * And more...

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