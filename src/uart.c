#include "stm32f446xx.h"
#include "uart.h"
#include "gpio.h"

extern uint32_t SystemCoreClock;

void UART_WRITE(uint8_t* data, uint8_t size){
    for(int i = 0; i < size; i++) {
        USART2->DR = data[i];
        while ((USART2->SR & USART_SR_TXE) == 0);
    }
}
  
void UART_INIT(void) {
    gpio_struct uart_gpio_tx = gpio_init(GPIO_A, 2, GPIO_AF); 
    gpio_struct uart_gpio_rx = gpio_init(GPIO_A, 3, GPIO_AF);

    gpio_af(&uart_gpio_tx, GPIO_USART);
    gpio_af(&uart_gpio_rx, GPIO_USART);


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