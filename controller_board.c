/*
 * controller_board.c
 *
 * Created: 2/1/2018 11:15:41 AM
 *  Author: Yann
 */ 

#define F_CPU 16000000

#include "controller_board.h"
#include <util/delay.h>

#define LED_check_DELAY 10


void setup_ports()
{
	// LEDs
	DDRL |= (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4) | (1<<LED5) | (1<<LED6); // set LEDs as outputs

}

void LED_check()
{
	int i;
	
	for (i=0; i<3; i++)
	{
		LED1_ON;
		_delay_ms(LED_check_DELAY);
		LED1_OFF;
		LED2_ON;
		_delay_ms(LED_check_DELAY);
		LED2_OFF;
		LED3_ON;
		_delay_ms(LED_check_DELAY);
		LED3_OFF;
		LED4_ON;
		_delay_ms(LED_check_DELAY);
		LED4_OFF;
		LED5_ON;
		_delay_ms(LED_check_DELAY);
		LED5_OFF;
		LED6_ON;
		_delay_ms(LED_check_DELAY);
		
		_delay_ms(LED_check_DELAY);
		LED6_OFF;
		LED5_ON;
		_delay_ms(LED_check_DELAY);
		LED5_OFF;
		LED4_ON;
		_delay_ms(LED_check_DELAY);
		LED4_OFF;
		LED3_ON;
		_delay_ms(LED_check_DELAY);
		LED3_OFF;
		LED2_ON;
		_delay_ms(LED_check_DELAY);
		LED2_OFF;
		LED1_ON;
		_delay_ms(LED_check_DELAY);
		LED1_OFF;
	}

}