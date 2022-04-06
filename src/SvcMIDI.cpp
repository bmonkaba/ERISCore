/**
 * @file svcMIDI.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-MAR-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SvcMidi.h"
#include <MIDI.h>
#include <midi_Defs.h>

#ifdef USE_USB_MIDI

#else
MIDI_CREATE_INSTANCE(HardwareSerial, Serial8, usbMIDI);
#endif

unsigned long last_midi_clock_rxtime;
unsigned long last_quarter_note_time;
unsigned long last_eighth_note_time;
uint8_t midi_clock_counter;
bool quarter_note_tick;
bool eighth_note_tick;
unsigned long quarter_note_count;
unsigned long eighth_note_count;
unsigned long sixteenth_note_count;
unsigned long thirtysecond_note_count;
unsigned long last_arp_note_count;
float tempo;
float last_note;
unsigned long midi_update_time;
uint8_t midi_note;
uint8_t last_midi_note;

void handleErrorCallback(int8_t error_code){}

void handleNoteOffCallback(byte channel, byte note, byte velocity){
    char msg[32];
    AppManager* am = AppManager::getInstance();
    SvcMIDI* m = (SvcMIDI*)am->getAppByName("MIDI");
    sprintf(msg,"NOTE_OFF %d %d %d",channel,note,velocity);
    m->publish(msg);
}

void handleNoteOnCallback(byte channel, byte note, byte velocity){
    char msg[32];
    AppManager* am = AppManager::getInstance();
    SvcMIDI* m = (SvcMIDI*)am->getAppByName("MIDI");
    if (note >= 12){
        sprintf(msg,"NOTE_ON %d %d %d %f",channel,note,velocity,note_freq[note-12]);
        m->publish(msg);
    }
}

void handleAfterTouchPolyCallback(byte channel, byte note, byte velocity){}

void handleControlChangeCallback(byte channel, byte control, byte value){
    char msg[32];
    AppManager* am = AppManager::getInstance();
    SvcMIDI* m = (SvcMIDI*)am->getAppByName("MIDI");
    sprintf(msg,"CC %d %d %d",channel,control,value);
    m->publish(msg);
}

void handleProgramChangeCallback(byte channel, byte program){
    char msg[32];
    AppManager* am = AppManager::getInstance();
    SvcMIDI* m = (SvcMIDI*)am->getAppByName("MIDI");
    sprintf(msg,"PC %d %d",channel,program);
    m->publish(msg);
}
void handleAfterTouchChannelCallback(byte channel, byte pressure){}
void handlePitchBendCallback(byte channel, int pitch){}
void handleSystemExclusiveCallback(byte *array, unsigned size){}
void handleTimeCodeQuarterFrameCallback(byte data){}
void handleSongPositionCallback(uint16_t beats){}
void handleSongSelectCallback(byte songnumber){}
void handleTuneRequestCallback(void){}

void handleClockCallback(){
  unsigned long t = micros();
  last_midi_clock_rxtime = t;
  if(midi_clock_counter%2) usbMIDI.sendRealTime(midi::Clock);
  midi_clock_counter += 1;
  if(midi_clock_counter % 3 == 0) thirtysecond_note_count++;
  if(midi_clock_counter % 6 == 0) sixteenth_note_count++;
  if(midi_clock_counter % 12 == 0) {eighth_note_count++; last_eighth_note_time = t; eighth_note_tick ^= true;}
  if(midi_clock_counter % 24 == 0){
    quarter_note_tick ^= true;
    midi_clock_counter = 0;
    tempo = round(60.0 * (1.0 / ((float)(t - last_quarter_note_time) / 1000000.0)));
    if (tempo > 240.0) tempo = 240.0;
    AppManager* am = AppManager::getInstance();
    am->data->update("TEMPO",tempo);
    last_quarter_note_time = t;
    quarter_note_count += 1;
  }
}

void handleStartCallback(void){}
void handleTickCallback(void){}
void handleHandleContinue(void){}
void handleStopCallback(void){}
void handleActiveSensingCallback(void){}
void handleSystemResetCallback(void){}

void SvcMIDI::midiInit(){
    //start midi
    #ifndef USE_USB_MIDI
    usbMIDI.begin(MIDI_CHANNEL_OMNI); //MIDI_LISTEN_CHANNEL - default listen on channel 7 (set to MIDI_CHANNEL_OMNI to receive all)
    usbMIDI.setHandlePitchBend(handlePitchBendCallback);
    usbMIDI.setHandleError(handleErrorCallback);
    #endif
    
    usbMIDI.setHandleNoteOff(handleNoteOffCallback);
    usbMIDI.setHandleNoteOn(handleNoteOnCallback);
    usbMIDI.setHandleAfterTouchPoly(handleAfterTouchPolyCallback);
    usbMIDI.setHandleControlChange(handleControlChangeCallback);
    usbMIDI.setHandleProgramChange(handleProgramChangeCallback);
    usbMIDI.setHandleAfterTouchChannel(handleAfterTouchChannelCallback);
    #ifdef USE_USB_MIDI
    usbMIDI.setHandlePitchChange(handlePitchBendCallback);
    usbMIDI.setHandleSongPosition(handleSongPositionCallback);
    #endif
    usbMIDI.setHandleSystemExclusive(handleSystemExclusiveCallback);
    usbMIDI.setHandleTimeCodeQuarterFrame(handleTimeCodeQuarterFrameCallback);
    usbMIDI.setHandleSongSelect(handleSongSelectCallback);
    usbMIDI.setHandleTuneRequest(handleTuneRequestCallback);
    usbMIDI.setHandleClock(handleClockCallback);
    usbMIDI.setHandleStart(handleStartCallback);
    //usbMIDI.setHandleTick(handleTickCallback);
    usbMIDI.setHandleContinue(handleHandleContinue);
    usbMIDI.setHandleStop(handleStopCallback);
    usbMIDI.setHandleActiveSensing(handleActiveSensingCallback);
    usbMIDI.setHandleSystemReset(handleSystemResetCallback);

    //usbMIDI.turnThruOff();
    last_midi_clock_rxtime = 0;
    last_quarter_note_time = micros();
    last_eighth_note_time = micros();
    midi_clock_counter = 0;
    tempo = 90;
    quarter_note_tick = false;
    quarter_note_count = 0;
    eighth_note_count = 0;
    sixteenth_note_count = 0;
    last_arp_note_count = 0;
    midi_note = 0;
    last_midi_note = 1;
    //end global var init
    midi_init = true;
    usbMIDI.begin();
}; 

bool SvcMIDI::subscribe(uint16_t app_id){
    //find the next available slot
    for(uint16_t i = 0; i < MAX_MIDI_MESSAGE_APP_SUBSCRIPTIONS; i ++){
        if (subscriptions[i] == 0 || subscriptions[i] == app_id){
            subscriptions[i] = app_id;
            return true;
        }
    }
    return false;
}

void SvcMIDI::publish(const char* message){
    for(uint16_t i = 0; i < MAX_MIDI_MESSAGE_APP_SUBSCRIPTIONS; i ++){
        if (subscriptions[i] == 0) return;
        am->sendMessage(this,am->getApp(subscriptions[i])->name,message);
        am->data->increment("MIDI_MSGS_PUB");
    }
}

void SvcMIDI::update(){
    while(usbMIDI.read(MIDI_LISTEN_CHANNEL)){
        am->data->increment("MIDI_MSGS_RX");
    };
}