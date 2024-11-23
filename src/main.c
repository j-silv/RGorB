#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bno055.h"
#include "i2c.h"
#include "modes.h"
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


  while(1){
      mode_2(pixel, NUM_PIXELS);
      write_ws2812b(pixel, NUM_PIXELS);
  }
}
