#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bno055.h"
#include "i2c.h"
#include "modes.h"
#include "ws2812b.h"
#include "uart.h"

#define NUM_PIXELS 6
#define MIN_ACC -10.0
#define MAX_ACC 10.0
#define MAX_LED_VAL 255.0
#define OFFSET 0.0

#endif