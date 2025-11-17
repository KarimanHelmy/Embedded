/*
 * SPI_driver.c
 *
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "SPI_driver.h"

// Initializes the SPI peripheral in Master mode: configures pins, sets clock to f_clk/16, and enables SPI.
void SPI_init(){
	
	/* Set SS, MOSI and SCK output, all others input */
	DDRB |= (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
    DDRB &= ~(1<<DDB6);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    SREG |= (1<<7);
    MCUCR &= ~(0b11<<ISC00);
}

// Performs a blocking SPI write: sends one byte and waits until the transfer is complete (received data is ignored).
void SPI_write(char cData) {
	
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
}

// Performs a full-duplex SPI transfer: sends one byte and returns the byte received at the same time.
uint8_t SPI_transfer(uint8_t cData) {
    SPDR = cData;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;  // Return received byte
}

// Performs an SPI read by sending a dummy byte and returning the received byte from the slave.
char SPI_read() {
	
	/* Start shifting registers by putting a char in the register */
	SPDR = 0x00;
	 
	/* Wait for receive complete */ 
	loop_until_bit_is_set(SPSR,SPIF);
	
	return SPDR;
}
