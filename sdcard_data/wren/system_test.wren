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
import "queue" for Queue

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
    foreign static setTextColor(r,g,b)
    foreign static setCursor(x,y)
    foreign static print(string)
}
//AppBase Class interface for implementing the scripts actions & behaviors
class App {
    construct new() {
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
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    updateRT() {
        _count = _count + 1
        if (_count > 15000){
            System.print(["FREE_MEM",Data.read("FREE_MEM")])
            System.print(["CPU_TEMP",Data.read("CPU_TEMP")])
            _count = 0
        }
    }
    update() {
        //App.setClockSpeed(600000000)
        var q = Queue.new()
        q.push(1)
        q.push(2)
        System.print("Queue contains %(q)")
        System.print("Number of elements in queue = %(q.count)")
        var item = q.pop()
        System.print("'%(item)' popped from the queue")
        System.print("First element is now %(q.peek())")
        q.clear()
        System.print("Queue cleared")
        System.print("Is queue now empty? %((q.isEmpty) ? "yes" : "no")")
        //App.setClockSpeed(600000000)
    }

}
//static methods can be called directly on the class object
App.setDimension(64, 64)
App.setWidgetDimension(64, 64)


