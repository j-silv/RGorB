#include "stm32f4xx.h"
  
void i2c(void) {

  // #######################################################################

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


  // #########################################################

  // first enable the clock that goes to the periphal
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;


  // Delay after an RCC peripheral clock enabling
  // errata 2.2.7 -> need to wait after peripheral clock enabling
  // workaround dummy read operation
  volatile uint32_t dummy = RCC->APB1ENR;


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


  // ########################################################

  // send start bit (condition)
  I2C1->CR1 |= I2C_CR1_START;

  // read status register to check for start bit being send
  while((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB);

  // write to DR register with MCP4725 address
  // and ask for read access (LSB)
  I2C1->DR |= 0b11000001; 

  // read status register to check for ADDR being send
  while((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR);

  // read status register to check for ACK was sent
  while((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF);

  // turn on acknowledges
  I2C1->CR1 |= I2C_CR1_ACK;

  // clear ADDR to enter master receiver mode
  I2C1->SR1 &= ~I2C_SR1_ADDR;

  volatile uint8_t data[5];

  // poll RxNE (not empty) until we have a byte available
  for(int i = 0; i < 5; i++) {

    // wait for byte to arrive
    while((I2C1->SR1 & I2C_SR1_RXNE) == 1);

    // byte arrived - clear DR
    data[i] = I2C1->DR & I2C_DR_DR;

    // generate nonacknowledge pulse before last received byte
    // also generate stop condition
    if (i == 3) {
      I2C1->CR1 &= ~I2C_CR1_ACK;
      I2C1->SR1 |= I2C_CR1_STOP;
    }
  }

  // #########################################################

  // see if it worked
  //Delay(1);


}