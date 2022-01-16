#include <Arduino.h>
#ifndef _GlobalDefines_
#define _GlobalDefines_


/**
 * @brief uncomment if using external ram/n
 * if so,only 16MB (2x8MB) is supported
 */
#define USE_EXTMEM


//AppManager
#define ENABLE_ASYNC_SCREEN_UPDATES

#define DISPLAY_UPDATE_PERIOD  120
/**
 * @brief rate at which the app manager will update the data dictionary
 * 
 */
#define APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC 100 
/**
 * @brief application manager shared image cache size\n
 * note: the reason for the multiplication here is to easily get an idea of\n 
 * how many surfaces and of what size the cache can support.\n
 * the default 64*64*8 can support 8 subsurfaces 64x64 in size
 */
#define AM_IMG_CACHE_SIZE 64*64*8

//AppBaseClass
/**
 * @brief max AppBaseClass::name string length
 * 
 */
#define MAX_NAME_LENGTH 24
/**
 * @brief max AppBaseClass::text string length
 * 
 */
#define MAX_TEXT_LENGTH 24
/**
 * @brief control activation display time\n 
 * for example this defines how long a button will display as pressed
 */
#define SHOW_ACTIVE_TIME_MILLISEC 150

//AppSCI
/**
 * @brief max serial polling rate
 * 
 */
#define SERIAL_POLLING_RATE_MAX 5
/**
 * @brief threshold at which point serial transmission is throttled
 * 
 */
#define SERIAL_THROTTLE_BUFFER_THRESHOLD 1000
/**
 * @brief delta threshold after throttle which will trigger an output flush
 * 
 */
#define SERIAL_THROTTLE_CHECK_CONNECTION_BUFFER_THRESHOLD 500
/**
 * @brief throttle delay used to capture the delta change used in the delta threshold check
 * 
 */
#define SERIAL_THROTTLE_CHECK_CONNECTION_DELAY_MSEC 60
/**
 * @brief max tx header size\n
 *  this is used to support multipart transmissions by capturing the start of message header
 */
#define SERIAL_TX_HEADER_BUFFER_SIZE 64
/**
 * @brief serial rx buffer size 
 * 
 */
#define SERIAL_RX_BUFFER_SIZE 1024
/**
 * @brief serial rx capture buffer size
 * 
 */
#ifdef USE_EXTMEM
#define SERIAL_RX_CAPTURE_BUFFER_SIZE 32000
#else
#define SERIAL_RX_CAPTURE_BUFFER_SIZE 10000
#endif
/**
 * @brief max serial command parameter length
 * 
 */
#define SERIAL_PARAM_BUFFER_SIZE 128
/**
 * @brief output buffer size
 * 
 */
#define SERIAL_OUTPUT_BUFFER_SIZE 6000
/**
 * @brief working buffer size
 * 
 */
#ifdef USE_EXTMEM
#define SERIAL_WORKING_BUFFER_SIZE 32000
#else
#define SERIAL_WORKING_BUFFER_SIZE 10000
#endif

/**
 * @brief filestream payload size\n
 * used for multipart transmission of files
 */
#define SERIAL_FILESTREAM_PAYLOAD_SIZE 1024
/**
 * @brief enables periodic messages to be transmitted
 * 
 */
#define SERIAL_AUTO_TRANSMIT_DATA_PERIODICALLY
/**
 * @brief transmission rate of the data dictionary
 * 
 */
#define SERIAL_AUTO_TRANSMIT_DATA_DICT_PERIOD 391
/**
 * @brief transmission rate of the stats
 * 
 */
#define SERIAL_AUTO_TRANSMIT_STATS_PERIOD 151

//Audio Director
/**
 * @brief max number of audio stream objects
 * 
 */
#define MAX_AUDIO_STREAM_OBJECTS 52
/**
 * @brief max number of audio memory blocks
 * 
 */
