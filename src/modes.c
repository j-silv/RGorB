#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ws2812b.h"
#include "bno055.h"
#include "modes.h"


int sinc_filter_previous_values[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0,
                                     0};


int moving_average_filter_previous_values[] = {0, 0, 0, 0, 0, 0, 0, 0,
		                                       0, 0, 0, 0, 0, 0, 0, 0};

int rc_filter_previous_values[] = {0, 0, 0, 0, 0, 0, 0};

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
uint8_t map(float val, float input_min, float input_max, float output_min) {
  float scaled_val = 0;
  float output_max = 0xFF;
  scaled_val = clamp(val, input_min, input_max);
  scaled_val = (((scaled_val - input_min)/(input_max - input_min))*(output_max-output_min)) + output_min;
  return scaled_val;
}

//===========================================
// lowpass_average()
//   16 tap moving average filter
//===========================================
uint8_t lowpass_average(uint8_t value) {
    const int filter_coeffients[] = {16, 16, 16, 16, 16, 16, 16, 16,
    								 16, 16, 16, 16, 16, 16, 16, 16};
    int filtered_value = 0;
    uint8_t prev;
	
    /*
     *    :
     *    previous_values[6] = previous_values[5];
     *    previous_values[5] = previous_values[4];
     *    :
     *    :
     *    previous_values[1] = previous_values[0];
     */
    for(prev = 15; prev > 0; prev--)
    {
    	moving_average_filter_previous_values[prev] = moving_average_filter_previous_values[prev-1];
    	filtered_value += moving_average_filter_previous_values[prev-1] * filter_coeffients[prev-1];
    }
    moving_average_filter_previous_values[0] = value;
    filtered_value += moving_average_filter_previous_values[0] * filter_coeffients[0];

    return (filtered_value >> 8); // shift back
}

uint8_t lowpass_rc(uint8_t value) {
    /*
     *  filter coeffiecients come from:
     *    n = 0:1:6;
     *    bk = (1-exp(-n))*0.185;
     *    a = round(bk .* 2^8);
     *    b = diff(a);
     *    c = round(b/1.63);
     */

    const int filter_coeffients[] = {99, 37, 13, 5, 2, 1, 0};

    int filtered_value = 0;
    uint8_t prev;

    /*
     *    :
     *    previous_values[6] = previous_values[5];
     *    previous_values[5] = previous_values[4];
     *    :
     *    :
     *    previous_values[1] = previous_values[0];
     */
    for(prev = 6; prev > 0; prev--)
    {
    	rc_filter_previous_values[prev] = rc_filter_previous_values[prev-1];
    	filtered_value += rc_filter_previous_values[prev-1] * filter_coeffients[prev-1];
    }
    rc_filter_previous_values[0] = value;
    filtered_value += rc_filter_previous_values[0] * filter_coeffients[0];

    // returning uint8_t, so zero out if would be less than 0x00 after shifting
    if (filtered_value < 0xFF) {
    	filtered_value = 0x00;
    // or max out if would be more than 0xFF after shifting
    } else if (filtered_value > 0x10000) {
    	filtered_value = 0xFF00;
    }

    return (filtered_value >> 8); // shift back
}

uint8_t lowpass_sinc(uint8_t value) {
    /*
     *  filter coeffiecients come from:
     *      n = -17:1:17;
     *      bk = 0.8*sinc(0.8*n);  // lowpass in time domain
     *      hk = hanning(17)';      // windowing function
     *
     *      filter_coeffients = bk .* hk * 2^8
     */

    const int filter_coeffients[] = {0, -1, 2, 0, -7, 19, -34, 46,
    								 206,
                                     46, -34, 19, -7, 0, 2, -1, 0};

    int filtered_value = 0;
    uint8_t prev;

    /*
     *    :
     *    previous_values[6] = previous_values[5];
     *    previous_values[5] = previous_values[4];
     *    :
     *    :
     *    previous_values[1] = previous_values[0];
     */
    for(prev = 16; prev > 0; prev--)
    {
    	sinc_filter_previous_values[prev] = sinc_filter_previous_values[prev-1];
    	filtered_value += sinc_filter_previous_values[prev-1] * filter_coeffients[prev-1];
    }
    sinc_filter_previous_values[0] = value;
    filtered_value += sinc_filter_previous_values[0] * filter_coeffients[0];

    // returning uint8_t, so zero out if would be less than 0x00 after shifting
    if (filtered_value < 0xFF) {
    	filtered_value = 0x00;
    // or max out if would be more than 0xFF after shifting
    } else if (filtered_value > 0x10000) {
    	filtered_value = 0xFF00;
    }

    return (filtered_value >> 8); // shift back
}

void clear_prev_value(void)
{
    uint8_t i;
    for(i = 0; i < 17; i++)
    {
    	sinc_filter_previous_values[i] = 0;
    }

    for(i = 0; i < 7; i++)
    {
    	rc_filter_previous_values[i] = 0;
    }

    for(i = 0; i < 16; i++)
    {
    	moving_average_filter_previous_values[i] = 0;
    }
}

