#include <Arduino.h>
#ifndef _GlobalDefines_
#define _GlobalDefines_

//AppManager
#define ENABLE_ASYNC_SCREEN_UPDATES
#define DISPLAY_UPDATE_PERIOD  120
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 1500 

#define AM_IMG_CACHE_SIZE 64*64*12

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
        //static vars
        __x = 64
        __y = 64
        __w = 64
        __h = 64
        //instance vars
        _count = 0
        _r = 0
        _g = 0
        _b = 0
        _Gain = 1
        _MaxIters = 40
        _Zoom = 1.125
        _MoveX = 0
        _MoveY = 0
        _CX = -0.7
        _CY = 0.27015
        _jx = 0
        _jy = 0
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
    
    createJulia() {
        var zx = 1.5 * (_jx - __w / 2) / (0.5 * _Zoom * __w) + _MoveX
        var zy = (_jy - __h / 2) / (0.5 * _Zoom * __h) + _MoveY
        var i = _MaxIters
        while (zx * zx + zy * zy < 4 && i > 0) {
            var tmp = zx * zx - zy * zy + _CX
            zy = 2 * zx * zy + _CY
            zx = tmp
            i = i - 1
        }
        if(_jx >= __w){
            _jx = 0
            _jy = _jy + 1
            if (_jy >= __h){
                _jy = 0
                _Zoom = _Zoom + (_Zoom * 0.05)
                _CY = _CY + 0.00010001
                _CX = _CX + 0.0001
                _Gain = _Gain + 0.018
                _MaxIters = (80 + (_Zoom * _Zoom * _Zoom)/2.0) 
                if (_MaxIters > 150){
                    _MaxIters = 150
                    _Zoom = 1
                }
            }
        } else _jx = _jx + 1
        //setPixel(jx, jy, (jy/3 +(zy/i))%128,(zx/i)%128,jx/2)
        var r = (2* i*_jx/__w)%(220)
        var g = (2 * i*_jy/__h)%(220)
        var b = (i*2)%220
        if (r < 0) r = 0
        if (g < 0) g = 20
        if (b < 0) b = 0
        
        
        App.setPixel(_jx, _jy,r,g,b)
    }
    
    updateRT () {
        //System.print("App::updateRT() ")
        _count = _count + 1
        if (_count > 15000){
            //System.print("App::updateRT() 50K calls")
            //System.print([__x,__y])
            System.print( _MaxIters )
            _count = 0
        }
    }
    update() {
        App.setWidgetPosition(__x, __y)
        for (y in 0...(__w)) {
            createJulia()
        }
       
        __x = 64
        __y = 64
    }
    onFocus() {
        var a = "test"
    }
    onFocusLost() {
        var a = "test"
    }
    onTouch() {
        var a = "test"
    }
    onTouchDrag() {
        var a = "test"
    }
    onTouchRelease() {
        var a = "test"
    }
    onAnalog1() {
       
    }
    onAnalog2() {
       
    }
    onAnalog3() {
        
    }
    onAnalog4() {
        
    }
    MessageHandler() {
        var a = "test"
    }
    //getter
    count { _count }
    //setter
    count=(value) { _count = value }
}
App.setDimension(64, 64)
App.setWidgetDimension(64, 64)
var ErisApp = App.new()
)";

#endif
