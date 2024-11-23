#include "ws2812b.h"
#include "stm32f4xx.h"
#include <math.h>
#include <stdint.h>

void init_ws2812b() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // enable clock for GPIOA
    GPIOA->MODER |= GPIO_MODER_MODER8_1;    // set as alt func (pin PA8 is connected to TIM1_CH1)

    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL8 & (1U << GPIO_AFRH_AFSEL8_Pos); // set as TIM1 (we are using AFRH because pins 8-15)     

    GPIOB->OTYPER |= GPIO_OTYPER_OT8;       // set as push pull
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;     // enable the clock that goes to the periphal
    (void)(volatile uint8_t)RCC->APB2ENR;   // errata 2.2.7 -> need to wait after peripheral clock enabling

    TIM1->ARR = RESET_PULSE;   // we will stay low until sending data out
    TIM1->CCR1 = 0;            // OC1REF = TIM1_CNT < TIM1_CCR1 ? 1 : 0

    TIM1->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // SET PWM1 mode (upcounting)
    TIM1->BDTR |= TIM_BDTR_MOE;                        // Enable the TIM1 Main Output
    TIM1->CCER |= TIM_CCER_CC1E;                       // Enable CC1 output (TIM1_CH1)

    TIM1->CCMR1 |= TIM_CCMR1_OC1PE; /* Enable the output compare 1 Preload */
    TIM1->CR1 |= TIM_CR1_ARPE;      /* Enable auto-reload Preload */

}

// TIM1->CCMR1 |= TIM_CCMR1_OC1PE; /* Enable the output compare 1 Preload */
// TIM1->CR1 |= TIM_CR1_ARPE; /* Enable auto-reload Preload */
// TIM1->EGR |= TIM_EGR_UG;  /* Enable UEV by setting UG bit to Load buffer data into preload registers */
// while((TIM1->EGR & TIM_EGR_UG) == SET); /* wait until the RESET of UG bit*/

void output_color(uint8_t color) {
    for(int j = 7; j >= 0; j--) {  
        if(color & (0x01 << j)) {
            TIM1->CCR1 = HIGH_DUTY_CYCLE;
        }   
        else {
            TIM1->CCR1 = LOW_DUTY_CYCLE;
        }
        while(!(TIM1->SR & TIM_SR_UIF)); // Loop until the update event flag is set
        TIM1->SR = 0;                    // clear update event flag
    }
}

void write_ws2812b(pixel_typedef *pixels, uint16_t num_pixels){

    TIM1->ARR = PERIOD;
    TIM1->CCR1 = 0;
    TIM1->CR1 |= TIM_CR1_CEN;        // Start the timer counter

    for(int i = 0; i < num_pixels; i++) {
        output_color(pixels[i].green);
        output_color(pixels[i].red);
        output_color(pixels[i].blue);
    }

    TIM1->CCR1 = 0;          // 0% duty cycle to get active low
    TIM1->ARR = RESET_PULSE; // now send 50 us low pulse
    while(!(TIM1->SR & TIM_SR_UIF)); // Loop 1 until the update event flag is set */
    TIM1->SR = 0;                    // clear update event flag1


    TIM1->CR1 &= ~TIM_CR1_CEN;       // stop the timer counter */
}