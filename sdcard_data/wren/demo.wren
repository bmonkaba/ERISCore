class Data {
    foreign static update(key,double_value)
    foreign static updatef(key,double_value)
    foreign static read(key)
    foreign static readf(key)
}

class App {
    foreign static sendMessage(to, message)
    foreign static setPosition(x, y)
    foreign static setDimension(width, height)
    foreign static setWidgetPosition(x, y)
    foreign static setWidgetDimension(width, height)
    foreign static requestPopUp(exclusive)
    foreign static releasePopUp()
    foreign static getFocus()
    foreign static returnFocus()
    //extra methods - extending beyond the C++ AppBase class
    foreign static restartVM(module_name)
    foreign static setClockSpeed(hz)

    construct new() {
        //double underscores indicate static class vars
        __x = 64
        __y = 64
        __w = 64
        __h = 64
        //single underscores indicate class instance vars
        //in wren the class itself is an object
        //so it will also have instance vars
        _count = 0
    }
    
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //
    updateRT() {
         
    }
    update() {
        _count = _count + 1
        if (_count > 5){
            App.restartVM("example")
        }
        App.setClockSpeed(600000000)
    }
    onFocus() {
    }
    onFocusLost() {
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
}

//note the demo timer will not report correct until the second loop 
//through the demo
var demo_loop_start_time = Data.readf("DEMO_LOOP_TIME")
var current_time = System.clock
var is_already_in_demo_mode = Data.read("DEMO_MODE")
var ErisApp = App.new()
if (is_already_in_demo_mode == 1){
    Data.update("DEMO_MODE",0)    
    Data.updatef("DEMO_LOOP_TIME",0)
    System.print("/////////////////////////////////////////////////////////")
    System.print("-------------------* DEMO COMPLETE *---------------------")
    System.print(".........................................................")
    System.print(["demo total_time (seconds)",current_time - demo_loop_start_time])
    System.print(".........................................................")
    System.print("---------------------------------------------------------")
}else{
    Data.update("DEMO_MODE",1)
    System.print("Demo - Enters demo mode which will cause the examples to cycle one time.")
    System.print("       This also serves as the autonmated system integration and regression test set for Eris Core")
    System.print("/////////////////////////////////////////////////////////")
    System.print("---------------------------------------------------------")
    System.print(".........................................................")
    System.print(["demo start_time (seconds)",current_time - demo_loop_start_time])
    System.print(".........................................................")
    System.print("---------------------------------------------------------")
    
    Data.updatef("DEMO_LOOP_TIME",current_time)
    
}



