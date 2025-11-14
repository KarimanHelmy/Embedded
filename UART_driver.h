/*
 * UART_driver.h
 *
 * Created: 03.09.2016 16:32:57
 *  Author: Whiskey dicks
 */ 


#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_
#include <avr/io.h>
#include <stdio.h>
void UART_init(unsigned long clock_speed);
void put_char(char c, FILE *stream);
char get_char(FILE *stream);
void uart_link_printf();
void send_string(char *str);


#endif /* MAX233_DRIVER_H_ */