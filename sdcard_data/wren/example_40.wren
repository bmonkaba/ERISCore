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
    foreign static format()
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
    render() {
        _count = _count + 1
        if (_count > 15000){
            System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            _count = 0
        }
    }
    update() {
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
        System.println([sender,message])
    }
    //example getter
    count { _count }
    //example setter
    count=(value) { _count = value }
}

System.print("-"*98)
System.print("example_40 - File System")
System.print("-"*98)
//static methods can be called directly on the class object
App.setDimension(120, 120)
App.setWidgetDimension(120, 120)
FileSystem.format()
//or a class instance may be created
//The host (C/C++ side) expects a top level variable named ErisApp of 
//type class App it's this object instance for which the event methods 
//will be called
var ErisApp = App.new()

var w = """
orem ipsum dolor sit amet, consectetur adipiscing elit. Proin sapien sem, pulvinar ac justo id, volutpat mollis arcu. Curabitur pretium commodo dolor in fermentum. Vivamus vitae eros laoreet, pretium quam maximus, pretium dolor. Vestibulum eu tellus elit. Curabitur nulla arcu, congue eleifend mi in, bibendum dignissim lorem. Nullam vel ex id quam porttitor commodo non posuere tellus. Praesent elementum vulputate massa, ac finibus enim accumsan non. Curabitur posuere pellentesque condimentum. Sed interdum neque lacinia mattis pharetra. Nam elementum est accumsan, efficitur ante sed, condimentum arcu. Donec nisi justo, consequat quis nibh et, interdum molestie nisl.

Vestibulum sem magna, condimentum eget molestie sed, gravida ut nibh. Fusce eu nisl et odio tincidunt sagittis sed quis leo. Donec cursus, urna ac tincidunt volutpat, lectus augue dignissim massa, lobortis semper ante magna et erat. Praesent hendrerit vestibulum justo id dapibus. Nam cursus ultrices turpis vel ullamcorper. In pharetra non augue bibendum hendrerit. Phasellus ullamcorper nisl ac risus dictum viverra. Pellentesque lobortis tortor ac metus placerat, non laoreet lorem convallis.

Nullam maximus, erat nec posuere sollicitudin, nisi justo laoreet dui, vitae mattis lacus enim a purus. Quisque tempus ut nunc sed dapibus. In fermentum dui quis nibh commodo, sed rutrum turpis cursus. Proin in velit id odio tristique vestibulum. Nunc et tortor sit amet lacus porta vehicula. Quisque sed diam tempor, tempor dolor at, venenatis felis. Vestibulum commodo arcu nec commodo pretium. Suspendisse tristique eros ut purus sollicitudin, vitae blandit ante cursus. Etiam aliquam posuere laoreet.

Sed vitae nulla odio. Sed sagittis dui a fringilla pretium. Maecenas pellentesque urna nunc, eu condimentum mauris tincidunt non. Proin id rutrum elit. Aenean sit amet leo finibus, finibus neque at, tristique tortor. Aenean ac nunc ut est condimentum viverra vitae quis risus. Aliquam quis auctor quam, ac tempus ante. Quisque vestibulum id ligula a suscipit. In lacus felis, vehicula ut aliquet et, convallis vitae dui. Ut condimentum neque sem, id lacinia odio consequat ac. Sed gravida velit mauris, ac fermentum quam ornare a. Integer in neque orci. Suspendisse eget vehicula purus. Aliquam nibh sem, ultrices non lorem vitae, porta aliquet risus. Sed pulvinar gravida sagittis. Maecenas nec malesuada nisi.

Nulla porttitor nisi sit amet tincidunt elementum. Suspendisse vehicula efficitur turpis quis accumsan. Phasellus efficitur mattis mi. Duis cursus sollicitudin tellus, ac faucibus nunc fringilla et. Integer porta diam tellus, vel interdum lectus aliquet et. Ut est risus, pharetra in eros scelerisque, porta maximus risus. Ut gravida id leo eget efficitur. Curabitur aliquet, nisi et feugiat luctus, lorem ante porta nisl, sagittis tincidunt purus orci a nulla. Vivamus gravida lectus magna, id sollicitudin magna bibendum eu. Nulla ut sapien velit. Maecenas at elit lacus. Mauris mollis turpis at urna tempor eleifend. Integer rhoncus elementum tempor. Duis aliquet, lectus faucibus lobortis sollicitudin, lorem odio convallis urna, et molestie eros enim quis lorem. Etiam interdum, est vitae vulputate pulvinar, diam urna iaculis eros, at suscipit leo orci in purus. Vestibulum sem massa, posuere luctus elementum sed, bibendum at nibh.

Donec ac nisl a quam facilisis cursus. Phasellus quis magna massa. Vivamus imperdiet ipsum et accumsan vehicula. Praesent commodo arcu auctor tellus ornare, sed convallis est ultrices. Quisque non aliquam risus. Integer id imperdiet massa, eget pellentesque mi. Cras vehicula rutrum lorem, et porta ante malesuada non. Sed consequat placerat rhoncus. Mauris dapibus imperdiet purus sed laoreet. Pellentesque ut lobortis libero, sit amet euismod neque. Quisque dapibus tristique elit, congue dapibus erat viverra a. Nam varius risus nibh. In viverra consequat justo et scelerisque. Proin venenatis dictum mi nec pretium.
"""

FileSystem.open("test.txt","a")

for (y in 0...(300)) {
    File.write(w)
}

//File.flush()
File.close()

FileSystem.open("test.txt","r")
System.print(["isOpen",File.isOpen()])
System.print(["available",File.available()])
System.print(["position",File.position()])
System.print(["size",File.size()])
var r = File.read(20)
System.write(r + "..." + "\n")
File.close()
System.print(["isOpen",File.isOpen()])
//FileSystem.remove("test.txt")

System.print(["File System totalSize",FileSystem.totalSize()])
System.print(["File System usedSize",FileSystem.usedSize()])
System.print(["File System remainingSize",FileSystem.totalSize() - FileSystem.usedSize()])

FileSystem.importFromSD("/I/U/","UI.json",".","UI.json")
FileSystem.open("UI.json","r")
r = File.read(200)
System.write(r + "..." + "\n")
r = File.read(200)
System.write(r + "..." + "\n")
r = File.read(200)
System.write(r + "..." + "\n")
File.close()
System.print(["File System remainingSize",FileSystem.totalSize() - FileSystem.usedSize()])

System.print("Done")
if (Data.read("DEMO_MODE") == 1) App.restartVM("demo")








