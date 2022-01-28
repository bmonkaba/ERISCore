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
    foreign static line(x,y,x2,y2,r,g,b)
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
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        
        __dw = 10.23
        __dh = 15.11
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        _demo_loops = 5
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
        _dx = 2
        _dy = -2
        //var for spiral method
        _theta = 0
        
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
    foreign static restartVM(script)
    
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
    
    spiral() {
        var x = 0
        var y = 0
        _theta = _theta + 0.2
        if (_theta > 120.0) { 
            _theta = 0.0
        }
        x = (((_theta/Num.pi).cos * _theta/3.0) + __w/2).truncate
        y = (((_theta/Num.pi).sin * _theta/3.0) + __h/2).truncate
        var offset = 5/Data.readf("OSCOPE_SCALE")
        var lt = Data.read("AM_LOOP_TIME")/100
       // Draw.line(x,y,x+offset,y+offset,(x)%220,0%220,(0)%220)
        var r = (lt)%220
        var g = (_count)%220
        var b = (_theta/offset)%220
        Draw.setPixel(x,y,r,g,b)
        //Draw.setPixel(x,y,0,0,255)
        var pc = Draw.getPixel(x,y)
        //CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
        r = pc >> 8
        g = (pc >> 3) & 252
        b = (pc << 3)  & 255
        for (i in 1...(3)) {
            r = r * 0.5
            g = g * 0.5
            b = b * 0.5
            Draw.setPixel(x+i,y+i,r,g,b)
        }
    }
    
    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    updateRT() {
        _count = _count + 1
        if (_count > 1000){
            __w = __w + __dw
            __h = __h + __dh
            if (__w > 120){
                __dw = __dw * -1
                __w = 120
            }
            if (__h > 120){
              __dh = __dh * -1  
              __h = 120
            } 
            if (__w < 64) __dw = __dw * -1
            if (__h < 64) __dh = __dh * -1
            App.setWidgetDimension(__w, __h)
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            _count = 0
            _demo_loops = _demo_loops - 1
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_17")
        }
    }
    update() {
        App.setWidgetPosition(__x, __y)
        //every pixel is calculated by many iterations
        //kick up the clock speed for this section
        App.setClockSpeed(700000000)
        //for (y in 0...(5)) {
            //createJulia()
        //}
        for (y in 0...(3)) {
            spiral()
        }
        //System.print("**")
        
        //Draw.setCursor(5 + __x,__y -9)
        //Draw.print("WREN VM")
        //and bring the clock speed back down 
        App.setClockSpeed(550000000)
        //let the widget window bounce of the edges of the screen
        __x = __x + _dx
        __y = __y + _dy
        if (__x < 0 ) _dx = _dx * -1
        if (__y < 0 ) _dy = _dy * -1
        if (__x > 320 - __w){
            _dx = _dx * -1
            __x = 320 - __w
        }
        if (__y > 240 - __h){
            _dy = _dy * -1  
            __y = 240 - __h
        } 
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

//or a class instance may be created
//The host (C/C++ side) expects a top level variable named ErisApp of 
//type class App it's this object instance for which the event methods 
//will be called
var ErisApp = App.new()
System.print("example_16")












