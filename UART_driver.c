/*
 * UART_driver.c
 *
 * Created: 03.09.2016 16:32:20
 *  Author: Whiskey dicks
 */

// UART = Universal Asynchronous Receiver/Transmitter


#include <avr/io.h>
#include <stdio.h>

#include "bit_macros.h"
#include "UART_driver.h"

#define BAUD 9600 //Baud rate = symbols pr second.

void UART_init(unsigned long clock_speed){

	//unsigned char ubrr = (clock_speed / (baud * 16)) - 1;	//Should be, and is, 31
	unsigned long ubrr = 31;

    /******************************************************************************/
    
    UBRR1H = ubrr >> 8;			// Save the most significant bits (4 most sign. bits out of 12 bits)
	UBRR1L = ubrr;				// Save the least sign. bits (8 least sign. bits)
	
	/* Set frame format: 8data, 2stop bit */								
	UCSR1C = (1<<URSEL1)|(1<<USBS1) |(3<<UCSZ00);		// UCSZ00: 3 - UCSZ0 and UCSZ1 defines 8 bits (11 binary) for each character	
	
	/* Enable receiver and transmitter */
	set_bit(UCSR1B,RXEN1);		//RXEN enables the receiver.
	set_bit(UCSR1B,TXEN1);//TXEN enables the transmitter.
	fdevopen(put_char, get_char); 
}

void put_char(char c, FILE *stream){
	//loop_until_bit_is_set(UCSR0A, UDRE0);
    while ( !( UCSR1A & (1<<UDRE1)) );
	//unsigned char 
    UDR1 = c;
}

char get_char(FILE *stream){
     
	while ( !(UCSR1A & (1<<RXC0)) );

	return UDR1;
}



