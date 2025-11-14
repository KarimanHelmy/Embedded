


#include <avr/io.h>
#include <stdio.h>

#include "bit_macros.h"
#include "UART_driver.h"

#define BAUD rate
// UART initilization, saving the most signficant bit (4/12) and the least (the remaining )
void UART_init(unsigned long clock_speed){

	
	unsigned long ubrr = 31;

    
    UBRR1H = ubrr >> 8;		
	UBRR1L = ubrr;				
	
	/* 8 data bit and 2stop bit  */								
	UCSR1C = (1<<URSEL1)|(1<<USBS1) |(3<<UCSZ00);		
	
	/* Enable receiver and transmitter */
	set_bit(UCSR1B,RXEN1);		
	set_bit(UCSR1B,TXEN1);
	fdevopen(send_char, recieve_char); 
}

void send_char(char c, FILE *stream){
	
    while ( !( UCSR1A & (1<<UDRE1)) );
	
    UDR1 = c;
}

char recieve_char(FILE *stream){
     
	while ( !(UCSR1A & (1<<RXC0)) );

	return UDR1;
}





