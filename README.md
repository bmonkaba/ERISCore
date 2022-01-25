# ERISCore

[![Build Status](https://app.travis-ci.com/bmonkaba/ERISCore.svg?branch=master)](https://app.travis-ci.com/bmonkaba/ERISCore)
<a href="https://scan.coverity.com/projects/bmonkaba-eriscore"></a>
<img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/21765/badge.svg">
<br>

# What is Eris Core:

Eris core is a audio focused operating system and rapid application development & test framework realized as a hardware/software subsystem component and pc interface software intended for further integration into projects & products.
<img src="https://github.com/bmonkaba/ERISCore/blob/master/pcb/Screenshot%202022-01-04%20005753.png?raw=true" width="300">

# Development State

The complete system is currently pre-alpha. 

The software is currently pre-alpha
* All interfaces are subject to modification
The hardware design is currently beta 2
* Ultra performance variant design requires bulk capacitor selection & integration to support short period extreme overclock boosting while under full load
The mechanical design is currently beta 2
* Minor optimizations of the shell internal thin wall supports to improve SLA 3D printing accuracy
* Multi-format Serial Data (SDIO) access port / connector interface for external connectivity of sensors or HMI expansion
* Ultra performance variant protect for heat sink clearance

# Hardware Features:

* 192khz@32bit stereo I/O\* (hardware capable)
* Low noise linear power supply section with a balanced thermal design
* TFT screen with touch interface
* Analog/Digital (ADIO) connector for interfacing external analog inputs, digital i/o & encoder inputs
* 8 pin Multi-format Serial Data (SDIO) connector for interfacing external I2C & SPI devices
* The audio front end supports single-ended input levels from small-mV microphone inputs to 2.1-VRMS line inputs
* The front-end mixer (MIX), multiplexer (MUX), and programable gain amplifier (PGA) also supports differential (Diff), pseudo-differential, and single-ended (SE) inputs

# Software Features:

* Custom crossover single threaded cooperative multitasking system
* Coperative scheduler with granular realtime priority control
* Integrated Wren compiler & virtual machine (VM) - 32K for the extended PSRAM variant or 16K RAM for the base variant
    * [WIP] Sandboxed filesystem with indirect access methods for utilizing system assets.
* VM extention allows scripts to 'request' a VM reboot to pass control to another script.
* Rapid application development framework
* Dynamic CPU clock scaling automatically maintains maximum performance while remaining within thermal limits
* Universal 'human readable' string based messaging.
* Shared Data Dictionary - int32\_t / float32\_t hash based lookup key value store with key buffer storage (again 'human readable')
* Extends the standard Teensy 4.1 Audio library to allow full dynamic control of audio connections
* Audio block parameter controller exposes most public methods to the universal messaging system without using run time reflectors
* (New) Sliding window 32 bit FFT Audio Block with subsampling control
* 2xFFT CQT Polyphonic application example
* Transparent LZ4 compression/decompression transport layer between the device and the web socket interface (wsi) can improve both latency and and bandwith
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