#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ws2812b.h"
#include "bno055.h"



float clamp(float min, float max, float val){
  if(val > max) {
    return max;
  }
  else if (val < min) {
    return min;
  }
  else {
    return val;
  }
}

float cutoff(float min, float val){
  if (val < min) {
    return 0;
  }
  return val;
}

uint8_t map(float min, float max, float offset, float val) {
  float temp = 0; 
  temp = clamp(min, max, val);
  temp = (((temp - min)/(max - min))*(0xFF-offset)) + offset;
  return temp;
}

mode_1(pixel_typedef *pixels, uint16_t num_pixels){
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   map(0, 20, 0, abs(cutoff(0x8, convert_lin(get_linear_acc_x_data()))));
        pixels[i].green = map(0, 20, 0, abs(cutoff(0x8, convert_lin(get_linear_acc_y_data()))));
        pixels[i].blue =  map(0, 20, 0, abs(cutoff(0x8, convert_lin(get_linear_acc_z_data()))));
    }
}

mode_2(pixel_typedef *pixels, uint16_t num_pixels){
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   map(-10, 10, 0, convert_acc(get_acc_x_data()));
        pixels[i].green = map(-10, 10, 0, convert_acc(get_acc_y_data()));
        pixels[i].blue =  map(-10, 10, 0, convert_acc(get_acc_z_data()));
    }
}

mode_3(pixel_typedef *pixels, uint16_t num_pixels){

    static uint8_t r_val = 0;
    static uint8_t g_val = 0;
    static uint8_t b_val = 0;

    uint8_t inc = 4;

    if (r_val < 0xFF-inc) {
        r_val+=inc;
    } else if (g_val < 0xFF-inc){
        g_val+=inc;
    } else if (b_val < 0xFF-inc){
        b_val+=inc;
    } else {
        r_val = 0;
        g_val = 0;
        b_val = 0;
    }
    
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}

mode_4(pixel_typedef *pixels, uint16_t num_pixels){

    static uint8_t r_val = 0;
    static uint8_t g_val = 0;
    static uint8_t b_val = 255;

    static uint16_t state = 0;

    uint8_t inc = 1;

    if (state == 0) {
        r_val+=inc;
        b_val-=inc;
    } else if (state == 1){
        g_val+=inc;
        r_val-=inc;
    } else {
        b_val+=inc;
        g_val-=inc;
    }

    if (b_val==255){
        state = 0;
    } else if (r_val==255){
        state = 1;
    } else if (g_val==255) {
        state = 2;
    }
    
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}
