

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_
#include <avr/io.h>
#include <stdio.h>
void UART_init(unsigned long clock_speed);
void send_char(char c, FILE *stream);
char recieve_char(FILE *stream);
void uart_link_printf();
void send_string(char *str);



#endif /* MAX233_DRIVER_H_ */
