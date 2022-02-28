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
        __x = 864
        __y = 64
        __w = 0
        __h = 0
        
        __path = "/I/U/S/TOON/ANIMALS/Square/"
        __fonts = [
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

        App.setDimension(__w, __h)
        App.setWidgetDimension(__w, __h)
        App.setWidgetPosition(__x, __y)
        App.requestPopUp(true)
        
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _file_index = 42
        _count = 0
        _demo_loops = __fonts.count
        _frames_rendered = 0
        _pixels = 0
        _timer = System.clock
        _dx = 0
        
        //format and load the initial fonts into the ram drive
        FileSystem.format()
        FileSystem.mkdir("fontcache")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","14.ile","/fontcache/","14.ile")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","18.ile","/fontcache/","18.ile")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","20.ile","/fontcache/","20.ile")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","28.ile","/fontcache/","28.ile")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","14.krn","/fontcache/","14.krn")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","18.krn","/fontcache/","18.krn")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","20.krn","/fontcache/","20.krn")
        FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","28.krn","/fontcache/","28.krn")
        
        FileSystem.importFromSD("/I/U/X/","software-3682509.ile","/","a.ile")
        FileSystem.importFromSD("/I/U/S/BG/","skull-gf8f5017d4_1920.ile","/","b.ile")
            
        System.print(FileSystem.usedSize().toString)
        Draw.fill(0,0,0)
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
        if (_count > 100){
            //System.print(["UP_TIME",System.clock,"FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            _count = 0
            
            if (_demo_loops == 0 &&  Data.read("DEMO_MODE") == 1) App.restartVM("example_37")
        }
    }
    
    render() {
        App.setWidgetPosition(__x, __y)
        
        _timer = System.clock
        var x = App.random(120)
        var y = App.random(120)
        var z = App.random(__fonts.count)
        //Draw.loadImageToSurface(__path,__files[z],x,y)
        _frames_rendered = _frames_rendered + 1
        if (_frames_rendered >= 20){
            _frames_rendered = 0
            _file_index = _file_index + 1
            if (_file_index >= __fonts.count) _file_index = 0
            _demo_loops = _demo_loops - 1
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","14.ile","/fontcache/","14.ile")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","18.ile","/fontcache/","18.ile")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","20.ile","/fontcache/","20.ile")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","28.ile","/fontcache/","28.ile")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","14.krn","/fontcache/","14.krn")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","18.krn","/fontcache/","18.krn")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","20.krn","/fontcache/","20.krn")
            FileSystem.importFromSD("/I/U/F/" + __fonts[_file_index] + "/","28.krn","/fontcache/","28.krn")
            
            return
        }
        
        var f = __fonts[_file_index].split("-")
        Draw.useWrenFileSystem()
        Draw.loadImage("/","a.ile",-_dx, -_dx,0)
        _dx = _dx + 1
        if (_dx > 160){
            _dx = 0
        }
        Draw.loadImage("/","b.ile",-150,-60,8)
        Draw.print(f[0],10,25,"fontcache",18)
        if (f.count > 1) Draw.print(f[-1],10,50,"fontcache",14)
        Draw.print("The Quick Brown Fox",10,80,"fontcache",20)
        Draw.print("Jumps Over The Lazy Dog.",10,110,"fontcache",18)
        Draw.print("abcdefghijklmnopqrstuvwxyz",5,140,"fontcache",14)
        Draw.print("zyxwvutsrqponmlkjihgfedcba",5,160,"fontcache",14)
        var elapsed_time = (System.clock - _timer) * 1000
        Draw.print(elapsed_time.truncate.toString + " mSec",10,190,"fontcache",20)
        Draw.useSDFileSystem()
        
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
System.print("example_36")
System.print("This is a demonstration of the system fonts available.Kerning not applied.")
System.print("-"*98)




