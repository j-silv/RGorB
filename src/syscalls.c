#include "uart.h"
#include <stdint.h>

// "Stub" functions so that the ARM linker does not give a warning
int _close(int fd) {
  (void) fd;
  return -1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd, (void) ptr, (void) dir;
  return 0;
}

int _write(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  if (fd == 1) UART_WRITE((uint8_t *)ptr, (uint8_t) len);
  return -1;
}

int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}
