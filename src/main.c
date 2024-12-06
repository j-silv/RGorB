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

  gpio_struct led_gpio = gpio_init(GPIO_A, 5, GPIO_OUT);

  while(1){
    gpio_set(&led_gpio);
    Delay(1000);
    gpio_reset(&led_gpio);
    Delay(1000);
  }
}






