#include <avr/io.h>

#include "usart.h"
#include "print.h"

void usart0_send_char(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)))
	; // wait for data buffer to be empty
	UDR0 = data;
}


unsigned char usart0_receive_char()
{
	while (!(UCSR0A & (1 << RXC0))) // RXCn is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
	;
	return UDR0;
}


void setup_usart0(unsigned char BR)
{
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); // enable USART1
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);               // 8-bit character size
	UBRR0L = BR;                                        // 51 for 9600 baud rate at 8Mhz
}


void usart1_send_char(unsigned char data)
{
	while (!(UCSR1A & (1 << UDRE0)))
	; // wait for data buffer to be empty
	UDR1 = data;
}


unsigned char usart1_receive_char()
{
	while (!(UCSR1A & (1 << RXC0))) // RXCn is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
	;
	return UDR1;
}

void usart1_receive_string()
{
	int i = 0;
	while(1)
	{
		while (!(UCSR1A & (1 << RXC0))) // RXCn is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
		;
		rcv_string[i] = UDR1;
		if ((rcv_string[i] == NL) || (i == MAX_STRING_SIZE))
		{
			break;
		}
		else
		{
			i++;
			
		}
	}
}

void setup_usart1(unsigned char BR)
{
	UCSR1B = (1 << TXEN0) | (1 << RXEN0); // enable USART1
	UCSR1C = (1 << UCSZ01) | (1 << UCSZ00);               // 8-bit character size
	UBRR1L = BR;                                        // 51 for 9600 baud rate at 8Mhz
}


