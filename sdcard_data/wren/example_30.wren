/*  
    Eris Core Wren Script Template
    https://wren.io/ for more information about the programming language
    https://rosettacode.org/wiki/Category:Wren examples
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
        __x = 64
        __y = 64
        __w = 64
        __h = 64
        AudioDirector.disconnect("amp_2",0)
        AudioDirector.connect("tonesweep_1 ",0,"amp_2",0)
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
    }
    
    //it's possible to include foreign methods (C/C++) within a wren class
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
    //  - in case these methods are not found in a scrip, method forwarding will halt
    //    until the next restartVM() call or external script load event 
    update() {
        _count = _count + 1
        if (_count > 9000){
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
             //send an audio parameter control message
             App.sendMessage("APC", "erisAudioSynthToneSweep 1 play 0.1,20,2700,6.0")
            _count = 0
        }
    }
    render() {
        App.setWidgetPosition(__x, __y)
        Draw.setCursor(5 + __x,__y -9)
        Draw.print("Hello World!")
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
        System.println([sender,message])
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