#define MAX_AUDIO_MEMORY_BLOCKS 220
/**
 * @brief max number of audio stream catagories
 * 
 */
#define MAX_CATEGORIES 16
/**
 * @brief max number of catagory types
 * 
 */
#define MAX_UNIQUE_NAMES_PER_CATEGORY 24
/**
 * @brief max number of audio connections
 * 
 */
#define MAX_CONNECTIONS 64 
/**
 * @brief max audio connection string length
 * 
 */
#define MAX_CONNECTION_STRING_LENGTH 96

//ILI9341_t3_ERIS
/**
 * @brief defines how many slices per image\n
 * used to acccelerate the wallpaper rendering by only drawing 1/n of the image at a time
 */
#define ANIMATION_CHUNKS_PER_FRAME 4

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
const char FREE_HEAP[] PROGMEM = "FREE_HEAP";
const char FREE_LOCAL[] PROGMEM = "FREE_LOCAL";
const char FREE_STACK[] PROGMEM = "FREE_STACK";
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

//WREN
#ifdef USE_EXTMEM
#define WREN_VM_HEAP_SIZE 32000
#define WREN_FRAME_BUFFER_SIZE 320*240
#else
#define WREN_VM_HEAP_SIZE 16000
#define WREN_FRAME_BUFFER_SIZE 120*120
#endif

const char g_wrenScript[] PROGMEM = R"(
/*  
    Eris Core Wren Script Template / Demonstration
    see https://wren.io/ for more information about the programming language
    note: plenty of wren language scripting examples can be found here:
    https://rosettacode.org/wiki/Category:Wren
    The App class is a close approximation of the C++ AppBase class with some 
    extra goodies builtin. Such as methods for drawing directly to a pre 
    allocated buffer which is automatically rendered to the screen 
    as soon as the wren App update method returns
    
    The example below adapts the julia rendering example from rosettacode 
    into the Eris Core framework
*/
//audio director interface for making/breaking audiostream connections
class AudioDirector{
    //foreign methods are implemented in C/C++
    foreign static connect(source_audio_stream,source_port,
        dest_audio_stream,dest_port)
    foreign static disconnect(audio_stream,port)
    foreign static disconnectAll()
}
//SvcDataDictionary interface for accessing & sharing variables between components
class Data {
    foreign static read(key)
    foreign static update(key,double_value)
    foreign static readf(key)
    foreign static updatef(key,float_value)
}
//Drawing interface for writing to the framebuffer
class Draw {
    foreign static loadImage(path,filename,x,y)
    foreign static setPixel(x,y,r,g,b)
    foreign static getPixel(x,y)
    foreign static line(x,y,to_x,to_y,r,g,b)
    foreign static fill(r,g,b)
    foreign static setTextColor(r,g,b)
    foreign static setCursor(x,y)
    foreign static print(string)
}
//AppBase Class interface for implementing the scripts actions & behaviors
class App {
    construct new() {
        //double underscores indicate static vars
        __x = 64
        __y = 64
        __w = 64
        __h = 64
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
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
        _dx = 1
        _dy = -1
        //AudioDirector.disconnect("biquad_4", 0)
    }
    //it's possible to include foreign methods within a wren class
    //which can interact outside the virtual machine
    
    //these methods bring the AppBase class functionality into the VM
    foreign static sendMessage(to, message)
    foreign static setPosition(x, y)
    foreign static setDimension(width, height)
    foreign static setWidgetPosition(x, y)
    foreign static setWidgetDimension(width, height)
    foreign static requestPopUp(exclusive)
    foreign static releasePopUp()
    foreign static getFocus()
    foreign static returnFocus()
    
    //extra functions - extending beyond the AppBase class
    foreign static setClockSpeed(hz)
    
