#ifndef __eris_waveshapes_h
#define __eris_waveshapes_h
#include <Arduino.h>
//513 x 18 compression tables + reference
#define COMP_WAVETABLE_SIZE 18
extern const float** comp_wavetable;

extern const int16_t** wt;
#endif
