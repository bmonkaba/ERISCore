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
    foreign static getPixel(x,y)
    foreign static setPixel(x,y,r,g,b)
    foreign static line(x,y,x2,y2,r,g,b)
    foreign static fill(r,g,b)
    foreign static setTextColor(r,g,b)
    foreign static setCursor(x,y)
    foreign static print(string)
}
//AppBase Class interface for implementing the scripts actions & behaviors
class App {
    construct new() {
        //double underscores indicate static vars
        __x = 72
        __y = 142
        __w = 120
        __h = 120
        __haspop = false
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        
        __dw = 10.23
        __dh = 15.11
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        _demo_loops = 4
        _r = 0
        _g = 0
        _b = 0
        _Gain = 1
        _iteration = 1
        _block_index = 0
        _max_iterations = 5
        _dx = 1.04
        _dy = -0.88
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
    foreign static random(how_big)
    foreign static restartVM(script)
    
    //
    //  example specific method
    //
    createPerlin() {
        var blocks = 4.pow(_max_iterations-_iteration)
        var square_size = __w / blocks
        var from_x = _block_index * square_size
        var to_x = from_x + square_size
        square_size = __h / blocks
        //var from_y = _block_index
        //var to_y = from_y + square_size
        
        for (row in 0...(blocks)){
            //draws one row of blocks
            var r = App.random(220)
            var b = 210-r //App.random(120)
            var g = (r + b)/2 + App.random(110) //App.random(120)
            var from_y = row * square_size
            var to_y = from_y + square_size
            for (y in from_y...(to_y)) {
                for (x in (from_x)...(to_x)) {
                    var pc
                    pc = Draw.getPixel(x,y)
                    //CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
                    var ar = ((_max_iterations-_iteration) * r + (_iteration-1)*(pc >> 8))/_max_iterations
                    var ag = ((_max_iterations-_iteration) * g + (_iteration-1)*((pc >> 3) & 252))/_max_iterations
                    var ab = ((_max_iterations-_iteration) * b + (_iteration-1)*((pc << 3)  & 255))/_max_iterations
                    Draw.setPixel(x,y,ar,ag,ab)
                }
            }
        }
        _block_index = _block_index + 1
        if (_block_index >= blocks ){
            _block_index = 0
            _iteration = _iteration + 1
            if (_iteration >= _max_iterations ){
                _iteration = 1
            }
            
        }
    }
    
    
    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    update() {
        _count = _count + 1
        if (_count > 5000){
            //__w = __w + __dw
            //__h = __h + __dh
            if (__w > 120){
                __dw = __dw * -1
                __w = 120
            }
            if (__h > 120){
              __dh = __dh * -1  
              __h = 120
            } 
            if (__w < 16) __dw = __dw * -1
            if (__h < 16) __dh = __dh * -1
            //App.setWidgetDimension(__w, __h)
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            if (__haspop){
                 //App.setWidgetDimension(1, 1)
            } else{
                //__haspop = true
                //App.requestPopUp(true)
                //Draw.fill(0,0,0)
            }
            _count = 0
            _demo_loops = _demo_loops - 1
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_24")
        }
    }
    render() {
        App.setWidgetPosition(__x, __y)
        createPerlin()
        
        //let the widget window bounce of the edges of the screen
        __x = __x + _dx + (App.random(1) - 0.5)
        __y = __y + _dy + (App.random(1) - 0.5)
        if (__x < 0 ){
            _dx = _dx * -1
            __x = 0
        }
        if (__y < 0 ){
            _dy = _dy * -1
            __y = 0
        }
        if (__x > 320-32){
            _dx = _dx * -1
            __x = 320-32
        }
        if (__y > 240){
            _dy = _dy * -1  
            __y = 240
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
System.print("example_23")



















