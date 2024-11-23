#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ws2812b.h"
#include "bno055.h"


//===========================================
// clamp()
//   takes a value and performs the following
//   function on it:
//     output = { min,   val < min
//              { max,   val > max
//              { val,   else
//===========================================
float clamp(float val, float min, float max){
  if(val > max) {
    return max;
  }
  else if (val < min) {
    return min;
  }
  return val;
}

//===========================================
// cutoff()
//   takes a value and performs the following
//   function on it:
//     output = { 0,     val < min
//              { val,   else
//===========================================
float cutoff(float val, float min){
  if (val < min) {
    return 0;
  }
  return val;
}

//===========================================
// map()
//   takes a value and does a linear scaling
//   that scales the fullscale range of the
//   input to the fullscale range of the
//   output
//-------------------------------------------
//   scaled_val - input_min
//   ----------------------  * (output_max - output_min) + output_min
//    input_max - input_min
//===========================================
uint8_t map(float val, float input_min, float input_max, float output_min, float output_max=0xFF) {
  float scaled_val = 0;
  scaled_val = clamp(val, input_min, input_max);
  scaled_val = (((scaled_val - input_min)/(input_max - input_min))*(output_max-output_min)) + output_min;
  return scaled_val;
}

//===========================================
// mode_1()
//   impulse mode that lights pixels based on
//   the magnitude of the linear acceleration
//===========================================
mode_1(pixel_typedef *pixels, uint16_t num_pixels){
    uint8_t r_val = map(abs(cutoff(convert_lin(get_linear_acc_x_data()), 0x8)), 0, 20, 0);
    uint8_t g_val = map(abs(cutoff(convert_lin(get_linear_acc_y_data()), 0x8)), 0, 20, 0);
    uint8_t b_val = map(abs(cutoff(convert_lin(get_linear_acc_x_data()), 0x8)), 0, 20, 0);

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}

//===========================================
// mode_1()
//   impulse mode that lights pixels based on
//   the acceleration vector
//===========================================
mode_2(pixel_typedef *pixels, uint16_t num_pixels){
    uint8_t r_val = map(convert_acc(get_acc_x_data()), -10, 10, 0);
    uint8_t g_val = map(convert_acc(get_acc_y_data()), -10, 10, 0);
    uint8_t b_val = map(convert_acc(get_acc_z_data()), -10, 10, 0);

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}

//===========================================
// mode_3()
//   7-State RGB cycle (HSV)
//     Red1 -> Yellow -> Green -> Cyan -> Blue
//     - > Magenta -> Red2
//===========================================
mode_3(pixel_typedef *pixels, uint16_t num_pixels){
    enum RBG_State {
      RED,
      YELLOW,
      GREEN,
      CYAN,
      BLUE,
      MAGENTA,
    };

    static uint8_t r_val = 0xFF;
    static uint8_t g_val = 0;
    static uint8_t b_val = 0;

    static enum HSV_STATE myVar = RED;

    switch (HSV_STATE) {
    	case RED:
    	    // output value logic
    		g_val++;

    	    // state transition logic
    		if (g_val == 0xFF) {
    			HSV_STATE = YELLOW;
    		}
    		break;
    	case YELLOW:
    	    // output value logic
    		r_val--;

    	    // state transition logic
    		if (r_val == 0x00) {
    			HSV_STATE = GREEN;
    		}
    		break;
    	case GREEN:
    	    // output value logic
    		b_val++;

    	    // state transition logic
    		if (b_val == 0xFF) {
    			HSV_STATE = CYAN;
    		}
    		break;
    	case CYAN:
    	    // output value logic
    		g_val--;

    	    // state transition logic
    		if (g_val == 0x00) {
    			HSV_STATE = BLUE;
    		}
    		break;
    	case BLUE:
    	    // output value logic
    		r_val++;

    	    // state transition logic
    		if (r_val == 0xFF) {
    			HSV_STATE = MAGENTA;
    		}
    		break;
    	case MAGENTA:
    	    // output value logic
    		b_val--;

    	    // state transition logic
    		if (b_val == 0x00) {
    			HSV_STATE = RED;
    		}
    		break;
    }

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}
