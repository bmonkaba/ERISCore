/*  
example_21 - Audio Connection System Test
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
        __x = 0
        __y = 0
        __w = 1
        __h = 1
        __haspop = false
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        App.requestPopUp(false)
        _count = 0
        _demo_loops = 200
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
    foreign static setClockSpeed(hz)
    foreign static random(how_big)
    foreign static restartVM(script)

    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    update() {
        _count = _count + 1
        if (_count > 5000){
            _count = 0
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            //Draw.fill(0,0,0)
            var AD = AudioDirector
            var source_streams = ["pink:1","i2s-in:1","waveform:1","waveform:2","waveform:3","waveform:4","waveform:5"]
            var sink_streams = ["scope:1", "i2s-out:1"]
            var io_streams = ["mixer:1", "mixer:2","delay:1","biquad:1","biquad:2","amp:2", "console:1","console:2"]
            AD.disconnectAll()
            
            
            var i = io_streams[App.random(io_streams.count)]
            var j = io_streams[App.random(io_streams.count)]
            var k = io_streams[App.random(io_streams.count)]
            
            var input = source_streams[App.random(source_streams.count)]
            var output = sink_streams[App.random(sink_streams.count)]
            
            System.print([input,"->",i])//App.random(io_streams.count))
            AD.connect(input, 0, i, 0)
            
         
            
            System.print([i,"->",j])
            if (i != j) AD.connect(i, 0, j, 0)
            System.print([j,"->",k])
            if (k != j) AD.connect(j, 0, k, 0)
            
            
            System.print([k,"->",output])
            AD.connect(k, 0, output, 0)
            
            _demo_loops = _demo_loops - 1
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_22")
        }
    }
    render() {
        
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
System.print("example_21")

















