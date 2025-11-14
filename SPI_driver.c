/*
 * SPI_driver.c
 *
 * Created: 13.10.2016 09:06:13
 *  Author: Whiskey Dicks
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "SPI_driver.h"

void SPI_init(){
	
	/* Set SS, MOSI and SCK output, all others input */
	DDRB |= (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
    DDRB &= ~(1<<DDB6);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	set_bit(PORTB, PB4);
    
    SREG |= (1<<7);
    
    
    MCUCR &= ~(0b11<<ISC00);
}

void SPI_write(char cData) {
	
	
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
}

uint8_t SPI_transfer(uint8_t cData) {
    SPDR = cData;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;  // Return received byte
}

char SPI_read() {
	
	/* Start shifting registers by putting a char in the register */
	SPDR = 0x00;
	 
	/* Wait for receive complete */ 
	loop_until_bit_is_set(SPSR,SPIF);
	
	return SPDR;
}