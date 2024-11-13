#include "ws2812b.h"
#include "stm32f4xx.h"
#include <math.h>
#include <stdint.h>

// From reference manual:

// A timer channel can generate a PWM signal with a constant predefined frequency and dutycycle parameters.
// The signal frequency parameter is controlled by the content of the timer
// TIMx_ARR auto-reload register. In this register, the duty-cycle parameter is controlled by the
// content of the timer channel 1 register (TIM1_CCR1). In order to make the timer channel
// output the requested waveform (refer to Figure 4), the content of these two timer registers
// (TIMx_ARR and TIMx_CCR1) at the end of each PWM cycle must be updated.

// To prevent the timer channel from generating unwanted glitches due to updating the timer
// channel register, the preload feature of the STM32 timer peripherals is used.

// To make the timer channel to output repetitive pulses with the same t_on and t_off
// parameters and without the timer generating an “update event” on each PWM cycle, the
// timer repetition counter should be used.
// - TIM1_ARR: contains the sum of t_on and t_off periods for the on-going pulse.
// - TM1_RCR: contains the number of pulses per the on-going signal waveform portion
//   minus one (i.e.TIM1_RCR=Number_of_pulses_per_portion–1).
// - TIM1_CCR1: contains the duration of the t_on period of the on-going pulse.

// Forced output mode
// In output mode (CCxS bits = 00 in the TIMx_CCMRx register), each output compare signal
// (OCxREF and then OCx/OCxN) can be forced to active or inactive level directly by software,
// independently of any comparison between the output compare register and the counter.

// To force an output compare signal (OCXREF/OCx) to its active level, one just needs to write
// 101 in the OCxM bits in the corresponding TIMx_CCMRx register. Thus OCXREF is forced
// high (OCxREF is always active high) and OCx get opposite value to CCxP polarity bit.
// For example: CCxP=0 (OCx active high) => OCx is forced to high level.
// The OCxREF signal can be forced low by writing the OCxM bits to 100 in the TIMx_CCMRx
// register.


// ##################################################################################
// Period calculation:
// I want low pulse of 1 s, and a high pulse of 1 s -> TPulse = ARR - (CCR) +1
// TDelay == CCR
// So really I just need to set ARR
// Period == (1/HCLK)*PSC
// The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1).
// HCLK == 16 MHz
// 1/16 MHz is period, and I want that to count to 1 S
// TIM1 Freq == TIM1_CLK/(ARR + 1)
// If ARR == 0, then there would be an update event every TIM1_CLK pulses because 
// we have a 16 MHz APB2 clock by default (reset), so if we want a 1 second 
// delay then we need to divide this down by a lot
// this would give us a frequency of 244 Hz roughly, still too quick to see i think
// Ehhhh I'll do the math later
// fCK_PSC / (PSC[15:0] + 1)
// ##################################################################################


void init_ws2812b() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock for GPIOA
    GPIOA->MODER |= GPIO_MODER_MODER8_1;  // set as alternate function (pin PA8 is connected to TIM1_CH1)
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL8 & (1U << GPIO_AFRH_AFSEL8_Pos); // set as TIM1 (we are using AFRH because pins 8-15)     
    GPIOB->OTYPER |= GPIO_OTYPER_OT8; // set as push pull
    // TODO -> might want to set pullup if we have the alternate function so that when we're setting up
    //         we don't acidentally toggle the WS2812B chip if we go from low to high or vice versa
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable the clock that goes to the periphal
    volatile uint32_t dummy = RCC->APB2ENR; // errata 2.2.7 -> need to wait after peripheral clock enabling

    TIM1->CCMR1 |= TIM_CCMR1_OC1M & (TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_2); /* SET Forced output High */
    // this is the default state when we are not transmitting anything

        
    TIM1->BDTR |= TIM_BDTR_MOE; /* Enable the TIM1 Main Output */
    TIM1->CCER |= TIM_CCER_CC1E; /* Enable CC1 output*/
}

void write_ws2812b(pixel_typedef *pixel, uint8_t num_pixels){

    // fCK_PSC / (PSC[15:0] + 1)
    // to get the lowest resolution, we set the prescaler to 0 (default)
    // for simplicity, 
    // TIM1->PSC = (1 << 16) - 1; 


    TIM1->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; /* SET PWM2 mode */
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE; /* Enable the output compare 1 Preload */
    TIM1->CR1 |= TIM_CR1_ARPE; /* Enable auto-reload Preload */

    // we already know our timebase for ones and 2s -> it is 1.25 us
    TIM1->ARR = 16*1.25; // 16 MHz is our HSI_VALUE 
    TIM1->CCR1 = TIM1->ARR/2; // 50% duty cycle by default

    TIM1->EGR |= TIM_EGR_UG;  /* Enable UEV by setting UG bit to Load buffer data into preload registers */
    while((TIM1->EGR & TIM_EGR_UG) == SET); /* wait until the RESET of UG bit*/

    // not sure when i need to do this -> maybe need to start the timer but then pause it?
    // start it and then reset it? idk
    TIM1->CR1 |= TIM_CR1_CEN; /* Start the timer counter */
 
    for(int i = 0; i < num_pixels; i++) {
        for(int j = 0; j < sizeof(uint8_t)*8; j++) {  
            if(pixel[i].red & (1 << j)) {
                // 1 -> high pulse: 0.8 us
                //   -> low pulse: 0.45 us
               TIM1->CCR1 = round(16*0.45);
            }   

            else {
                // 0 -> high pulse: 0.4 us
                //   -> low pulse: 0.85 us
                TIM1->CCR1 = round(16*0.85);
            }

            while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
            TIM1->SR = 0; // clear update event flag

        }

        for(int j = 0; j < sizeof(uint8_t)*8; j++) {  
            if(pixel[i].green & (1 << j)) {
                TIM1->CCR1 = round(16*0.45);
            }
            else {
                TIM1->CCR1 = round(16*0.85);
            }

            while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
            TIM1->SR = 0; // clear update event flag
        }

        for(int j = 0; j < sizeof(uint8_t)*8; j++) {  
            if(pixel[i].blue & (1 << j)) {
                TIM1->CCR1 = round(16*0.45);
            }
            else {
                TIM1->CCR1 = round(16*0.85);
            }

            while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
            TIM1->SR = 0; // clear update event flag
        }

    }
    // now send 50 us low pulse (might need to increase this for some margin ?)
    TIM1->ARR = 16*50;
    TIM1->CCR1 = TIM1->ARR + 1; // 0% duty cycle to get active low (i don't know if i need the +1)

    while(!(TIM1->SR & TIM_SR_UIF)); /* Loop until the update event flag is set */
    TIM1->SR = 0; // clear update event flag

    
}