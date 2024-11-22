#ifndef INC_UART_H_
#define INC_UART_H_
#include <stdint.h>

void UART_INIT(void);
void UART_WRITE(uint8_t* data, uint8_t size);

#endif 
