#include <Arduino.h>
#ifndef _GlobalDefines_
#define _GlobalDefines_

//AppManager
#define ENABLE_ASYNC_SCREEN_UPDATES
#define DISPLAY_UPDATE_PERIOD  120
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 1500 

#define AM_IMG_CACHE_SIZE 64*64*9

//AppBaseClass
#define MAX_NAME_LENGTH 36
#define MAX_TEXT_LENGTH 32

#define SHOW_ACTIVE_TIME_MILLISEC 150

//AppSCI
#define SERIAL_POLLING_RATE_MAX 15
#define SERIAL_THROTTLE_BUFFER_THRESHOLD 5700
#define SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD 2000
#define SERIAL_THROTTLE_CHECK_CONNECTION_DELAY_MSEC 20

#define SERIAL_RX_BUFFER_SIZE 1024
#define SERIAL_RX_CAPTURE_BUFFER_SIZE 16384
#define SERIAL_PARAM_BUFFER_SIZE 128
#define SERIAL_OUTPUT_BUFFER_SIZE 16384/3
#define SERIAL_WORKING_BUFFER_SIZE 16384/3
#define SERIAL_FILESTREAM_PAYLOAD_SIZE 1024

#define SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
#define SERIAL_AUTO_TRANSMIT_DATA_DICT_PERIOD 1777
#define SERIAL_AUTO_TRANSMIT_STATS_PERIOD 1551

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


const char g_wrenScript[] PROGMEM = R"(
/*  
    Eris Core Wren Script Template
    see https://wren.io/ for more information about the programming language
    note: plenty of wren language scripting examples can be found here:
    https://rosettacode.org/wiki/Category:Wren

    The App class is a close approximation of the C++ AppBase class with some extra goodies builtin.
    Such as methods for drawing directly to a pre allocated buffer which is automatically rendered to the screen 
    as soon as the App update method returns

    The example below adapts the julia rendering example from rosettacode into the Eris Core framework
*/
class App {
    construct new() {
        __count = 0
        __x = 0
        __y = 0
        __r = 0
        __g = 0
        __b = 0
        __w = 64
        __h = 64
        //static vars for julia set rendering
        __MaxIters = 900
        __Zoom = 1
        __MoveX = 0
        __MoveY = 0
        __CX = -0.7
        __CY = 0.27015
        __jx = 0
        __jy = 0
    }
    foreign static sendMessage(to, message)
    foreign static setPosition(x, y)
    foreign static setDimension(width, height)
    foreign static setWidgetPosition(x, y)
    foreign static setWidgetDimension(width, height)
    foreign static requestPopUp(exclusive)
    foreign static releasePopUp()
    foreign static getFocus()
    foreign static returnFocus()
    foreign static setPixel(x,y,r,g,b)
    
    static createJulia() {
        var zx = 1.5 * (__jx - __w / 2) / (0.5 * __Zoom * __w) + __MoveX
        var zy = (__jy - __h / 2) / (0.5 * __Zoom * __h) + __MoveY
        var i = __MaxIters
        while (zx * zx + zy * zy < 4 && i > 0) {
            var tmp = zx * zx - zy * zy + __CX
            zy = 2 * zx * zy + __CY
            zx = tmp
            i = i - 1
        }
        setPixel(__jx, __jy, i % 256,i % 256,(i*8) % 256)
        if(__jx>(320-__w)){
            __jx = 0
            __jy = __jy + 1
            if (__jy > (240 - __h)){
                __jy = 0
                 __Zoom = __Zoom + 0.5
            }
        } else __jx = __jx + 1
    }
    
    updateRT () {
        //System.print("static App::updateRT() ")
        __count = __count + 1
        if (__count > 50000){
            System.print("App::updateRT() 50K calls")
            __count = 0
        }
    }
    static updateRT() {
        //System.print("static App::updateRT() ")
        __count = __count + 1
        if (__count > 50000){
           // System.print("static App::updateRT() 50K calls")
            System.print([__x,__y])
            __count = 0
        }
    }
    static update() {
        setWidgetPosition(__x, __y)
        createJulia()
       
        //setPixel(__x%64,__y%64,__r,__g,__b)
        __x = __x + 1
        __b = __b + 1
        if (__x > 320-64){
            __y = __y + 1
            __x = 0
            __r = __r + 1
            if (__y > 240-64){
                __x = 0
                __y = 0
                __g = __g + 1
            }
        }
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
       
    }
    static onAnalog2() {
       
    }
    static onAnalog3() {
        
    }
    static onAnalog4() {
        
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
