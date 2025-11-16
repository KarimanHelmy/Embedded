


#ifndef ADC_H_
#define ADC_H_

typedef enum {CHANNEL0,CHANNEL1,CHANNEL2,CHANNEL3}channel_t;
void adc_init (void);

/**
 *      Read analog value from a certain channel 
 */
uint8_t adc_read(uint8_t channel);

/**
 *Read analog value from all chanels
 */
void adc_read_all(uint8_t * ret_vals);

#endif /* ADC_H_ */