//===========================================
// mode_1()
//   impulse mode that lights pixels based on
//   the magnitude of the linear acceleration
//===========================================
void mode_1(pixel_typedef *pixels, uint16_t num_pixels){
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
// mode_2()
//   mode that lights pixels based on
//   the acceleration vector
//===========================================
void mode_2(pixel_typedef *pixels, uint16_t num_pixels){
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
void mode_3(pixel_typedef *pixels, uint16_t num_pixels){
	enum HSV_STATE {
	  RED,
	  YELLOW,
	  GREEN,
	  CYAN,
	  BLUE,
	  MAGENTA,
	};

    static uint8_t r_val = 0xFF;
    static uint8_t g_val = 0x00;
    static uint8_t b_val = 0x00;

    static enum HSV_STATE state = RED;

    switch (state) {
    	case RED:
    	    // output value logic
    		g_val++;

    	    // state transition logic
    		if (g_val == 0xFF) {
    			state = YELLOW;
    		}
    		break;
    	case YELLOW:
    	    // output value logic
    		r_val--;

    	    // state transition logic
    		if (r_val == 0x00) {
    			state = GREEN;
    		}
    		break;
    	case GREEN:
    	    // output value logic
    		b_val++;

    	    // state transition logic
    		if (b_val == 0xFF) {
    			state = CYAN;
    		}
    		break;
    	case CYAN:
    	    // output value logic
    		g_val--;

    	    // state transition logic
    		if (g_val == 0x00) {
    			state = BLUE;
    		}
    		break;
    	case BLUE:
    	    // output value logic
    		r_val++;

    	    // state transition logic
    		if (r_val == 0xFF) {
    			state = MAGENTA;
    		}
    		break;
    	case MAGENTA:
    	    // output value logic
    		b_val--;

    	    // state transition logic
    		if (b_val == 0x00) {
    			state = RED;
    		}
    		break;
    }

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}



//===========================================
// mode_4()
//   true North mode
//     lights up golden when north is faced
//===========================================
void mode_4(pixel_typedef *pixels, uint16_t num_pixels) {
    uint8_t r_val = 0x00;
    uint8_t g_val = 0x00;
    uint8_t b_val = 0x00;

    uint16_t is_golden;

    double x_angle;
    double y_angle;
    double z_angle;

    double range = 5.0;  // have to be within +- 5 degrees

    x_angle = convert_euler(get_euler_x_data());
    y_angle = convert_euler(get_euler_y_data());
    z_angle = convert_euler(get_euler_z_data());


    if      ((x_angle < range) || (x_angle > 360-range)) {
       is_golden = 1;
    }
    else if ((y_angle < range) || (y_angle > 360-range)) {
       is_golden = 1;
    }
    else if ((z_angle < range) || (z_angle > 360-range)) {
       is_golden = 1;
    }
    else {
        is_golden = 0;
    }

    if (is_golden) {
    	r_val = 0xFF;
    	g_val = 0xFF;
    	b_val = 0x00;
    } else {
    	r_val = 0x00;
    	g_val = 0x00;
    	b_val = 0x00;
    }

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}


//===========================================
// mode_5()
//   position mode
//     lights up based on heading
//===========================================
void mode_5(pixel_typedef *pixels, uint16_t num_pixels){
    uint8_t r_val = map(convert_euler(get_euler_x_data()), 0, 360, 0);
    uint8_t g_val = map(convert_euler(get_euler_y_data()), 0, 360, 0);
    uint8_t b_val = map(convert_euler(get_euler_z_data()), 0, 360, 0);

    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   r_val;
        pixels[i].green = g_val;
        pixels[i].blue =  b_val;
    }
}


//===========================================
// mode_6()
//   linear acceration magitiude mode
//     lights up white based on linear 
//     acceration mag
//===========================================
void mode_6(pixel_typedef *pixels, uint16_t num_pixels){
    uint8_t r_val = map(convert_lin(get_linear_acc_x_data()), -10, 10, 0);
    uint8_t g_val = map(convert_lin(get_linear_acc_y_data()), -10, 10, 0);
    uint8_t b_val = map(convert_lin(get_linear_acc_z_data()), -10, 10, 0);

    uint8_t value = cutoff(map(r_val+g_val+b_val, 0, 255*3, 0), 8);
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   value;
        pixels[i].green = value;
        pixels[i].blue =  value;
    }
}

//===========================================
// mode_7()
//   spin city mode
//     lights up white based on spin power
//===========================================
void mode_6(pixel_typedef *pixels, uint16_t num_pixels){
    uint8_t r_val = map(convert_gyro(get_gyro_x_data()), -1080, 1080, 0);
    uint8_t g_val = map(convert_gyro(get_gyro_y_data()), -1080, 1080, 0);
    uint8_t b_val = map(convert_gyro(get_gyro_z_data()), -1080, 1080, 0);

    uint8_t value = cutoff(map(r_val+g_val+b_val, 0, 255*3, 0), 8);
    for(int i = 0; i<num_pixels; i++) {
        pixels[i].red =   value;
        pixels[i].green = value;
        pixels[i].blue =  value;
    }
}
