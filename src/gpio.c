#include "gpio.h"


#define PORT2AHBEN(port) (0x1UL << (port))                     // port number to AHB enable bit
#define PORT2BASE(port) ((AHB1PERIPH_BASE + 0x0400UL*(port)))  // port to GPIO base address value
#define PORT2GPIO(port) ((GPIO_TypeDef *) PORT2BASE(port))     // port to pointer to GPIO base address 
#define PIN2MODER(pin, mode) (((mode) & 3UL) << (0x2UL*(pin))) // pin to the MODER register position


gpio_struct gpio_init(gpio_port_enum port, uint8_t pin, gpio_mode_enum mode) {
    gpio_struct gpio = {port, pin};
    RCC->AHB1ENR |= (0x1UL << (port));
    gpio_mode(&gpio, mode);
    return gpio;
}

// set GPIO mode (input, output, alternate function)
void gpio_mode(gpio_struct* gpio, gpio_mode_enum mode) {
    PORT2GPIO(gpio->port)->MODER |= PIN2MODER(gpio->pin, mode);
}


void gpio_opendrain(gpio_struct* gpio, uint8_t enable){}
void gpio_pull(gpio_struct* gpio, gpio_pull_enum pull){}
void gpio_af(gpio_struct* gpio, gpio_af_enum af){}

void gpio_set(gpio_struct* gpio){
    PORT2GPIO(gpio->port)->BSRR |= 1UL << gpio->pin;
}

void gpio_reset(gpio_struct* gpio){
    PORT2GPIO(gpio->port)->BSRR |= (1UL << gpio->pin) << 16UL;
}