#include "ws2812b.h"
#include "stm32f4xx.h"

void init_ws2812b() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock for GPIOA
    GPIOA->MODER |= GPIO_MODER_MODER8_1;  // set as alternate function (pin PA8 is connected to TIM1_CH1)
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL8 & (1U << GPIO_AFRH_AFSEL8_Pos); // set as TIM1 (we are using AFRH because pins 8-15)     
    GPIOB->OTYPER |= GPIO_OTYPER_OT8; // set as push pull
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable the clock that goes to the periphal
    volatile uint32_t dummy = RCC->APB2ENR; // errata 2.2.7 -> need to wait after peripheral clock enabling

    //TIM1->SR = 0; // clear update event flag but i'm pretty sure i don't need this on reset

    // we have a 16 MHz APB2 clock by default (reset), so if we want a 1 second 
    // delay then we need to divide this down by a lot
    TIM1->PSC = (1 << 16) - 1; // this would give us a frequency of 244 Hz roughly, still too quick to see i think
                               // but then we can apply the ARR to something ! to 244 ! 

    TIM1->ARR = 244; // we will count up to this value -> we should get 1 Hz
    TIM1->CR1 |= TIM_CR1_CEN; /* Start the timer counter */

}







// #define ANY_DELAY_RQUIRED 0x0FFF
// /* Hardware-precision delay loop implementation using TIM6 timer
// peripheral. Any other STM32 timer can be used to fulfill this function, but
// TIM6 timer was chosen as it has the less integration level. Other timer
// peripherals may be reserved for more complicated tasks */
// /* Clear the update event flag */
// TIM6->SR = 0
// /* Set the required delay */
// /* The timer presclaer reset value is 0. If a longer delay is required the
// presacler register may be configured to */
// /*TIM6->PSC = 0 */
// TIM6->ARR = ANY_DELAY_RQUIRED
// /* Start the timer counter */
// TIM6->CR1 |= TIM_CR1_CEN
// /* Loop until the update event flag is set */
// while (!(TIM6->SR & TIM_SR_UIF));
// /* The required time delay has been elapsed */
// /* User code can be executed */









// /* The ARR register reset value is 0x0000FFFF for TIM3 timer. So it should
// be ok for this snippet. If you want to change it uncomment the below line */
// /* TIM3->ARR = ANY_VALUE_YOU_WANT */
// /* The TIM3 timer channel 1 after reset is configured as output */
// /* TIM3->CC1S reset value is 0 */
// /* To select PWM2 output mode set the OC1M control bit-field to '111' */
// TIM3->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
// /* Set the duty cycle to 50% */
// TIM3->CCR1 = TIM3->ARR / 2;
// /* By default, after reset, preload for channel 1 is turned off */
// /* To change it uncomment the below line */
// /* TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
// /* Enable the TIM3 channel1 and keep the default configuration (state after
// reset) for channel polarity */
// TIM3->CCER |= TIM_CCER_CC1E;
// /* Start the timer counter */
// TIM3->CR1 |= TIM_CR1_CEN












// /* set the Timer prescaler */
//  Tim1Prescaler= (uint16_t) (SystemCoreClock / 32000000) - 1;
// /* Configure the period */
//  TIM1->ARR = 0xFFFF;
// /* Configure the Timer prescaler */
//  TIM1->PSC = Tim1Prescaler;
// /* Configure pulse width */
//  TIM1->CCR1 = 0xFFF;
// /* Select the ClockDivison to 1 */
// /* Reset clockDivision bit field */
//  TIM1->CR1 &= ~ TIM_CR1_CKD;
// /* Select DIV1 as clock division*/
//  TIM1->CR1 |= TIM_CLOCKDIVISION_DIV1;
// /* Select the Up-counting for TIM1 counter */
// /* Reset mode selection bit fiels*/
//  TIM1->CR1 &= ~( TIM_CR1_DIR | TIM_CR1_CMS);
// /* selct Up-counting mode */
//  TIM1->CR1 |= TIM_COUNTERMODE_UP;
// /* SET PWM1 mode */
// /* Reset the Output Compare Mode Bits */
//  TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
//  TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
// /* Select the output compare mode 1*/
//  TIM1->CCMR1 |= TIM_OCMODE_PWM1;
// /* Enable the output compare 1 Preload */
//  TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
// /* Enable auto-reload Preload */
//  TIM1->CR1 |= TIM_CR1_ARPE;
// /* TIM1 DMA Update enable */
//  TIM1->DIER |= TIM_DMA_UPDATE;
// /* Configure of the DMA Base register and the DMA Burst Length */
// /* Reset DBA and DBL bit fields */
//  TIM1->DCR &= ~TIM_DCR_DBA;
//  TIM1->DCR &= ~TIM_DCR_DBL;
// /* Select the DMA base register and DMA burst length */
//  TIM1->DCR = TIM_DMABase_ARR | TIM_DMABurstLength_3Transfers;
// /* Enable UEV by setting UG bit to Load buffer data into preload registers
// */
//  TIM1->EGR |= TIM_EGR_UG;
// /* wait until the RESET of UG bit*/
//  while((TIM1->EGR & TIM_EGR_UG) == SET){}
// /* Enable UEV by setting UG bit to load data from preload to active
// registers */
//  TIM1->EGR |= TIM_EGR_UG;
// /* Enable the TIM1 Main Output */
//  TIM1->BDTR |= TIM_BDTR_MOE;
// /* Enable CC1 output*/
//  TIM1->CCER |= TIM_CCER_CC1E;
// /* Enable the TIM Counter */
//  TIM1->CR1 |= TIM_CR1_CEN;

// GPIO:
// Pin PA8: TIM1_ch1_output
// Mode: push pull
// Pull: pull-up
// Speed: high
// Alternate function: GPIO_AF6_TIM1








// // basically we can 

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
// minus one (i.e.TIM1_RCR=Number_of_pulses_per_portion–1).
// - TIM1_CCR1: contains the duration of the t_on period of the on-going pulse.


// The configuration for the timer registers to be used in order to reconstitute the waveform
// shown in Figure 29 are the following:
// For the first portion of the waveform, the right configuration is:
// • TIM1_ARR = t1_on + t1_off , TIM1_RCR= 1, TIM1_CCR1= t1_on;

// For the second portion of the signal waveform, the right configuration is:
// • TIM1_ARR = t2_on + t2_off , TIM1_RCR= 0, TIM1_CCR1= t2_on;

// For the third portion of the signal waveform, the right configuration is:
// • TIM1_ARR = t3_on + t3_off , TIM1_RCR= 2, TIM1_CCR1= t3_on;


// uint32_t const aSRC_Buffer[9] = { t1_on+t1_off,1,t1_on,
// t2_on+t2_off,0,t2_on, t3_on+t3_off,2,t3_on};

// uint32_t aSRC_Buffer[9] = { 4000,1,800,10000,0,8500,4000,2,200};


// System clock
// – PLL as system clock source: 64 MHz
// – HSI as oscillator (no need to solder HSE in Nucleo board)
// – AHB div = 1 / APB1 div = 2 / APB2 div= 1


// Pulse Width Modulation mode allows to generate a signal with a frequency determined by
// the value of the TIMx_ARR register and a duty cycle determined by the value of the
// TIMx_CCRx register.







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

// Anyway, the comparison between the TIMx_CCRx shadow register and the counter is still
// performed and allows the flag to be set. Interrupt and DMA requests can be sent
// accordingly. This is described in the output compare mode section below.