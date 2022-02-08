/*  
    Eris Core Wren Script Template
    https://wren.io/ for more information about the programming language
    https://rosettacode.org/wiki/Category:Wren examples
*/
//audio director interface for making/breaking audiostream connections
class AudioDirector{
    //foreign methods are implemented in C/C++
    foreign static enableInterrupts()
    foreign static disableInterrupts()
    foreign static connect(source_audio_stream,source_port,
        dest_audio_stream,dest_port)
    foreign static disconnect(audio_stream,port)
    foreign static disconnectAll()
    foreign static getFunctionList()
    foreign static getTypeList(by_function)
    foreign static getTypeInstanceCount(by_type)
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
    
    construct new() {
        //double underscores indicate static vars
        __x = 0
        __y = 8
        __w = 64
        __h = 64
        //single underscores indicate class instance vars
        //in wren the class itself is an object so it will also have instance vars
        _count = 0
        
        _ad = AudioDirector
        _source_streams = []
        _sink_streams = []
        _io_streams = []
        _audio_name_obj_methods = [
            ["bitcrusher","erisAudioEffectBitcrusher","bits",["uint8_t-b"]], 
            ["bitcrusher","erisAudioEffectBitcrusher","sampleRate",["float-hz"]], 
            ["chorus","erisAudioEffectChorus","voices",["int-n_chorus"]], 
            ["combine","erisAudioEffectDigitalCombine","setCombineMode",["int-mode_in"]], 
            ["delay","erisAudioEffectDelay","delay",["uint8_t-channel","float-milliseconds"]], 
            ["delay","erisAudioEffectDelay","disable",["uint8_t-channel"]], 
            ["delay","erisAudioEffectDelay","recompute_maxblocks",["void"]], 
            ["delayExt","erisAudioEffectDelayExternal","delay",["uint8_t-channel", "float-milliseconds"]], 
            ["delayExt","erisAudioEffectDelayExternal","disable",["uint8_t-channel"]], 
            ["delayExt","erisAudioEffectDelayExternal","initialize",["erisAudioEffectDelayMemoryType_t-type", "uint32_t-samples"]], 
            ["delayExt","erisAudioEffectDelayExternal","zero",["uint32_t-address", "uint32_t-count"]], 
            ["envelope","erisAudioEffectEnvelope","noteOn",[""]], 
            ["envelope","erisAudioEffectEnvelope","noteOff",[""]], 
            ["envelope","erisAudioEffectEnvelope","delay",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","attack",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","hold",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","decay",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","sustain",["float-level"]], 
            ["envelope","erisAudioEffectEnvelope","release",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","releaseNoteOn",["float-milliseconds"]], 
            ["envelope","erisAudioEffectEnvelope","isActive",[""]], 
            ["envelope","erisAudioEffectEnvelope","isSustain",[""]], 
            ["fade","erisAudioEffectFade","fadeIn",["uint32_t-milliseconds"]], 
            ["fade","erisAudioEffectFade","fadeOut",["uint32_t-milliseconds"]], 
            ["fade","erisAudioEffectFade","fadeBegin",["uint32_t-newrate","uint8_t-dir"]], 
            ["flange","erisAudioEffectFlange","voices",["int-delay_offset","int-d_depth", "float-delay_rate"]], 
            ["freeverb","erisAudioEffectFreeverb","roomsize",["float-n"]], 
            ["freeverb","erisAudioEffectFreeverb","damping",["float-n"]], 
            ["freeverbs","erisAudioEffectFreeverbStereo","roomsize",["float-n"]], 
            ["freeverbs","erisAudioEffectFreeverbStereo","damping",["float-n"]], 
            ["granular","erisAudioEffectGranular","setSpeed", ["float-ratio"]], 
            ["granular","erisAudioEffectGranular","beginFreeze", ["float-grain_length"]], 
            ["granular","erisAudioEffectGranular","beginPitchShift", ["float-grain_length"]], 
            ["granular","erisAudioEffectGranular","stop", [""]], 
            ["granular","erisAudioEffectGranular","beginFreeze_int", ["int-grain_samples"]], 
            ["granular","erisAudioEffectGranular","beginPitchShift_int", ["int-grain_samples"]], 
            ["reverb","erisAudioEffectReverb","reverbTime", ["float"]], 
            ["reverb","erisAudioEffectReverb","init_comb_filters", ["void"]], 
            ["reverb","erisAudioEffectReverb","clear_buffers", ["void"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","reset", [""]], 
            ["fft1024","erisAudioAnalyzeFFT1024","enableFFT", ["bool-enable_state"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","configSubsample", ["uint16_t-subsample", "subsample_range-range"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","setActiveRange", ["subsample_range-range"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","toggleActiveRange", [""]], 
            ["fft1024","erisAudioAnalyzeFFT1024","available", [""]], 
            ["fft1024","erisAudioAnalyzeFFT1024","read", ["uint32_t-binNumber"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","averageTogether", ["uint8_t-n"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","spectralFilter", [""]], 
            ["fft1024","erisAudioAnalyzeFFT1024","capture", ["void"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","analyze", ["void"]], 
            ["fft1024","erisAudioAnalyzeFFT1024","init", ["void"]], 
            ["scope","erisAudioAnalyzeScope","trigger", ["void"]], 
            ["scope","erisAudioAnalyzeScope","trigger", ["float-level", "int-edge"]], 
            ["scope","erisAudioAnalyzeScope","available", ["void"]], 
            ["scope","erisAudioAnalyzeScope","getDotProduct", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotProductAvg", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotProductAvgSlow", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotMACD", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotDelta", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotAcceleration", [""]], 
            ["scope","erisAudioAnalyzeScope","getDotDeltaMACD", [""]], 
            ["scope","erisAudioAnalyzeScope","getEdgeCount", [""]], 
            ["scope","erisAudioAnalyzeScope","getEdgeCount_ch2", [""]], 
            ["scope","erisAudioAnalyzeScope","getEdgeDelay", [""]], 
            ["scope","erisAudioAnalyzeScope","getEdgeDelay2", [""]], 
            ["waveform","erisAudioSynthWaveform","frequency", ["float-freq"]], 
            ["waveform","erisAudioSynthWaveform","phase", ["float-angle"]], 
            ["waveform","erisAudioSynthWaveform","amplitude", ["float-n"]], 
            ["waveform","erisAudioSynthWaveform","offset", ["float-n"]], 
            ["waveform","erisAudioSynthWaveform","pulseWidth", ["float-n"]], 
            ["waveform","erisAudioSynthWaveform","begin", ["short-t_type"]], 
            ["waveform","erisAudioSynthWaveform","begin", ["float-t_amp", "float-t_freq", "short-t_type"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","frequency", ["float-freq"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","amplitude", ["float-n"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","offset", ["float-n"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","begin", ["short-t_type"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","begin", ["float-t_amp", "float-t_freq", "short-t_type"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","frequencyModulation", ["float-octaves"]], 
            ["waveform_mod","erisAudioSynthWaveformModulated","phaseModulation", ["float-degrees"]], 
            ["fir","erisAudioFilterFIR", "end", ["void"]], 
            ["ladder","erisAudioFilterLadder","frequency", ["float-FC"]], 
            ["ladder","erisAudioFilterLadder","resonance", ["float-reson"]], 
            ["ladder","erisAudioFilterLadder","octaveControl", ["float-octaves"]], 
            ["ladder","erisAudioFilterLadder","passbandGain", ["float-passbandgain"]], 
            ["ladder","erisAudioFilterLadder","inputDrive", ["float-drv"]], 
            ["ladder","erisAudioFilterLadder","interpolationMethod", ["erisAudioFilterLadderInterpolation-im"]], 
            ["ladder","erisAudioFilterLadder","compute_coeffs", ["float-fc"]], 
            ["ladder","erisAudioFilterLadder","initpoly", [""]], 
            ["ladder","erisAudioFilterLadder","resonating", [""]], 
            ["filter","erisAudioFilterStateVariable","frequency", ["float-freq"]], 
            ["filter","erisAudioFilterStateVariable","resonance", ["float-q"]], 
            ["filter","erisAudioFilterStateVariable","octaveControl", ["float-n"]], 
            ["mixer","erisAudioMixer4","gain", ["uint32_t-channel", "float-gain"]], 
            ["mixer","erisAudioMixer4","gain", ["uint32_t-channel", "float-gain"]], 
            ["amp","erisAudioAmplifier","gain", ["float-n"]], 
            ["dc","erisAudioSynthWaveformDc","amplitude", ["float-n"]], 
            ["dc","erisAudioSynthWaveformDc","amplitude", ["float-n", "float-milliseconds"]], 
            ["dc","erisAudioSynthWaveformDc","read", ["void"]], 
            ["string","erisAudioSynthKarplusStrong", "noteOn", ["float-frequency", "float-velocity"]], 
            ["string","erisAudioSynthKarplusStrong", "noteOff", ["float-velocity"]], 
            ["pink","erisAudioSynthNoisePink","amplitude", ["float-n"]], 
            ["pwm","erisAudioSynthWaveformPWM","frequency", ["float-freq"]], 
            ["pwm","erisAudioSynthWaveformPWM","amplitude", ["float-n"]], 
            ["drum","erisAudioSynthSimpleDrum","noteOn", [""]], 
            ["drum","erisAudioSynthSimpleDrum","frequency", ["float-freq"]], 
            ["drum","erisAudioSynthSimpleDrum","length", ["int32_t-milliseconds"]], 
            ["drum","erisAudioSynthSimpleDrum","secondMix", ["float-level"]], 
            ["drum","erisAudioSynthSimpleDrum","pitchMod", ["float-depth"]], 
            ["sine","erisAudioSynthWaveformSine","frequency", ["float-freq"]], 
            ["sine","erisAudioSynthWaveformSine","phase", ["float-angle"]], 
            ["sine","erisAudioSynthWaveformSine","amplitude", ["float-n"]], 
            ["sine_hires","erisAudioSynthWaveformSineHires","frequency", ["float-freq"]], 
            ["sine_hires","erisAudioSynthWaveformSineHires","phase", ["float-angle"]], 
            ["sine_hires","erisAudioSynthWaveformSineHires","amplitude", ["float-n"]], 
            ["sine_fm","erisAudioSynthWaveformSineModulated","frequency", ["float-freq"]], 
            ["sine_fm","erisAudioSynthWaveformSineModulated","phase", ["float-angle"]], 
            ["sine_fm","erisAudioSynthWaveformSineModulated","amplitude", ["float-n"]], 
            ["tonesweep","erisAudioSynthToneSweep","play", ["float-t_amp", "int-t_lo", "int-t_hi", "float-t_time"]], 
            ["tonesweep","erisAudioSynthToneSweep","isPlaying", ["void"]], 
            ["tonesweep","erisAudioSynthToneSweep","read", ["void"]], 
            ["wavetable","erisAudioSynthWavetable","amplitude", ["float-v"]], 
            ["wavetable","erisAudioSynthWavetable","stop", ["void"]], 
            ["wavetable","erisAudioSynthWavetable","setFrequency", ["float-freq"]], 
            ["wavetable","erisAudioSynthWavetable","getEnvState", ["void"]], 
            ["wavetable","erisAudioSynthWavetable","setState", ["int-note", "int-amp", "float-freq"]], 
            ["noise","erisAudioSynthNoiseWhite","amplitude", ["float-n"]],
            ["biquad","erisAudioFilterBiquad", "setLowpass", ["uint32_t-stage", "float-frequency", "float-q"]],
            ["biquad","erisAudioFilterBiquad", "setHighpass", ["uint32_t-stage", "float-frequency", "float-q"]],
            ["biquad","erisAudioFilterBiquad", "setBandpass", ["uint32_t-stage", "float-frequency", "float-q"]],
            ["biquad","erisAudioFilterBiquad", "setNotch", ["uint32_t-stage", "float-frequency", "float-q"]],
            ["biquad","erisAudioFilterBiquad", "setLowShelf", ["uint32_t-stage", "float-frequency", "float-gain", "float-slope"]],
            ["biquad","erisAudioFilterBiquad", "setHighShelf", ["uint32_t-stage", "float-frequency", "float-gain", "float-slope"]]
            ]

        App.requestPopUp(true)
        Draw.fill(0,0,60)
        Draw.loadImage("/I/U/W/","bluewithcolorlines.ile",0,0)
        
        AudioDirector.disableInterrupts()
        AudioDirector.disconnectAll()
        AudioDirector.connect("pink:1",0,"biquad:1",0)
        AudioDirector.connect("pink:1",0,"scope:1",0)
        AudioDirector.connect("biquad:1",0,"fft1024:1",0)
        AudioDirector.connect("biquad:1",0,"fft1024:2",0)
        AudioDirector.connect("biquad:1",0,"scope:1",1)
        AudioDirector.connect("pink:1",0,"i2s-out:1",0)
        AudioDirector.enableInterrupts()
        
        getAudioPoolInfo()
    }
    
    getAudioPoolInfo(){
        System.print("*" * 98)
        System.print("AudioBlock Pool")
        System.print("*" * 98)
        _source_streams = []
        _sink_streams = []
        _io_streams = []
        var functions = AudioDirector.getFunctionList()
        for (f in functions){
            var types =  AudioDirector.getTypeList(f)
            for (t in types){
                var ti = AudioDirector.getTypeInstanceCount(t)
                System.print("function:" + f + " type:" + t + " instances:" + ti.toString + "\nVM  methods:")
                var meth = getAudioMethodsByType(t)
                var meth_str = ""
                if(meth.count > 0){
                    for (i in 0..meth.count-1){
                        System.write("  ")
                        System.printAll(meth[i])
                        //System.print("\n")
                    }
                }
                System.print("\n")
                if(f == "synth-function"){
                    for (i in 1 ..(ti)){
                        _source_streams.add(t+":"+i.toString)
                    }
                }else if(f == "output-function"){
                    for (i in 1 ..(ti)){
                        _sink_streams.add(t+":"+i.toString)
                    }
                }else if(f == "analyze-function"){
                    for (i in 1 ..(ti)){
                        _sink_streams.add(t+":"+i.toString)
                    }
                }else {
                    for (i in 1 ..(ti)){
                        _io_streams.add(t+":"+i.toString)
                    }
                }
            }
        }
        System.print("*" * 98)
        System.print("sources:")
        System.print(_source_streams)
        System.print("-" * 98)
        System.print("sinks:")
        System.print(_sink_streams)
        System.print("-" * 98)
        System.print("io:")
        System.print(_io_streams)
        System.print("-" * 98)
    }

    getAudioMethodsByType(audio_block_type){
        var ret = []
        for (record in _audio_name_obj_methods){
            if (record[0] == audio_block_type) ret.add(record[1..-1])
        }
        return ret
    }

    //
    //  required methods - the VM host (C++ side) provides the AppBase class 
    //  wrappers which will forward the method calls here for execution
    //  - in case these methods are not found in a scrip, method forwarding will halt
    //    until the next restartVM() call or external script load event 
    
    render() {
        _count = _count + 1
        if (_count > 150){
            //System.print(["FREE_MEM",Data.read("FREE_MEM"),"CPU_TEMP",Data.readf("CPU_TEMP")])
            Draw.setCursor(__x,__y)
            Draw.print("""Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ac molestie justo, a aliquam libero. Integer sollicitudin sapien eu rutrum finibus. Nam in aliquet lectus. Suspendisse facilisis nunc sed congue feugiat. Donec quam risus, pharetra vitae gravida id, gravida sed odio. Nunc mattis gravida enim, nec posuere neque. Sed dictum quam volutpat, hendrerit sapien in, commodo turpis. Fusce fringilla finibus eros, id rhoncus leo luctus in. Mauris dictum, turpis ac euismod venenatis, urna sem varius nisi, vel pretium urna nunc feugiat nulla. Maecenas sem nulla, tempor id nulla dictum, ullamcorper gravida turpis. Duis lobortis velit nec accumsan dapibus. Nam elit neque, tristique venenatis augue ac, ultricies venenatis tellus. Aenean in dictum risus. Ut sit amet consectetur dui. Nunc vitae lectus eleifend, imperdiet nibh vel, mollis tellus. Donec turpis risus, pellentesque nec diam id, dignissim pellentesque felis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ac molestie justo, a aliquam libero. Integer sollicitudin sapien eu rutrum finibus. Nam in aliquet lectus. Suspendisse facilisis nunc sed congue feugiat. Donec quam risus, pharetra vitae gravida id, gravida sed odio. Nunc mattis gravida enim, nec posuere neque. Sed dictum quam volutpat, hendrerit sapien in, commodo turpis. Fusce fringilla finibus eros, id rhoncus leo luctus in. Mauris dictum, turpis ac euismod venenatis, urna sem varius nisi, vel pretium urna nunc feugiat nulla. Maecenas sem nulla, tempor id nulla dictum, ullamcorper gravida turpis. Duis lobortis velit nec accumsan dapibus. Nam elit neque, tristique venenatis augue ac, ultricies venenatis tellus. Aenean in dictum risus. Ut sit amet consectetur dui. Nunc vitae lectus eleifend, imperdiet nibh vel, mollis tellus. Donec turpis risus, pellentesque nec diam id, dignissim pellentesque felis.

Vestibulum finibus, tortor ac varius porttitor, urna urna vehicula turpis, vel commodo odio diam at erat. Sed vel ultrices nunc. Mauris gravida est nisl, lobortis pharetra quam vestibulum vitae. Nam in tristique ante. In bibendum luctus vulputate. Fusce mauris ante, accumsan sit amet vulputate et, venenatis et metus. Donec vel mattis elit.""")
            _count = 0
        }
    }
    update() {
        App.setWidgetPosition(__x, __y)
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
        System.print([sender,message])
    }
    //example getter
    count { _count }
    //example setter
    count=(value) { _count = value }
}
//static methods can be called directly on the class object
App.setDimension(320, 240)
App.setWidgetDimension(120, 120)
//or a class instance may be created
//The host (C/C++ side) expects a top level variable named ErisApp of 
//type class App it's this object instance for which the event methods 
//will be called
System.print("audio_method_test")
var ErisApp = App.new()



