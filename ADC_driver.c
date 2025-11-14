/*
 * ADC.c
 *
 * Created: 15.09.2016 15:46:33
 *  Author: Whiskey Dicks
 */
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "ADC_driver.h"

#ifndef ADC_ADDRESS
#define ADC_ADDRESS (volatile char*)0x1400
#endif

//volatile char* ext_adc = ADC_ADDRESS; //Create a pointer to the array of all addresses we will write to. ADC starting at 0x1400.
#define ADC_BUSY        !(PIND & (1<<PD4))

/**
 *      Initializes the ext. ADC
 */
void adc_init (void) {
        MCUCR |= (1 << SRE ); // enable XMEM
        SFIOR |= (1 << XMM2 ); // mask unused bits

        /* configure CLK signal and BUSY pin */

        /* configure Timer Counter Control register 1A */
        TCCR1A |= (1<< WGM11) |(1<< WGM10) | (1<< COM1A0);
        TCCR1B |= (1<< WGM13) | (1<< WGM12) | (1<< CS10);

        ICR1 = 1;
        /* use PD5 as output */
        DDRD |= (1<< PD5);

        /* use PD4 as input */
        DDRD &= ~(1<< PD4);
}

/**
 *      Read analog value from the given channel of the ext. ADC
 */
uint8_t adc_read(uint8_t channel) {
        uint8_t ret_val = 0, i;

        volatile uint8_t * ext_adc = ( uint8_t *) ADC_ADDRESS ;
        /* WR is set implicitly by writing to the address range */
        *ext_adc = 0x00;

        while(ADC_BUSY); // blocking function!

        for (i=0;i<=channel;i++) {
                /* RD is set implicitly by reading from the address range */
                ret_val = *ext_adc;
        }
        return ret_val;
}

/*
 *  Read analog values of all channels from the ext. ADC
 */
void adc_read_all(uint8_t * ret_vals) {
        uint8_t i;

         volatile uint8_t * ext_adc = ( uint8_t *) ADC_ADDRESS ;
         /* WR is set implicitly by writing to the address range */
         *ext_adc = 0x00;

        while(ADC_BUSY); // blocking function!

        for (i=0;i<4;i++) {
                /* RD is set implicitly by reading from the address range */
                ret_vals[i] = *ext_adc;
        }
}
                              