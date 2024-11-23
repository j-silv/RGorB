#ifndef WS2812B_H_
#define WS2812B_H_

#include <stdint.h>

// 1 -> high pulse: 0.8 us
//   -> low pulse: 0.45 us
// 0 -> high pulse: 0.4 us
//   -> low pulse: 0.85 us
#define PERIOD 180*1.25
#define LOW_DUTY_CYCLE 180*0.45
#define HIGH_DUTY_CYCLE 180*0.85       
#define RESET_PULSE 180*350  

void init_ws2812b();

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_typedef;

void write_ws2812b(pixel_typedef *pixel, uint16_t num_pixels);


#endif