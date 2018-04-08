/*
 * adc.h
 *
 * Created: 4/8/2018 1:22:01 AM
 *  Author: talho_000
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#define DELAY_CONVERSION 1
#define AN0 0
#define AN1 1
#define AN2 2
#define AN3 3
#define AN4 4
#define AN5 5
#define AN6 6
#define AN7 7
#define AN8 8

#define ADC_SCALING 10.23 // divide analog-read int by this number

void setup_adc();
int analog_read(unsigned char channel);
int analog_get_average(unsigned char channel, unsigned char elements);

#endif /* ADC_H_ */


