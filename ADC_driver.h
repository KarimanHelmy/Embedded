/*
 * ADC.h
 *
 * Created: 15.09.2016 15:46:43
 *  Author: Whiskey Dicks
 */ 


#ifndef ADC_H_
#define ADC_H_

typedef enum {CHANNEL0,CHANNEL1,CHANNEL2,CHANNEL3}channel_t;
void adc_init (void);

/**
 *      Read analog value from the given channel of the ext. ADC
 */
uint8_t adc_read(uint8_t channel);

/**
 *Read analog values of all channels from the ext. ADC
 */
void adc_read_all(uint8_t * ret_vals);

#endif /* ADC_H_ */
