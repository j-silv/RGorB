/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2016 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include <stdint.h>
#include "bno055.h"
#include "i2c.h"

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
  SysTick_Config(SystemCoreClock / 1000); /* SysTick 1 msec interrupts */                  

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA
  GPIOA->MODER |= GPIO_MODER_MODER5_0;  // set as GPIO output

  // //##############################
  // // needed to reset BNO
  // GPIOA->MODER |= GPIO_MODER_MODER6_0;
  // GPIOA->ODR &= ~GPIO_ODR_OD6;
  // Delay(500);
  // GPIOA->ODR |= GPIO_ODR_OD6;
  // Delay(500);
  // //##############################

  // I2C_INIT();
  // uint8_t return_code = init_bno055();
  // if (return_code == NO_SYSTEM_ERROR) {
  //   GPIOA->ODR |= GPIO_ODR_OD5; // turn on GPIOA_P5 (LED)
  // }
  
  // int temp =  0;
  // double acc = 0;
  // while(1){
  //     temp = convert_temp(get_temp());
  //     acc = convert_acc(get_acc_x_data());
  // }


  while(1) {
    GPIOA->ODR |= GPIO_ODR_OD5; // turn on GPIOA_P5 (LED)
    Delay(1000);
    GPIOA->ODR &= ~GPIO_ODR_OD5; // turn on GPIOA_P5 (LED)
    Delay(1000);
  }
}


