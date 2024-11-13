#ifndef WS2812B_H_
#define WS2812B_H_

#include <stdint.h>

// this will use timers on STM32 to generate an arbitrary waveform
// specifically, a waveform that matches the WS2812B datasheet
// for outputting pixel values

void init_ws2812b();


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_typedef;

void write_ws2812b(pixel_typedef *pixel, uint8_t num_pixels);


#endif