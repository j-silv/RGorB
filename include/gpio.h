#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f446xx.h"

typedef enum { 
  GPIO_IN   = 0x00,    // input pin
  GPIO_OUT  = 0x01,    // output pin
  GPIO_AF   = 0x10,    // alternate function pin
  GPIO_AN   = 0x11,    // analog mode pin
} gpio_mode_enum;

typedef enum { 
  GPIO_NONE,    // no pullupso
  GPIO_UP,      // enable pullup
  GPIO_DOWN     // enable pulldown
} gpio_pull_enum;

typedef enum { 
  GPIO_I2C1,     // I2C alternate function
  GPIO_TIM1,     // TIM alternate function
  GPIO_USART2    // UART alternate function
} gpio_af_enum;

typedef enum { 
  GPIO_A = 0x00,
  GPIO_B = 0x01,
  GPIO_C = 0x02,
  GPIO_D = 0x03,
  GPIO_E = 0x04,
  GPIO_F = 0x05,
  GPIO_G = 0x06,
  GPIO_H = 0x07
} gpio_port_enum;


// struct which encapsulates port and pin number
// address of this struct is passed to various GPIO calls
typedef struct {
    gpio_port_enum port;  // GPIO port (A,B,...,H)
    uint8_t pin;          // GPIO pin (0,1,...,15)
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