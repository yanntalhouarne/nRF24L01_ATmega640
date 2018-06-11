/*
 * external_interrupt.c
 *
 * Created: 6/10/2018 6:41:06 PM
 *  Author: Yann
 */
 
#include <avr/io.h>
#include "gpio.h"
#include "print.h"

void setup_ext_int(uint8_t INT, uint8_t trigger)
{
	if (trigger == ANY)
	{
		if (INT == 0)
		{
			EICRA |= (1<<ISC00);
			EICRA &= ~(1<<ISC01);
		}
		else if (INT == 1)
		{
			EICRA |= (1<<ISC10);
			EICRA &= ~(1<<ISC11);
		}
		else if (INT == 2)
		{
			EICRA |= (1<<ISC20);
			EICRA &= ~(1<<ISC21);
		}
		else if (INT == 3)
		{
			EICRA |= (1<<ISC30);
			EICRA &= ~(1<<ISC31);
		}
		else if (INT == 4)
		{
			EICRB |= (1<<ISC40);
			EICRB &= ~(1<<ISC41);
		}

		else if (INT == 5)
		{
			EICRB |= (1<<ISC50);
			EICRB &= ~(1<<ISC51);
		}
		else if (INT == 6)
		{
			EICRB |= (1<<ISC60);
			EICRB &= ~(1<<ISC61);
		}
		else if (INT == 7)
		{
			EICRB |= (1<<ISC70);
			EICRB &= ~(1<<ISC71);
		}
	}
	if (trigger == FALLING)
	{
		if (INT == 0)
		{
			EICRA |= (1<<ISC01);
			EICRA &= ~(1<<ISC00);
		}
		else if (INT == 1)
		{
			EICRA |= (1<<ISC11);
			EICRA &= ~(1<<ISC10);
		}
		else if (INT == 2)
		{
			EICRA |= (1<<ISC21);
			EICRA &= ~(1<<ISC20);
		}
		else if (INT == 3)
		{
			EICRA |= (1<<ISC31);
			EICRA &= ~(1<<ISC30);
		}
		else if (INT == 4)
		{
			EICRB |= (1<<ISC41);
			EICRB &= ~(1<<ISC40);
		}

		else if (INT == 5)
		{
			EICRB |= (1<<ISC51);
			EICRB &= ~(1<<ISC50);
		}
		else if (INT == 6)
		{
			EICRB |= (1<<ISC61);
			EICRB &= ~(1<<ISC60);
		}
		else if (INT == 7)
		{
			EICRB |= (1<<ISC71);
			EICRB &= ~(1<<ISC70);
		}
	}
	else
	{
		if (INT == 0)
			EICRA |= (1<<ISC01) | (1<<ISC00);
		else if (INT == 1)
			EICRA |= (1<<ISC11) | (1<<ISC10);
		else if (INT == 2)
			EICRA |= (1<<ISC21) | (1<<ISC20);
		else if (INT == 3)
			EICRA |= (1<<ISC31) | (1<<ISC30);
		else if (INT == 4)
			EICRB |= (1<<ISC41) | (1<<ISC40);
		else if (INT == 5)
			EICRB |= (1<<ISC51) | (1<<ISC50);
		else if (INT == 6)
			EICRB |= (1<<ISC61) | (1<<ISC60);
		else if (INT == 7)
			EICRB |= (1<<ISC71) | (1<<ISC70);
	}
	
	EIMSK |= (1<<INT);
}

uint8_t digital_read(uint8_t PORT, uint8_t PIN)
{
	uint8_t LEVEL = 0;
	
	if (PORT == PORT_A)
	{
		LEVEL = ((PINA >> PIN) & 0x01);
	}
	else if (PORT == PORT_B)
	{
		LEVEL = ((PINB >> PIN) & 0x01);
	}
	else if (PORT == PORT_C)
	{
		LEVEL = ((PINC >> PIN) & 0x01);
	}
	else if (PORT == PORT_D)
	{
		LEVEL = ((PIND >> PIN) & 0x01);
	}
	else if (PORT == PORT_E)
	{
		LEVEL = ((PINE >> PIN) & 0x01);
	}
	else if (PORT == PORT_F)
	{
		LEVEL = ((PINF >> PIN) & 0x01);
	}
	else if (PORT == PORT_G)
	{
		LEVEL = ((PING >> PIN) & 0x01);
	}
	else if (PORT == PORT_H)
	{
		LEVEL = ((PINH >> PIN) & 0x01);
	}
	else if (PORT == PORT_J)
	{
		LEVEL = ((PINJ >> PIN) & 0x01);
	}
	else if (PORT == PORT_K)
	{
		LEVEL = ((PINK >> PIN) & 0x01);
	}
	else if (PORT == PORT_L)
	{
		LEVEL = ((PINL >> PIN) & 0x01);
	}
	
	println_int_0(LEVEL);
	
	return LEVEL;
}