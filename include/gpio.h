#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f446xx.h"

typedef enum { 
  GPIO_IN   = 0b00,    // input pin
  GPIO_OUT  = 0b01,    // output pin
  GPIO_AF   = 0b10,    // alternate function pin
  GPIO_AN   = 0b11,    // analog mode pin
} gpio_mode_enum;

typedef enum { 
  GPIO_NONE,    // no pullupso
  GPIO_UP,      // enable pullup
  GPIO_DOWN     // enable pulldown
} gpio_pull_enum;

// defined in Figure 19 of STM32F446RE reference manual
typedef enum { 
  GPIO_I2C    = 4U,     // I2C alternate function
  GPIO_TIM1   = 1U,     // TIM1 alternate function
  GPIO_USART  = 7U,     // UART alternate function
} gpio_af_enum;

typedef enum { 
  GPIO_A = 0U,
  GPIO_B = 1U,
  GPIO_C = 2U,
  GPIO_D = 3U,
  GPIO_E = 4U,
  GPIO_F = 5U,
  GPIO_G = 6U,
  GPIO_H = 7U
} gpio_port_enum;


// struct which encapsulates port and pin number
// address of this struct is passed to various GPIO calls
typedef struct {
    gpio_port_enum port;  // GPIO port (A,B,...,H)
    uint8_t pin;          // GPIO pin (0,1,...,15)

    // save the pointer to the GPIO address in this struct
    // once we init -> then for subsequent functions we don't
    // need to keep calculating with the macro 
    // pin we still need though to know
    GPIO_TypeDef* reg;
} gpio_struct;


// set GPIO port and pin number -> returns struct that encapsulates 
gpio_struct gpio_init(gpio_port_enum port, uint8_t pin, gpio_mode_enum mode);

// set GPIO mode (input, output, alternate function)
void gpio_mode(gpio_struct* gpio, gpio_mode_enum mode);

// set open drain if enable == 1, else set push pull (default)
void gpio_opendrain(gpio_struct* gpio, uint8_t enable);

// enable pullup or pulldown resistors depending on pull
void gpio_pull(gpio_struct* gpio, gpio_pull_enum pull);

// set the alternate function depending on af
void gpio_af(gpio_struct* gpio, gpio_af_enum af);

// set gpio to logic level 1 (high)
void gpio_set(gpio_struct* gpio);

// set gpio to logic level 0 (low)
void gpio_reset(gpio_struct* gpio);

#endif