    //
    //  example specific method - render one pixel of the julia fractal
    //
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
        var r = (2* i*_jx/__w)%(220)
        var g = (2 * i*_jy/__h)%(220)
        var b = (i*2)%220
        if (r < 0) r = 0
        if (g < 0) g = 20
        if (b < 0) b = 0
        Draw.setPixel(_jx, _jy,r,g,b)
    }
    
    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    updateRT() {
        _count = _count + 1
        if (_count > 15000){
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            _count = 0
        }
    }
    update() {
        App.setWidgetPosition(__x, __y)
        //every pixel is calculated by many iterations
        //kick up the clock speed for this section
        App.setClockSpeed(600000000)
        for (y in 0...(__w)) {
            createJulia()
        }
        Draw.setCursor(5 + __x,__y -9)
        Draw.print("WREN VM")
        //and bring the clock speed back down 
        App.setClockSpeed(600000000)
        //let the widget window bounce of the edges of the screen
        __x = __x + _dx
        __y = __y + _dy
        if (__x < 0 - 32) _dx = _dx * -1
        if (__y < 0 - 32) _dy = _dy * -1
        if (__x > 320 - 32) _dx = _dx * -1
        if (__y > 240 - 32) _dy = _dy * -1
    }
    onFocus() {
        var a = "test"
    }
    onFocusLost() {
        var a = "test"
    }
    onTouch() {
        System.print(["Touch_Press",x,y])
    }
    onTouchDrag() {
        System.print(["Touch_Drag",x,y])
    }
    onTouchRelease() {
        System.print(["Touch_Release",x,y])
    }
    onAnalog1(fval) {
        //System.print(["ANALOG_1",fval])
    }
    onAnalog2(fval) {
        //System.print(["ANALOG_2",fval])
    }
    onAnalog3(fval) {
        //System.print(["ANALOG_3",fval])
    }
    onAnalog4(fval) {
        //System.print(["ANALOG_4",fval])
    }
    MessageHandler(sender,message) {
        //System.println([sender,message])
    }
    //example getter
    count { _count }
    //example setter
    count=(value) { _count = value }
}
//static methods can be called directly on the class object
App.setDimension(120, 120)
App.setWidgetDimension(120, 120)
//or a class instance may be created
//The host (C/C++ side) expects a top level variable named ErisApp of 
//type class App it's this object instance for which the event methods 
//will be called
var ErisApp = App.new()
)";


const char g_math_wren[] PROGMEM = R"(
/* Module "math.wren" */
 
/* Math supplements the Num class with various other operations on numbers. */
class Math {
    // Constants.
    static e    { 2.71828182845904523536 } // base of natural logarithms
    static phi  { 1.6180339887498948482  } // golden ratio
    static ln2  { 0.69314718055994530942 } // natural logarithm of 2
    static ln10 { 2.30258509299404568402 } // natural logarithm of 10
 
    // Log function.
    static log10(x) { x.log/ln10 }  // Base 10 logarithm
 
    // Hyperbolic trig functions.
    static sinh(x) { (x.exp - (-x).exp)/2 } // sine
    static cosh(x) { (x.exp + (-x).exp)/2 } // cosine
    static tanh(x) { sinh(x)/cosh(x)      } // tangent
 
    // Inverse hyperbolic trig functions.
    static asinh(x) { (x + (x*x + 1).sqrt).log } // sine
    static acosh(x) { (x + (x*x - 1).sqrt).log } // cosine
    static atanh(x) { ((1+x)/(1-x)).log/2 }      // tangent
 
    // Angle conversions.
    static radians(d) { d * Num.pi / 180}
    static degrees(r) { r * 180 / Num.pi }
 
    // Returns the square root of 'x' squared + 'y' squared.
    static hypot(x, y) { (x*x + y*y).sqrt }
 
    // Returns the integer and fractional parts of 'x'. Both values have the same sign as 'x'.
    static modf(x) { [x.truncate, x.fraction] }
 
