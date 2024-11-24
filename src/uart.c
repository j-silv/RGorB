#include "stm32f446xx.h"
#include "uart.h"

extern uint32_t SystemCoreClock;

void UART_WRITE(uint8_t* data, uint8_t size){
    for(int i = 0; i < size; i++) {
        USART2->DR = data[i];
        while ((USART2->SR & USART_SR_TXE) == 0);
    }
}
  
void UART_INIT(void) {

    // set up GPIO (pins ). on nucleo board, PA2 == USART2_TX, PA3 == USART2_RX
    // enable clock that goes to GPIOA pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // set as alternate function
    GPIOA->MODER |= GPIO_MODER_MODER2_1;  
    GPIOA->MODER |= GPIO_MODER_MODER3_1;

    // set as USART alternate function (we are using AFRL because pins 0-7)
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2 & (7U << GPIO_AFRL_AFSEL2_Pos);
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3 & (7U << GPIO_AFRL_AFSEL3_Pos);

    // GPIOA->OTYPER |= GPIO_OTYPER_OT2;
    // GPIOA->OTYPER |= GPIO_OTYPER_OT3;

    // enable the clock that goes to the periphal
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Delay after an RCC peripheral clock enabling
    // errata 2.2.7 -> need to wait after peripheral clock enabling
    // workaround dummy read operation
    (void)(volatile uint8_t)RCC->APB1ENR; 

    USART2->CR1 = 0;                           // Disable this UART

    uint32_t tmpreg = 0x00, apbclock = 0x00;
    uint32_t integerdivider = 0x00;
    uint32_t fractionaldivider = 0x00;

    apbclock = SystemCoreClock/4;
    integerdivider = ((25 * apbclock) / (4 * (115200)));    
    tmpreg = (integerdivider / 100) << 4;
    fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    USART2->BRR = (uint16_t)tmpreg;


    USART2->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;  // Set UE, RE, TE
}