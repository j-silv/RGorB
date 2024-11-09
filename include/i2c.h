/*
 * i2c.h
 *
 *  Created on: Sep 28, 2024
 *      Author: dbac
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f4xx_hal.h"

void MX_I2C1_Init(void);

void I2C_READ(uint8_t dev_id, uint8_t addr, uint8_t* data);
void I2C_WRITE(uint8_t dev_id, uint8_t addr, uint8_t* data);

#endif /* INC_I2C_H_ */
