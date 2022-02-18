# Eris Core

[software documentation](https://bmonkaba.github.io/ERISCore/html/index.html)

## Executive Overview

(Teensy 4.1 core + 2x PSRAM chips) + custom PCB + off the shelf TFT module = Eris Core electronic assembly

The electronis assembly + Eris Core software = Eris Core

# design goals

Stero audio signal processor assembly built around a Teensy 4.1 combined with a custom TFT ready PCB with integrated power supply, MIDI and ADC/DAC chipset
Real time polyphonic analysis (constant q-transform)
No 3rd party operating system
No threads
AI capabilility for offline audio training and 'realtime' deployment
MIDI in/out capable
Should be able to load and unload applications

# design methodology

Agile... but like this:

Use cases identify needed features.
Features from the use cases drive functions.
Implementation of features is scripted in Wren
Functions to support the features are implemented as callback C extentions
The AppWren class is found by the callbacks through the AppManager interface
System/Integration testing is the abuse of the implemented use cases in wren
Regression/load testing is the continuous cycling of the test cases
Calls from the VM are sometimes bounds checked to avoid crashes where as the C++ interface may be unprotected for performance (no training wheels)
Plan static code analysis clean up, refactoring, formatting after every major feature implementation
Documentation is constantly updated but always could be better.
Automate as much as possible. Then automate more.

# Why

The thing I wanted didn't exist. So I made it.
Following the philosophy of 'there is no such thing as an original idea' there must be other's who may find this usefull or at least interesting.

More info to come.

<br>
<br>
<br>
<br>
<br>
<br>
<br>
``` markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```