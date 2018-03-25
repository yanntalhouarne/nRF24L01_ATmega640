/*
 * nRF24L01_Atmega324PB.c
 *
 * Created: 3/17/2018 11:41:05 PM
 * Author : Yann
 */ 
#define F_CPU 16000000
#define LOOP_DELAY 100

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "controller_board.h"
#include "print.h"
#include "usart.h"
#include "spi.h"
#include "mirf.h"
#include "lcd.h"

#define BUFFER_SIZE 2

char buffer[mirf_PAYLOAD] = {1,2};

uint8_t status = 0;

int main(void)
{
	setup_ports();
	setup_lcd();
	_delay_ms(10);
	lcd_set_cursor(1,1);
	lcd_print("Initializing...");
	_delay_ms(3);
	setup_usart0(BR_9600); // for FTDI debugging (terminal)
	mirf_init();
	_delay_ms(2000);	
	lcd_send_cmd(CLEAR_DISPLAY);
	
	LED_check();
	
	sei(); // enable global interrupts
	
	mirf_config();
	
	_delay_ms(10);

    while (1) 
    {
		// to UART
		//println_0("Waiting for data...;");
		// to LCD
		lcd_send_cmd(CLEAR_DISPLAY);
		_delay_ms(3);
		lcd_set_cursor(1,1);
		lcd_print("Waiting for data");
		_delay_ms(3);
		
		// wait for data
		while(!mirf_data_ready())
		{
			TOGGLE_LED1; // flash LED while waiting for data
			_delay_ms(100);
		}
		LED1_ON; // turn on LED when data is received
		
		mirf_get_data(buffer);
		
		/*
		// send to UART
		print_0("Data received: ;");
		print_char_0(buffer[0]);
		print_char_0(',');
		print_char_0(' ');
		print_char_0(buffer[1]);
		print_char_0(NL);
		*/
		
		// send to LCD
		lcd_send_cmd(CLEAR_DISPLAY);
		_delay_ms(3);
		lcd_set_cursor(1,1);
		lcd_print("Data received:");
		_delay_ms(3);
		lcd_set_cursor(2,1);
		lcd_print_int(buffer[0]);
		_delay_ms(3);
		lcd_print(", ");
		_delay_ms(3);
		lcd_print_int(buffer[1]);
		_delay_ms(3);
		
		_delay_ms(5000);
		
		buffer[0] = 3;
		buffer[1] = 4;
		
		lcd_send_cmd(CLEAR_DISPLAY);
		_delay_ms(3);
		lcd_set_cursor(1,1);
		_delay_ms(3);
		lcd_print("Sending data...");
		_delay_us(10);
		mirf_send(buffer, mirf_PAYLOAD);
		_delay_us(10);
		while (!mirf_data_sent());
		mirf_config_register(STATUS, (1 << TX_DS) | (1 << MAX_RT)); // Reset status register
		lcd_set_cursor(2,1);
		_delay_ms(3);
		lcd_print("Data sent.");
		_delay_us(10);
		
		while(1);
		
		_delay_ms(LOOP_DELAY);
    }
}