    // Returns the IEEE 754 floating-point remainder of 'x/y'.
    static rem(x, y) {
        if (x.isNan || y.isNan || x.isInfinity || y == 0) return nan
        if (!x.isInfinity && y.isInfinity) return x
        var nf = modf(x/y)
        if (nf[1] != 0.5) {
            return x - (x/y).round * y
        } else {
            var n = nf[0]
            if (n%2 == 1) n = (n > 0) ? n + 1 : n - 1
            return x - n * y
        }
    }
 
    // Round away from zero.
    static roundUp(x) { (x >= 0) ? x.ceil : x.floor }
  
    // Convenience version of above method which uses 0 for the 'mode' parameter.
    static toPlaces(x, p) { toPlaces(x, p, 0) }
 
    // Static alternatives to instance methods in Num class.
    // Clearer when both arguments are complex expressions.    
    static min(x, y)  { (x < y) ? x : y }
    static max(x, y)  { (x > y) ? x : y }
    static atan(x, y) { x.atan(y) }
}
 
/* Int contains various routines which are only applicable to integers. */
class Int {
 
    // Returns the remainder when 'b' raised to the power 'e' is divided by 'm'.
    static modPow(b, e, m) {
        if (m == 1) return 0
        var r = 1
        b = b % m
        while (e > 0) {
            if (e%2 == 1) r = (r*b) % m
            e = e >> 1
            b = (b*b) % m
        }
        return r
    }
 
    // Private helper method which checks a number and base for validity.
    static check_(n, b) {
        if (!(n is Num && n.isInteger && n >= 0)) {
            Fiber.abort("Number must be a non-negative integer.")
        }
        if (!(b is Num && b.isInteger && b >= 2 && b < 64)) {
            Fiber.abort("Base must be an integer between 2 and 63.")
        }
    }
 
    // Returns a list of an integer n's digits in base b. Optionally checks n and b are valid.
    static digits(n, b, check) {
        if (check) check_(n, b)
        if (n == 0) return [0]
        var digs = []
        while (n > 0) {
            digs.add(n%b)
            n = (n/b).floor
        }
        return digs[-1..0]
    }
 
    // Returns the sum of an integer n's digits in base b. Optionally checks n and b are valid.
    static digitSum(n, b, check) {
        if (check) check_(n, b)
        var sum = 0
        while (n > 0) {
            sum = sum + (n%b)
            n = (n/b).floor
        }
        return sum
    } 
 
    // Returns the digital root and additive persistence of an integer n in base b.
    // Optionally checks n and b are valid.
    static digitalRoot(n, b, check) {
        if (check) check_(n, b)
        var ap = 0
        while (n > b - 1) {
            n = digitSum(n, b)
            ap = ap + 1
        }
        return [n, ap]
    }
 
    // Convenience versions of the above methods which never check for validity
    // and/or use base 10 by default.    
    static digits(n, b)      { digits(n, b, false) }
    static digits(n)         { digits(n, 10, false) }
    static digitSum(n, b)    { digitSum(n, b, false) }
    static digitSum(n)       { digitSum(n, 10, false) }
    static digitalRoot(n, b) { digitalRoot(n, b, false) }
    static digitalRoot(n)    { digitalRoot(n, 10, false) }
 
    // Returns the unique non-negative integer that is associated with a pair 
    // of non-negative integers 'x' and 'y' according to Cantor's pairing function. 
    static cantorPair(x, y) {
        if (x.type != Num || !x.isInteger || x < 0) {
            Fiber.abort("Arguments must be non-negative integers.")
        }
        if (y.type != Num || !y.isInteger || y < 0) {
            Fiber.abort("Arguments must be non-negative integers.")
        }
        return (x*x + 3*x + 2*x*y + y + y*y) / 2
    }
 
    // Returns the pair of non-negative integers that are associated with a single 
    // non-negative integer 'z' according to Cantor's pairing function. 
    static cantorUnpair(z) {
        if (z.type != Num || !z.isInteger || z < 0) {
            Fiber.abort("Argument must be a non-negative integer.")
        }
        var i = (((1 + 8*z).sqrt-1)/2).floor      
        return [z - i*(1+i)/2, i*(3+i)/2 - z]
    }
}
 
