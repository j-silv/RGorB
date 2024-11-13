#include "stm32f4xx.h"
#include <stdint.h>
#include "bno055.h"
#include "i2c.h"
#include "ws2812b.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks from SysTickHandler */  
#define NUM_PIXELS 4

void SysTick_Handler(void) {
  msTicks++;
}

void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

int main (void) {
  SysTick_Config(SystemCoreClock / 1000); /* SysTick 1 msec interrupts */                  

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  // set as GPIO output

  init_ws2812b();

  // declare 4 pixels -> testing
  pixel_typedef pixel[NUM_PIXELS] = {0,0,0};
  pixel[0].red = 100;
  pixel[1].green = 100;
  pixel[2].blue = 100;

  write_ws2812b(&pixel, NUM_PIXELS);



  while(1) {
    // GPIOA->ODR |= GPIO_ODR_OD5;
    // while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
    // TIM1->SR = 0; // clear update event flag

    // GPIOA->ODR &= ~GPIO_ODR_OD5; 
    // while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
    // TIM1->SR = 0; // clear update event flag

    write_ws2812b(&pixel, NUM_PIXELS);
    Delay(500);
  }
}


