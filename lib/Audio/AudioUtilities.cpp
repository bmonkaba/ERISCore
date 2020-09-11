#include "Arduino.h"
#include "AudioUtilities.h"


//chord progressions minor
//1,6,7
//1,4,7
//1,4,5
const uint8_t cp_m4_1[] = {1,6,3,7};
const uint8_t cp_m4_2[] = {1,6,4,5};
const uint8_t cp_m4_3[] = {1,6,4,3};
const uint8_t cp_m4_4[] = {6,5,1,3};
const uint8_t cp_m4_5[] = {1,4,5,5};
const uint8_t cp_m4_6[] = {1,4,6,5};
const uint8_t cp_m4_7[] = {6,3,5,1};
const uint8_t cp_m4_8[] = {3,1,5,7};
const uint8_t cp_m4_9[] = {6,4,1,3};
const uint8_t *cp_m[] ={&cp_m4_1[0],&cp_m4_2[0],&cp_m4_3[0],&cp_m4_4[0],&cp_m4_5[0],&cp_m4_6[0],&cp_m4_7[0],&cp_m4_8[0]};
/*
maj
1,5,6,4
1,6,4,5
1,4,5,4
5,4,1
*/

//chord quality https://en.wikipedia.org/wiki/Chord_(music)
/*
maj,1,0,0,0,1,0,0,1,0,0,0,0
maj6,1,0,0,0,1,0,0,1,0,1,0,0
dom7,1,0,0,0,1,0,0,1,0,0,1,0
maj7,1,0,0,0,1,0,0,1,0,0,0,1
aug,1,0,0,0,1,0,0,0,1,0,0,0
aug7,1,0,0,0,1,0,0,0,1,0,1,0
min,1,0,0,1,0,0,0,1,0,0,0,0
min6,1,0,0,1,0,0,0,1,0,1,,0
min7,1,0,0,1,0,0,0,1,0,0,1,0
minmaj7,1,0,0,1,0,0,0,1,0,0,0,1
dim,1,0,0,1,0,0,1,0,0,0,0,0
dim7,1,0,0,1,0,0,1,0,0,1,0,0
hdim7,1,0,0,1,0,0,1,0,0,0,1,0
*/



//
//  Frequency To Midi Note
//

//TODO: Integrate this into ErisCore
/*
uint8_t ftm(float f,uint8_t * midi_note_history){
  float delta = 10000;
  float delta_test = 10000;
  uint8_t i;
  uint8_t search_dir = 1;
  //frequency to midi note conversion
  //start search at middle c
  delta = f - note_freq[48]; //if pos then note is greater than C
  //Serial.print("inital delta:");
  //Serial.println(delta);
  if (delta < 0) search_dir = -1; // if less than set negative search direction
  delta = abs(delta);
  for(i = 48 + search_dir; i < (127-12) and i > 0; i+= search_dir){
    delta_test = abs(f - note_freq[i]);
    if (delta_test < delta) delta = delta_test;
    else{
      //last index was closer; revert index and break
      i-= search_dir;
      break;
    }
  }

  //update the midi note history
  for (uint8_t k = 0; k < 2; k++){
    for (uint8_t j = MIDI_NOTE_HISTORY_LEN - 1; j > 0; j--){
      midi_note_history[j] = midi_note_history[j-1];
    }
  }
  midi_note_history[0] = i;
  
  return i; //midi note lookup table starts at note 12
}

float ou(uint8_t index){return octave_up[index];} 
float od(uint8_t index){return octave_down[index];}
const char * nn(uint8_t index){
  if (index > 88) return "NONE";
  return note_name[index];
}
float nf(uint8_t index){return note_freq[index];}
*/

