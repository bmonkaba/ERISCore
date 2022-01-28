/*  
    Eris Core Wren Script Template
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
    foreign static loadImageToSurface(path,filename,x,y)
    foreign static blt(from_x, from_y, width, height, dest_x, dest_y)
    foreign static setPixel(x,y,r,g,b)
    foreign static getPixel(x,y)
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
        __x = 64
        __y = 64
        __w = 120
        __h = 120
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        App.setWidgetPosition(__x, __y)
        App.requestPopUp(true)
        Draw.fill(0,90,0)
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        _demo_loops = 2
        _i = 0
        _timer = System.clock
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
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    updateRT() {
        _count = _count + 1
        if (_count > 2900){
            System.print(["UP_TIME",System.clock,"FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            var elapsed_time = System.clock - _timer
            
            System.print(["elapsed time",elapsed_time])
           
            _timer = System.clock
            _count = 0
            _demo_loops = _demo_loops - 1
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("demo")
        }
    }
    
    update() {
        App.setWidgetPosition(__x, __y)
        var start = System.clock
        var x = App.random(120)
        var y = App.random(120)
        var z = App.random(100)
        if(z < 25){
            Draw.loadImageToSurface("/I/U/L/","red.ile",x,y)
        }else if (z < 50){
            Draw.loadImageToSurface("/I/U/L/","yellow.ile",x,y)
        }else if (z < 75){
            Draw.loadImageToSurface("/I/U/L/","grey.ile",x,y)
        }else{
            Draw.loadImageToSurface("/I/U/L/","green.ile",x,y)
        }
        
        x = App.random(320)
        y = App.random(240)
        //Draw.blt(x/3, y/3,120,120,x,y)
        x = App.random(320)
        y = App.random(200)
        Draw.blt(40, 40, 120, 80, x,y)
        _i = _i + 1
    }
    onFocus() {
        //System.print(["onFocus"])
    }
    onFocusLost() {
        //System.print(["onFocusLost"])
    }
    onTouch(x,y) {
        System.print(["Touch_Press",x,y])
    }
    onTouchDrag(x,y) {
        System.print(["Touch_Drag",x,y])
    }
    onTouchRelease(x,y) {
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
System.print("example_29")



















