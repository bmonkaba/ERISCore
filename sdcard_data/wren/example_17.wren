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
    foreign static fill(r,g,b)
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
        App.setDimension(0,0)
        App.requestPopUp(true)
        Draw.fill(0,0,0)
        App.setWidgetPosition(-100,240)
        __x = 0
        __y = 0
        __w = 320
        __h = 240
        

        _start_render = false
        
        
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
        _MaxIters = 120
        _Zoom = 1.725
        _MoveX = 0
        _MoveY = 0
        _CX = -0.7
        _CY = 0.27015
        _jx = 0
        _jy = 0
        _dx = 0.02
        _dy = -0.02
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
                _Zoom = _Zoom - (_Zoom * 0.025)
                //_CY = _CY + 0.0103
                //_CX = _CX + 0.01
                _Gain = _Gain + 0.18
                _MaxIters = _MaxIters + 1
                if (_MaxIters > 350){
                    _MaxIters = 150
                    _Zoom = 1
                }
            }
        } else _jx = _jx + 1
        var r = (_jx/__w)%(220)
        var g = (_jy/__h)%(220)
        var b = (i)%220
        if (r < 0) r = 0
        if (g < 0) g = 0
        if (b < 0) b = 0
        var pc
        pc = Draw.getPixel(_jx+1,_jy+1)
        //CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
        var ar = ( (i * 4 * r) + (i * 8 * (pc >> 8)))/(12)
        var ag = ( (i * 4 * g) + (i * 8 * ((pc >> 3) & 252)))/(12)
        var ab = ( (i * 4 * b) + (i * 8 * ((pc << 3)  & 255)))/(12)
        Draw.setPixel(_jx, _jy,ar,ag,ab)
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
        //Draw.line(x,y,x+offset,y+offset,(x)%220,0%220,(0)%220)
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
        for (i in 1...(6)) {
            r = r * 0.65
            g = g * 0.75
            b = b * 0.85
            Draw.setPixel(x+i,y+i,r,g,b)
        }
    }
    
    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    update() {
        App.setWidgetPosition(-100,240)
        _count = _count + 1
        if (_count > 10000){
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            _count = 0
            _demo_loops = _demo_loops - 1
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_18")
        }
    }
    render() {
        
        //every pixel is calculated by many iterations
        //kick up the clock speed for this section
        App.setClockSpeed(740000000)
        if (_start_render) {
            for (y in 0...(512)) {
                createJulia()
            }
        } else {
            _start_render = true
        }
        
        //for (y in 0...(5)) {
        //    spiral()
        //}
        //App.setClockSpeed(600000000)
    }
    onFocus() {
        var a = "test"
         Draw.fill(100,0,0)
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
System.print("example_17")























