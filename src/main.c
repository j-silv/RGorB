#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "bno055.h"
#include "i2c.h"
#include "ws2812b.h"
#include "clock.h"
#include "uart.h"

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
  SystemCoreClockConfigure();  /* configure HSI as System Clock */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); /* SysTick 1 msec interrupts */                  

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  // set as GPIO output

  UART_INIT(); 
  uint8_t count = 0;
  // uint8_t newline = (uint8_t)'\n';
  while(1) {
    // UART_WRITE(&count, 1);
    // UART_WRITE(&newline, 1);
    // count++;
    printf("Testing the output! %d\n", count);
    count++;

    Delay(100);
  }

  // init_ws2812b();
  // I2C_INIT();

  // // declare 4 pixels -> testing
  // pixel_typedef pixel[NUM_PIXELS];
  // pixel[0].red = 0x00;
  // pixel[0].green = 0x00;
  // pixel[0].blue = 0x00;

  // pixel[1].red = 0x00;
  // pixel[1].green = 0x00;
  // pixel[1].blue = 0x00;

  // pixel[2].red = 0x00;
  // pixel[2].green = 0x00;
  // pixel[2].blue = 0x00;

  // pixel[3].red = 0x00;
  // pixel[3].green = 0x00;
  // pixel[3].blue = 0x00;

  // init_bno055();

  // // pixel[1].red = 0xFF;
  // // pixel[1].green = 0xFF;
  // // pixel[1].blue = 0xFF;
  // double acc;

  // uint8_t scaled_acc;

  // while(1) {
  //   acc = convert_acc(get_acc_x_data());
  //   scaled_acc = 10*(acc + 20);
  //   pixel[3].blue = scaled_acc;
  //   write_ws2812b(pixel, NUM_PIXELS);
  // }
}





