#include <Arduino.h>
#ifndef _GlobalDefines_
#define _GlobalDefines_

//AppManager
#define ENABLE_ASYNC_SCREEN_UPDATES
#define DISPLAY_UPDATE_PERIOD  120
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 1500 

#define AM_IMG_CACHE_SIZE 64*64*8

//AppBaseClass
#define MAX_NAME_LENGTH 36
#define MAX_TEXT_LENGTH 32

//AppSCI
#define SERIAL_POLLING_RATE_MAX 15
#define SERIAL_THROTTLE_BUFFER_THRESHOLD 4700
#define SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD 1000
#define SERIAL_THROTTLE_CHECK_CONNECTION_DELAY_MSEC 20

#define SERIAL_RX_BUFFER_SIZE 1024
#define SERIAL_RX_CAPTURE_BUFFER_SIZE 16384/2
#define SERIAL_PARAM_BUFFER_SIZE 128
#define SERIAL_OUTPUT_BUFFER_SIZE 16384/2
#define SERIAL_WORKING_BUFFER_SIZE 16384/2
#define SERIAL_FILESTREAM_PAYLOAD_SIZE 1024

#define SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
#define SERIAL_AUTO_TRANSMIT_DATA_DICT_PERIOD 2821
#define SERIAL_AUTO_TRANSMIT_STATS_PERIOD 1821

//Audio Director
#define MAX_AUDIO_STREAM_OBJECTS 52
#define MAX_AUDIO_MEMORY_BLOCKS 120
#define MAX_CATEGORIES 16
#define MAX_UNIQUE_NAMES_PER_CATEGORY 24
#define MAX_CONNECTIONS 64 
#define MAX_CONNECTION_STRING_LENGTH 96

//ILI9341_t3_ERIS
#define ANIMATION_CHUNKS_PER_FRAME 6

//STRINGS - APPS
const char OCTAVE_DOWN_INTERVAL[] PROGMEM = "OCTAVE_DOWN_INTERVAL";
const char CQT_UPDATE_COUNT[] PROGMEM = "CQT_UPDATE_COUNT";
const char CQT_INFO[] PROGMEM = "CQT_INFO";
const char FFT_AGE_THRESHOLD[] PROGMEM = "FFT_AGE_THRESHOLD";
const char ENABLE[] PROGMEM = "ENABLE";
const char DISABLE[] PROGMEM = "DISABLE";

const char PROGMEM s[][16] = {"SIN","TRI","SAW","REVSAW","SQUARE","TEST","COMPRESSOR","BACK"};
const char PROGMEM p[][16] = {"DOUBLE.ile","GRAIN.ile","PHASER.ile","BITCRUSH.ile","FUZZ.ile","REVERB.ile","COMPRESSOR.ile","BACK.ile"};
const char INPUT_PEAK[] PROGMEM = "INPUT_PEAK";

//STRINGS - ERIS CORE
const char FREE_MEM[] PROGMEM = "FREE_MEM";
const char HEAP_FREE[] PROGMEM = "HEAP_FREE";
const char LOCAL_MEM[] PROGMEM = "LOCAL_MEM";
const char CPU_TEMP[] PROGMEM = "CPU_TEMP";
const char AM_LOOP_TIME[] PROGMEM = "AM_LOOP_TIME";
const char AM_SERIAL_AVAIL[] PROGMEM = "AM_SERIAL_AVAIL";
const char AM_AN1[] PROGMEM = "AM_AN1";
const char AM_AN2[] PROGMEM = "AM_AN2";
const char AM_AN3[] PROGMEM = "AM_AN3";
const char AM_AN4[] PROGMEM = "AM_AN4"; 
const char RT_CALLS[] PROGMEM = "AM_RT_CALLS";
const char UPDATE_CALLS[] PROGMEM = "AM_UPDATE_CALLS";
const char RENDER[] PROGMEM = "RENDER";
const char RENDER_PERIOD[] PROGMEM = "AM_RENDER_PERIOD";
const char RENDER_FRAME[] PROGMEM = "AM_RENDER_FRAME";
const char AM_REDRAW_BG[] PROGMEM = "AM_REDRAW_BG";
const char AM_AUDIO_CPU_MAX[] PROGMEM = "AM_AUDIO_CPU_MAX";
const char AM_AUDIO_CPU[] PROGMEM = "AM_AUDIO_CPU";
const char AM_AUDIO_MEM_MAX[] PROGMEM = "AM_AUDIO_MEM_MAX";
const char AM_AUDIO_MEM[] PROGMEM = "AM_AUDIO_MEM";
const char UI_ANIMATED_WALLPAPER_PATH[] PROGMEM = "/V/POINTSOLIGHT";
const char UI_BUTTON_FILL_COLOR[] PROGMEM = "UI_BUTTON_FILL_COLOR";
const char UI_BUTTON_SHADE_COLOR[] PROGMEM = "UI_BUTTON_SHADE_COLOR";
const char UI_BUTTON_ACTIVE_BORDER_COLOR[] PROGMEM = "UI_BUTTON_ACTIVE_BORDER_COLOR";
const char UI_BUTTON_INACTIVE_BORDER_COLOR[] PROGMEM = "UI_BUTTON_INACTIVE_BORDER_COLOR";
const char UI_BUTTON_TEXT_COLOR[] PROGMEM = "UI_BUTTON_TEXT_COLOR";
const char UI_SLIDER_BORDER_COLOR[] PROGMEM = "UI_SLIDER_BORDER_COLOR";
const char UI_SLIDER_SHADE_COLOR[] PROGMEM = "UI_SLIDER_SHADE_COLOR";
const char UI_SLIDER_FILL_COLOR[] PROGMEM = "UI_SLIDER_FILL_COLOR";
const char UI_SLIDER_TEXT_COLOR[] PROGMEM = "UI_SLIDER_TEXT_COLOR";

//Wren test script
//test script
//note: plenty of wren language scripting examples can be found here:
//https://rosettacode.org/wiki/Category:Wren
const char g_wrenScript[] PROGMEM = R"(
class App {
    construct new() {
        __count = 0
    }

    updateRT () {
        //System.print("M  static App::updateRT() ")
        __count = __count + 1
        if (__count > 50000){
            System.print("M App::updateRT() 50K calls")
            __count = 0
        }
    }

    static updateRT() {
        //System.print("M  static App::updateRT() ")
        __count = __count + 1
        if (__count > 50000){
            System.print("M static App::updateRT() 50K calls")
            __count = 0
        }
    }

    static update() {
        var a = "test"
    }

    static onFocus() {
        var a = "test"
    }

    static onFocusLost() {
        var a = "test"
    }

    static onTouch() {
        var a = "test"
    }

    static onTouchDrag() {
        var a = "test"
    }

    static onTouchRelease() {
        var a = "test"
    }

    static onAnalog1() {
        System.print("M App::onAnalog1() ")
    }

    static onAnalog2() {
        System.print("M App::onAnalog2() ")
    }

    static onAnalog3() {
        System.print("M App::onAnalog3() ")
    }

    static onAnalog4() {
        System.print("M App::onAnalog4() ")
    }

    static MessageHandler() {
        var a = "test"
    }

    //getter
    count { __count }
    //setter
    count=(value) { __count = value }
}

var ErisApp = App.new()
//ErisApp.updateRT()
//static function call
//App.updateRT()

)";

#endif