/*
    Nums contains various routines applicable to lists or ranges of numbers
    many of which are useful for statistical purposes.
*/
class Nums {
    // Methods to calculate sum, various means, product and maximum/minimum element of 'a'.
    // The sum and product of an empty list are considered to be 0 and 1 respectively.
    static sum(a)  { a.reduce(0) { |acc, x| acc + x } }
    static mean(a) { sum(a)/a.count }
    static geometricMean(a) { a.reduce { |prod, x| prod * x}.pow(1/a.count) }
    static harmonicMean(a) { a.count / a.reduce { |acc, x| acc + 1/x } }
    static quadraticMean(a) { (a.reduce(0) { |acc, x| acc + x*x }/a.count).sqrt }
    static prod(a) { a.reduce(1) { |acc, x| acc * x } }
    static max(a)  { a.reduce { |acc, x| (x > acc) ? x : acc } }
    static min(a)  { a.reduce { |acc, x| (x < acc) ? x : acc } }
 
    // Returns the median of a sorted list 'a'.
    static median(a) {
        var c = a.count
        if (c == 0) {
            Fiber.abort("An empty list cannot have a median")
        } else if (c%2 == 1) {
            return a[(c/2).floor]
        } else {
            var d = (c/2).floor
            return (a[d] + a[d-1])/2
        }
    }
 
    // Returns a list whose first element is a list of the mode(s) of 'a'
    // and whose second element is the number of times the mode(s) occur.
    static modes(a) {
        var m = {}
        for (e in a) m[e] = (!m[e]) ? 1 : m[e] + 1
        var max = 0
        for (e in a) if (m[e] > max) max = m[e]
        var res = []
        for (k in m.keys) if (m[k] == max) res.add(k)
        return [max, res]
    }
 
    // Returns the sample variance of 'a'.
    static variance(a) {
        var m = mean(a)
        var c = a.count
        return (a.reduce(0) { |acc, x| acc + x*x } - m*m*c) / (c-1)
    }
 
    // Returns the population variance of 'a'.
    static popVariance(a) {
        var m = mean(a)
        return (a.reduce(0) { |acc, x| acc + x*x }) / a.count - m*m
    }
 
    // Returns the sample standard deviation of 'a'.
    static stdDev(a) { variance(a).sqrt }
 
    // Returns the population standard deviation of 'a'.
    static popStdDev(a) { popVariance(a).sqrt }
 
    // Returns the mean deviation of 'a'.
    static meanDev(a) {
        var m = mean(a)
        return a.reduce { |acc, x| acc + (x - m).abs } / a.count
    }
}
 
/* Boolean supplements the Bool class with bitwise operations on boolean values. */
class Boolean {
    // Private helper method to convert a boolean to an integer.
    static btoi_(b) { b ? 1 : 0 }
 
    // Private helper method to convert an integer to a boolean.
    static itob_(i) { i != 0 }
 
    // Private helper method to check its arguments are both booleans.
    static check_(b1, b2) {
        if (!((b1 is Bool) && (b2 is Bool))) Fiber.abort("Both arguments must be booleans.")
    }
 
    // Returns the logical 'and' of its boolean arguments.
    static and(b1, b2) {
        check_(b1, b2)
        return itob_(btoi_(b1) & btoi_(b2))
    }
 
    // Returns the logical 'or' of its boolean arguments.
    static or(b1, b2) {
        check_(b1, b2)
        return itob_(btoi_(b1) | btoi_(b2))
    }
 
    // Returns the logical 'xor' of its boolean arguments.
    static xor(b1, b2) {
        check_(b1, b2)
        return itob_(btoi_(b1) ^ btoi_(b2))
    }
}
)";
#endif
