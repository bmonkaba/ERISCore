# Eris Core Software Overview

[software documentation](https://bmonkaba.github.io/ERISCore/html/index.html)
<br>
## Design goals

* Stereo audio signal processor assembly built around a Teensy 4.1 combined with a custom TFT ready PCB with integrated power supply, MIDI and independent ADC/DAC
* Real time polyphonic analysis (constant q-transform)
* No 3rd party operating system
* No threads
* AI capability for offline audio training and 'realtime' deployment
* MIDI in/out capable
* Should be able to load and unload applications

## Design methodology

Agile... but like this:
<br>
* Use cases identify needed functions
* Functions from the use cases drive features.
* Implementation of functions are scripted in Wren
* Features to support the functions are implemented in C++ and accessed through C callback extensions from Wren
* C callbacks interact with the C++ framework by requesting the AppWren object from the AppManager singleton
* System/Integration testing is the abuse of the implemented use cases in wren
* Regression/load testing is the continuous cycling of the previous abuse test cases
* Before a function/feature set is checked in it must pass system, integration, regression and load testing with no major issues.
* Note: many test cases are visual or audio in nature therefore pass/fail criteria will be subjective in cases where objective measurements can be made.
* Note: test cases also serve as demonstrations of current functionality

<br>
Calls from the VM are sometimes bounds checked to avoid crashes whereas the C++ interface may be unprotected for performance reasons (no training wheels)

Plan static code analysis clean up, refactoring, formatting after every major feature implementation; as needed

Documentation is constantly updated but always could be better.

Automate as much as possible. Then automate more.

<br>
## Overview for Software Developers
<br>
Fundamentally this is an arduino class library which provides an abstracted framework for rapid application development.

Modifications have been implemented in the teensy audio library to replace reference passing with pointers.

These modifications can be found in the /lib/Audio folder; Specifically the AudioStream sw unit.

Wrappers for the standard teensy audio blocks serve the simple purpose of holding string constants for both the short name and class names, along with the count of inputs and outputs per block

These wrappers are what enable string based messaging for audio block connection and parameter control.

Functionally there are no changes to behavior. The audio blocks update function calls are simply passed to the base class.

<br>
Within this folder are also modified and/or new audio blocks.

For example the eris\_analyze\_fft1024 unit is a custom 32bit floating point, overlapping window implementation with an interface

for querying peak frequency amd power by bin ranges, enabling simple constant q transforms by using precalculated fft bin ranges.

C++ "Applications" are created using the class framework. Specifically an application is a sub class of <span class="colour" style="color:rgb(78, 201, 176)">AppBaseClass</span>

The /include/AppTemplate.h file is provided as a template which contains empty virtual function implementations required by the framework.

What does required by the framework mean? How are apps executed? initialization? what about my own libraries and arduino code... etc??? Lets take a step back and look at a standard arduino application structure

<br>
```
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
```
<br>
<br>
Now with Eris Core (simplified to highlight the structure):
<br>
```
AudioDirector DMAMEM _ad;
AppAudioToPolyphonic EXTMEM appPoly;
SvcSerialCommandInterface FASTRUN sci;
SvcMIDI DMAMEM m;
SvcErisAudioParameterController EXTMEM apc;
AppReprogram EXTMEM appReprogram;
AppWren* FASTRUN appWren;

void setup() {
  Serial.begin(3000000);//baud rate is ignored by the library as it's fixed at max USB speed
  AppManager::setup();
  appWren = new AppWren();//note: The AppBaseClass constructor self registers with the app manager
  appWren->setParent(&appPoly);
  _ad.setSCI(&sci);//give the audio director a pointer to the sci class
  _ad.setAPC(&apc);//give the audio director a pointer to the apc class
  appPoly.getFocus();
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
```
<br>
As can bee clearly seen. The arduino application structure of setup() and loop() functions remains unchanged within the main sw unit.

Therefore, one's own arduino code and libraries can be integrated with Eris core or, execute independently.

Care of course is still required to ensure no resource conflicts.
<br>
### Resources Owned
<br>
It should be considered that Eris Core owns:
<br>
* TFT display buffers and associated HW signals
* Touch and associated HW signals
* MIDI and associated HW signals
* Serial comms
* Audio and I2S HW signals
* Analog control inputs
* SD card
* EXT\_RAM - a significant portion is owned by ErisCore 8MB dedicated RAM drive for the VM, display buffers, audio mem

<br>
Note: C/C++ based applications may take control of digital io and additional EXT\_RAM

Note: HW signal to teensy 4.1 output pin map can be found in HSI.h
<br>
### Initialization
<br>
The Appmanager::setup() call within the setup() function initializes Eris Core.

Applications can be static or dynamically created with new

A few points to note here:

* AppManager is a singleton object instance
* Applications self register with the AppManager on creation by way of the AppBase class constructor

<br>
Many apps could be loaded on setup therefore a method is required to tell Eris Core which one to give focus. This is done with the AppBaseClass method getFocus():

From the setup() example above:

```
appPoly.getFocus();
```
<br>
### Main Loop
<br>
Eris Core is updated by the AppManager from within the arduino main loop like so:
<br>
```
AppManager::getInstance()->update;
```
<br>
This statement requests a pointer to the singleton AppManager then calls update on it.

This is akin to a sys tick(), however there is no context switching as this is a cooperatively scheduled and not a preemptive 'operating system'. The point to be made here is that update does not mean anything other than to update the internal state machine and make appropriate application and core method calls in the process.
<br>
### Application Requirements & Interfaces
<br>
Application interfaces are specified by the implementation of AppBaseClass

If using the AppTemplate, which contains empty implementations of the required virtual functions there is only one requirement:

* Requirement: Every application must have a unique name

<br>
Within the Application constructor set the object name like so:

```
sprintf(name, "MyApplication"); //set the applications name
```
<br>
### Inter-Application Communication
<br>
Message based communication is sent through the string based messageHandler of the receiver object

From ControlButton
<br>
```
void FLASHMEM onTouchRelease(uint16_t t_x, uint16_t t_y) override{
    if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
    	parent_node->messageHandler(this,"Pressed");
    }
    is_pressed = false;
    time_active = 0;
    is_dirty = true;
};
```
<br>
This was a design trade-off in that the obvious first thought was to use enumerated message IDs. However, there is one single reason which drove the decision for a string based messaging system: Universal communication.

What is meant by that is that inter-app, virtual machine, serial & MIDI are all routed through the same interface with no need for managing enum tables across multiple environments & programming languages.

Note: In case the receiver is unknown, the AppManager provides a method for requesting a pointer to an application

From AppWren
<br>
```
AppManager* am = AppManager::getInstance();
AppWren* app = (AppWren*)am->getAppByName("AppWren");
```
<br>
This is how c callback functions are able to interact with Eris core C++ objects.
<br>
### Shared Data and Signaling
<br>
AppManager hosts a hash map shared data dictionary service with a CRUD (create, read, update, destroy) interface as specified in SvcDataDictionary.h

Here is an example reading data from the data service; from ControlButton
<br>
```
if (show_active){
    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_ACTIVE_BORDER_COLOR"));
} else{
    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_INACTIVE_BORDER_COLOR"));
}
```
<br>
Note: The data dictionary service can store multiple data types. check the documentation for specific access methods for each type. (currently float and int32\_t types are supported)
<br>
### Core Services
<br>
Eris Core includes services such as the <span class="colour" style="color: rgb(210, 219, 222);">SvcSerialCommandInterface, SvcMIDI, SvcErisAudioParameterController</span>
<br>
### Standardized Apps
<br>
Eris Core includes standard apps such as ControlButton<span class="colour" style="color: rgb(210, 219, 222);">, Slider</span>
<br>
### Default Apps
<br>
Default Apps are not required but rather flagship examples of application development within the Eris Core framework.

See AppPoly, AppCQT, AppWren
<br>
### Standard Components
<br>
Basic input output components include <span class="colour" style="color: rgb(210, 219, 222);"><span class="colour" style="color: rgb(210, 219, 222);">Touch, AnalogInputs, </span>ILI9341\_t3\_ERIS</span>

<br>
<br>
