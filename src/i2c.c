#include "stm32f446xx.h"
#include "i2c.h"

void I2C_READ(uint8_t dev_id, uint8_t addr, uint8_t* data) {

  I2C1->CR1 |= I2C_CR1_START;                           // S
  while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);        // EV5
  I2C1->DR = I2C_DR_DR & (dev_id & ~((uint8_t)1U));     // Address
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A
  while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);    // EV6 
  (void)(volatile uint8_t)I2C1->SR2;                    // EV6

  // #######################################################################

  I2C1->DR = I2C_DR_DR & addr;                          // EV8_1, EV8, Data1
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A

  // #######################################################################

  I2C1->CR1 |= I2C_CR1_START;                           // Sr  
  while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);        // EV5
  (void)(volatile uint8_t)I2C1->SR1;                    // EV5
  I2C1->DR = I2C_DR_DR & (dev_id | ((uint8_t)1U));      // EV5/Address
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A
  while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);    // EV6 
  I2C1->CR1 &= ~I2C_CR1_ACK;                            // NA
  (void)(volatile uint8_t)I2C1->SR1;                    // EV6
  (void)(volatile uint8_t)I2C1->SR2;                    // EV6
  I2C1->CR1 |= I2C_CR1_STOP;                            // P

  // #######################################################################

  while((I2C1->SR1 & I2C_SR1_RXNE) != I2C_SR1_RXNE);    // EV7
  *data = I2C1->DR & I2C_DR_DR;                         // EV7
  
}


void I2C_WRITE(uint8_t dev_id, uint8_t addr, uint8_t* data){

  I2C1->CR1 |= I2C_CR1_START;                           // S
  while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);        // EV5
  I2C1->DR = I2C_DR_DR & (dev_id & ~((uint8_t)1U));     // Address
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A
  while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);    // EV6 
  (void)(volatile uint8_t)I2C1->SR2;                    // EV6

  // #######################################################################
  while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);      // EV8_1 
  I2C1->DR = I2C_DR_DR & addr;                          // EV8_1, EV8, Data1
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A

  // #######################################################################

  while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);      // EV8
  I2C1->DR = I2C_DR_DR & (*data);                       // EV8_1, EV8, Data2
  while((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);      // EV8_1 
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);        // A

  // #######################################################################

  I2C1->CR1 |= I2C_CR1_STOP;                            // P

  // #######################################################################

}
  
void I2C_INIT(void) {

  // set up GPIO (pins ). on nucleo board, PB8 == I2C1_SCL, PB9 == I2C1_SDA
  // enable clock that goes to GPIOB pins
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  // set as alternate function
  // default speed is fine for our slow I2C bus purposes (OSPEEDR)
  GPIOB->MODER |= GPIO_MODER_MODER8_1;  
  GPIOB->MODER |= GPIO_MODER_MODER9_1;

  // set as I2C1 alternate function (we are using AFRH because pins 8-15)
  GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8 & (4U << GPIO_AFRH_AFSEL8_Pos);
  GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9 & (4U << GPIO_AFRH_AFSEL9_Pos);

  // set as output open-drain
  GPIOB->OTYPER |= GPIO_OTYPER_OT8;
  GPIOB->OTYPER |= GPIO_OTYPER_OT9;

  // enable pullups
  GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0;
  GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0;

  // first enable the clock that goes to the periphal
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  // Delay after an RCC peripheral clock enabling
  // errata 2.2.7 -> need to wait after peripheral clock enabling
  // workaround dummy read operation
  (void)(volatile uint8_t)RCC->APB1ENR;

  // we have to do all this when I2C itself is disabled
  // write freq bits in I2C clock register with APB1 amount
  // 50 MHz because HCLK is sysclk which is 100 MHz i think
  I2C1->CR2 |= (0b110010 & I2C_CR2_FREQ); 
  
  // set up SCL frequency to be 100 kHz. TPCLK1 is 50 MHz
  // Thigh == Tlow == CCR*TPCLK1
  // 1 / (2*0.1 MHz) == CCR * 1/50 MHz
  I2C1->CCR |= (250U & I2C_CCR_CCR_Msk);

  // set to max rise time allowable by I2C spec
  I2C1->TRISE |= (51U & I2C_TRISE_TRISE);

  // enable the I2C1 perihperal 
  I2C1->CR1 |= I2C_CR1_PE;

}