#include "main.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks from SysTickHandler */  

void SysTick_Handler(void) {
  msTicks++;
}

void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

int main (void) {
  SysTick_Config(SystemCoreClock / 1000); // SysTick 1 msec interrupts

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  // set as GPIO output
  while(1){
    GPIOA->ODR |= GPIO_ODR_OD5;
    Delay(1000);
    GPIOA->ODR &= ~GPIO_ODR_OD5;
    Delay(1000);
  }
}
