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
//Drawing interface
class Draw {
    foreign static useWrenFileSystem()
    foreign static useSDFileSystem()
    foreign static getImageSize(path,filename)
    foreign static loadImage(path,filename,x,y,blt_op)
    foreign static loadImageToSurface(path,filename,x,y,blt_op)
    foreign static blt(from_x, from_y, width, height, dest_x, dest_y,blt_op)
    foreign static setPixel(x,y,r,g,b)
    foreign static getPixel(x,y)
    foreign static line(x,y,x2,y2,r,g,b)
    foreign static fill(r,g,b)
    foreign static setTextColor(r,g,b)
    foreign static setCursor(x,y)
    foreign static setFontSize(pt)
    foreign static print(string)
    foreign static print(string,x,y,font,font_point)
}
//RAM drive file system
class FileSystem{
    foreign static mkdir(dir_name)
    foreign static rmdir(dir_name)
    foreign static open(file_name,mode)
    foreign static exists(file_name)
    foreign static rename(old_name,new_name)
    foreign static remove(file_name)
    foreign static usedSize()
    foreign static totalSize()
    foreign static importFromSD(src_path,src_file_name,dst_path,dst_file_name)
}
//RAM drive file interface
class File{
    foreign static read(nbytes)
    foreign static readBytes(nbytes)
    foreign static write(data)
    foreign static available()
    foreign static peek()
    foreign static flush()
    foreign static truncate(size)
    foreign static seek(pos,mode)
    foreign static position()
    foreign static size()
    foreign static close()
    foreign static isOpen()
    foreign static name()
    foreign static isDirectory()
    foreign static openNextFile(mode)
    foreign static rewindDirectory()
}
//AppBase Class interface for implementing the scripts actions & behaviors
class App {
    construct new() {
        //double underscores indicate static vars
        __x = 0
        __y = 0
        __w = 320
        __h = 240
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        //App.requestPopUp(false)
        Draw.fill(0,0,0)
        
        
        
        __dw = 10.23
        __dh = 15.11
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        _demo_loops = 4
        _r = 0
        _g = 0
        _b = 0
        _Gain = 3.51
        _MaxIters = 100
        _Zoom = 1
        _MoveX = 0
        _MoveY = 0
        _CX = 0.01
        _CY = 0.6015
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
                _demo_loops = _demo_loops - 1
                _jy = 0
                _Zoom = _Zoom + ( _Zoom * 1.1)
                _CY = _CY + 0.02
                _CX = _CX - (_CX * 0.02)
                _Gain = _Gain + 1.58
                _MaxIters = (_MaxIters + (_Zoom  )) * _Gain 
                if (_MaxIters > 100){
                    _MaxIters = 100
                    //_Zoom = 0.9
                    //_Gain = 1
                }
            }
        } else _jx = _jx + 1
        var r = (i*_jx/(__w*2))%(400)
        var g = (400-_jx)%(400)/2
        var b = (i)%400
        g = (g * b)
        if (r < 0) r = 0
        if (g < 0) g = 0
        if (b < 0) b = 0
        if (r > 200) r = 200 - (r - 200)
        if (g > 200) g = 200 - (g - 200)
        if (b > 200) b = 200 - (b - 200)
        
        var pc
        pc = Draw.getPixel(_jx,_jy)
        //CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
        var ar = ( (i * 1 * r) + (i * 12 * (pc >> 8)))/(i*13)
        var ag = ( (i * 1 * g) + (i * 12 * ((pc >> 3) & 252)))/(i*13)
        var ab = ( (i * 1 * b) + (i * 12 * ((pc << 3)  & 255)))/(i*13)
        //if (g > ag) ag = g
        //ar = ag
        Draw.setPixel(_jx, _jy,ar,ag,ab)
    }
    
    
    
    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    update() {
        _count = _count + 1
        if (_count > 1000){
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP"),"iter",_MaxIters])
            _count = 0
            
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_32")
        }
    }
    render() {
        App.setWidgetPosition(__x, __y)
        //every pixel is calculated by many iterations
        //kick up the clock speed for this section
        App.setClockSpeed(740000000)
        for (y in 0...(1024)) {
            createJulia()
        }
        //for (y in 0...(5)) {
        //    spiral()
        //}
        App.setClockSpeed(700000000)
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
System.print("example_31")


