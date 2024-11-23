#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "bno055.h"
#include "i2c.h"
#include "ws2812b.h"
#include "clock.h"
#include "uart.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks from SysTickHandler */  
#define NUM_PIXELS 6
#define MIN_ACC -10.0
#define MAX_ACC 10.0
#define MAX_LED_VAL 255.0
#define OFFSET 0.0

void SysTick_Handler(void) {
  msTicks++;
}

void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

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

uint8_t map(float min, float max, float offset, float val) {
  float temp = 0; 
  temp = clamp(min, max, val);
  temp = (((temp - min)/(max - min))*(MAX_LED_VAL-offset)) + offset;
  return temp;
}

int main (void) {
  SystemCoreClockConfigure();  /* configure HSI as System Clock */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); /* SysTick 1 msec interrupts */ 

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  // set as GPIO output

  UART_INIT(); 
  init_ws2812b();
  I2C_INIT();


  pixel_typedef pixel[NUM_PIXELS];

  for(int i = 0; i<NUM_PIXELS; i++) {
    uint8_t val = 0;
    if (i == 4) {
      val = 125;
    }
    pixel[i].red = 0;
    pixel[i].green = 0;
    pixel[i].blue = val;
  }
  write_ws2812b(pixel, NUM_PIXELS);

  //##############################
  // needed to reset BNO
  GPIOA->MODER |= GPIO_MODER_MODER6_0;
  GPIOA->ODR &= ~GPIO_ODR_OD6;
  Delay(500);
  GPIOA->ODR |= GPIO_ODR_OD6;
  Delay(500);
  //##############################

  uint8_t return_code = init_bno055();
  if (return_code == NO_SYSTEM_ERROR) {
    GPIOA->ODR |= GPIO_ODR_OD5; // turn on GPIOA_P5 (LED)
  }

  for(int i = 0; i<NUM_PIXELS; i++) {
    uint8_t val = 0;
    if (i == 3) {
      val = 125;
    }
    pixel[i].red = 0;
    pixel[i].green = val;
    pixel[i].blue = val;
  }
  write_ws2812b(pixel, NUM_PIXELS);

  int temp =  0x80;
  double acc = 0;
  uint8_t map_acc = 0;
  float clamp_acc = 0;
  while(1){
      temp = convert_temp(get_temp());
      acc = convert_acc(get_acc_x_data());
      map_acc = map(MIN_ACC, MAX_ACC, OFFSET, acc);
      clamp_acc = clamp(MIN_ACC, MAX_ACC, acc);

      printf("temp: %d, acc: %.5f clamp(acc): %.5f map(acc): %d\n", temp, acc, clamp_acc, map_acc);
      // Delay(100);
      for(int i = 0; i<NUM_PIXELS; i++) {
        // pixel[i].red = map_acc;
        // pixel[i].green = map_acc;
        // pixel[i].blue = map_acc;
        pixel[i].red = map(MIN_ACC, MAX_ACC, OFFSET, convert_acc(get_acc_x_data()));
        pixel[i].green = map(MIN_ACC, MAX_ACC, OFFSET, convert_acc(get_acc_y_data()));
        pixel[i].blue = map(MIN_ACC, MAX_ACC, OFFSET, convert_acc(get_acc_z_data()));
      }
      write_ws2812b(pixel, NUM_PIXELS);
  }
}





  // double acc;

  // uint8_t scaled_acc;

  // while(1) {
  //   acc = convert_acc(get_acc_x_data());
  //   scaled_acc = 10*(acc + 20);
  //   pixel[3].blue = scaled_acc;
  //   write_ws2812b(pixel, NUM_PIXELS);
  // }





