#ifndef INC_MODES_H_
#define INC_MODES_H_

//===========================================
// clamp()
//   takes a value and performs the following
//   function on it:
//     output = { min,   val < min
//              { max,   val > max
//              { val,   else
//===========================================
float clamp(float val, float min, float max);

//===========================================
// cutoff()
//   takes a value and performs the following
//   function on it:
//     output = { 0,     val < min
//              { val,   else
//===========================================
float cutoff(float val, float min);

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
uint8_t map(float val, float input_min, float input_max, float output_min, float output_max=0xFF);

#endif /* INC_MODES_H_ */
