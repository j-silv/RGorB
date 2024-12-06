#include "gpio.h"


#define PORT2AHBEN(port) (0x01 << (port))                              // port number to AHB enable bit
#define PORT2BASE(port) ((AHB1PERIPH_BASE + 0x0400U*(port)))           // port to GPIO base address value
#define PORT2GPIO(port) ((GPIO_TypeDef *) PORT2BASE(port))             // port to pointer to GPIO base address 
#define PIN2MODER(pin, mode) (((mode) & 0x03U) << (0x02U*(pin)))       // pin to the MODER register position
#define PIN2AFR(pin, af) (((af) & 0x0FU) << (0x04U*((pin) & 0x7U)))    // pin to the AFR register position


gpio_struct gpio_init(gpio_port_enum port, uint8_t pin, gpio_mode_enum mode) {
    gpio_struct gpio = {port, pin, PORT2GPIO(port)};
    RCC->AHB1ENR |= (0x1UL << (port));
    gpio_mode(&gpio, mode);
    return gpio;
}

// set GPIO mode (input, output, alternate function)
void gpio_mode(gpio_struct* gpio, gpio_mode_enum mode) {
    gpio->reg->MODER |= PIN2MODER(gpio->pin, mode);
}

void gpio_set(gpio_struct* gpio){
    gpio->reg->BSRR |= 1UL << gpio->pin;
}

void gpio_reset(gpio_struct* gpio){
    gpio->reg->BSRR |= (1UL << gpio->pin) << 16UL;
}

void gpio_af(gpio_struct* gpio, gpio_af_enum af){
    uint8_t idx = 0; // by default we are looking at pins 0-7 (AFRL)
    if (gpio->pin > 7UL) {
        idx = 1; // change to pins 8-15 (AFRH)
    }
    gpio->reg->AFR[idx] |= PIN2AFR(gpio->pin, af);
}




void gpio_opendrain(gpio_struct* gpio, uint8_t enable){}
void gpio_pull(gpio_struct* gpio, gpio_pull_enum pull){}