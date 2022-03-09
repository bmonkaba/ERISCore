/**
 * @file AudioUtilities.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __AudioUtils_
#define __AudioUtils_  



//function prototypes

//Frequency to Midi Note
uint8_t ftm(float f,uint8_t * midi_note_history);
float ou(uint8_t index); 
float od(uint8_t index);
const char * nn(uint8_t index);
float nf(uint8_t index);
#endif
