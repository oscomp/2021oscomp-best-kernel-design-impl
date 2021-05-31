#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <type.h>

#define REG_DAT     0x00
#define REG_IER     0x01
#define REG_IIR     0x02
#define REG_FCR     0x02
#define REG_LCR     0x03
#define REG_MCR     0x04
#define REG_LSR     0x05
#define REG_MSR     0x06
#define REG_CR      0x08
#define REG_MR      0x09

#define COLOR_RED      "\e[31m"
#define COLOR_GREEN    "\e[32m"
#define COLOR_YELLOW   "\e[33m"
#define COLOR_BLUE     "\e[34m"
#define COLOR_MAGENTA  "\e[35m"
#define COLOR_CYAN     "\e[36m"
#define COLOR_RESET    "\e[0m"

// enter a char into serial port
// use sbi printch function
void port_write_ch(char ch);

// enter a message into seraial port
// use sbi printstr function
void port_write(char *buf);

#endif
