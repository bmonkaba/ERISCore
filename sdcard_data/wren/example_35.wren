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
    foreign static setPixel(x,y,r,g,b)
    foreign static getPixel(x,y)
    foreign static line(x,y,x2,y2,r,g,b)
    foreign static fill(r,g,b)
    foreign static setTextColor(r,g,b)
    foreign static setCursor(x,y)
    foreign static print(string)
    foreign static print(string,x,y,font,font_point)
}
//AppBase Class interface for implementing the scripts actions & behaviors
class App {
    construct new() {
        //double underscores indicate static vars
        __x = 864
        __y = 64
        __w = 120
        __h = 120
        
        __path = "/I/U/S/TOON/ANIMALS/Square/"
        __fonts = ["'!PaulMaul'",
            "'!PaulMaul-b'",
            "ArchitectsDaughter",
            "Blokletters-Balpen",
            "Blokletters-Potlood",
            "Blokletters-Viltstift",
            "Brizel",
            "CamingoCode-Bold",
            "CamingoCode-BoldItalic",
            "CamingoCode-Italic",
            "CamingoCode-Regular",
            "Cascadia",
            "Dearest",
            "Dearest-open",
            "Dearest-outline",
            "ElliotSix",
            "EraserRegular",
            "Exotica",
            "LICENCE'",
            "Flux-Architect-Bold",
            "Flux-Architect-Bold-Italic",
            "Flux-Architect-Italic",
            "Flux-Architect-Regular",
            "FreeMono",
            "FreeMonoBold",
            "FreeMonoBoldOblique",
            "FreeMonoOblique",
            "FreeSans",
            "FreeSansBold",
            "FreeSansBoldOblique",
            "FreeSansOblique",
            "FreeSerif",
            "FreeSerifBold",
            "FreeSerifBoldItalic",
            "FreeSerifItalic",
            "GONGN---",
            "Gaegu-Bold",
            "Gaegu-Light",
            "Gaegu-Regular",
            "GenzschEtHeyse",
            "GenzschEtHeyseAlt",
            "GermaniaOne-Regular",
            "GloriaHallelujah",
            "HEAVY-DATA",
            "Hack-Bold",
            "Hack-BoldItalic",
            "Hack-Italic",
            "Hack-Regular",
            "IndieFlower",
            "JINKY",
            "Kalam-Bold",
            "Kalam-Light",
            "Kalam-Regular",
            "Kingthings-Gothique",
            "Kingthings-Spikeless",
            "Leafy-Glade",
            "Leafy-Shade",
            "LiberationMono-Bold",
            "LiberationMono-BoldItalic",
            "LiberationMono-Italic",
            "LiberationMono-Regular",
            "MajorMonoDisplay-Regular",
            "MountainsofChristmas",
            "Note-this",
            "OvertheRainbow",
            "PermanentMarker",
            "PlaytimeOblique",
            "PlaytimeWithHotToddies3D",
            "RAPSCALL",
            "Recursive-VF-1",
            "Redressed",
            "RobotoMono-Italic-VariableFont-wght",
            "RobotoMono-VariableFont-wght",
            "RockSalt",
            "Scratch-",
            "Snickles",
            "SpaceMono-Bold",
            "SpaceMono-BoldItalic",
            "SpaceMono-Italic",
            "SpaceMono-Regular",
            "TrashHand",
            "TungusFont-Tinet",
            "UnifrakturMaguntia",
            "UnifrakturMaguntia16",
            "UnifrakturMaguntia17",
            "UnifrakturMaguntia18",
            "UnifrakturMaguntia19",
            "UnifrakturMaguntia20",
            "UnifrakturMaguntia21",
            "Wagnasty",
            "belligerent",
            "hetilica",
            "journal",
            "justanotherhand-regular",
            "luximb",
            "luximbi",
            "luximr",
            "luximri",
            "playtime",
            "teutonic1",
            "teutonic2",
            "teutonic3",
            "teutonic4",
            "zenda"]
        __files = ["bear.ile",
            "bear_nz2.ile",
            "bear_nz4.ile",
            "buffalo.ile",
            "buffalo_nz2.ile",
            "buffalo_nz4.ile",
            "chick.ile",
            "chick_nz2.ile",
            "chick_nz4.ile",
            "chicken.ile",
            "chicken_nz2.ile",
            "chicken_nz4.ile",
            "cow.ile",
            "cow_nz2.ile",
            "cow_nz4.ile",
            "crocodile.ile",
            "crocodile_nz2.ile",
            "crocodile_nz4.ile",
            "dog.ile",
            "dog_nz2.ile",
            "dog_nz4.ile",
            "duck.ile",
            "duck_nz2.ile",
            "duck_nz4.ile",
            "elephant.ile",
            "elephant_nz2.ile",
            "elephant_nz4.ile",
            "frog.ile",
            "frog_nz2.ile",
            "frog_nz4.ile",
            "giraffe.ile",
            "giraffe_nz2.ile",
            "giraffe_nz4.ile",
            "goat.ile",
            "goat_nz2.ile",
            "goat_nz4.ile",
            "gorilla.ile",
            "gorilla_nz2.ile",
            "gorilla_nz4.ile",
            "hippo.ile",
            "hippo_nz2.ile",
            "hippo_nz4.ile",
            "horse.ile",
            "horse_nz2.ile",
            "horse_nz4.ile",
            "monkey.ile",
            "monkey_nz2.ile",
            "monkey_nz4.ile",
            "moose.ile",
            "moose_nz2.ile",
            "moose_nz4.ile",
            "narwhal.ile",
            "narwhal_nz2.ile",
            "narwhal_nz4.ile",
            "owl.ile",
            "owl_nz2.ile",
            "owl_nz4.ile",
            "panda.ile",
            "panda_nz2.ile",
            "panda_nz4.ile",
            "parrot.ile",
            "parrot_nz2.ile",
            "parrot_nz4.ile",
            "penguin.ile",
            "penguin_nz2.ile",
            "penguin_nz4.ile",
            "pig.ile",
            "pig_nz2.ile",
            "pig_nz4.ile",
            "rabbit.ile",
            "rabbit_nz2.ile",
            "rabbit_nz4.ile",
            "rhino.ile",
            "rhino_nz2.ile",
            "rhino_nz4.ile",
            "sloth.ile",
            "sloth_nz2.ile",
            "sloth_nz4.ile",
            "snake.ile",
            "snake_nz2.ile",
            "snake_nz4.ile",
            "walrus.ile",
            "walrus_nz2.ile",
            "walrus_nz4.ile",
            "whale.ile",
            "whale_nz2.ile",
            "whale_nz4.ile",
            "zebra.ile",
            "zebra_nz2.ile",
            "zebra_nz4.ile"]
        
        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        App.setWidgetPosition(__x, __y)
        App.requestPopUp(true)
        
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        _demo_loops = 100
        _pixels = 0
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
    update() {
        _count = _count + 1
        if (_count > 290){
            System.print(["UP_TIME",System.clock,"FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            var elapsed_time = System.clock - _timer
            _pixels = 0
            _timer = System.clock
            _count = 0
            _demo_loops = _demo_loops - 1
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("demo")
        }
    }
    
    render() {
        App.setWidgetPosition(__x, __y)
        var start = System.clock
        var x = App.random(120)
        var y = App.random(120)
        var z = App.random(__files.count)
        var filename = "portrait%(z)_nz2.ile"
        Draw.fill(0,0,0)
        //Draw.loadImageToSurface(__path,__files[z],x,y)
        Draw.print(__fonts[_demo_loops],10,95,__fonts[_demo_loops],18)
        Draw.print(System.clock.truncate.toString,10,220,__fonts[z],10)
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
System.print("-"*98)
System.print("example_35")
System.print("This is a demonstration of the system fonts available.Kerning not applied.")
System.print("-"*98)



















