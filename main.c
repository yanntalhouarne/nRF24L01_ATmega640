/*
 * nRF24L01_Atmega324PB.c
 *
 * Created: 3/17/2018 11:41:05 PM
 * Author : Yann
 */ 
#define F_CPU 16000000
#define LOOP_DELAY 10

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "controller_board.h"
#include "print.h"
#include "usart.h"
#include "adc.h"
#include "spi.h"
#include "mirf.h"
#include "lcd.h"

#define BUFFER_SIZE 2

void setup_TMR1();
void reset_TMR1();

char buffer[mirf_PAYLOAD] = {0,0};
int mtr_cmd = 0;
int srv_cmd = 0;

uint8_t comm_lost_count = 0;
int8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
int8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
	
uint8_t status = 0;
uint8_t comm_lost = 0;

int main(void)
{
	setup_ports();
	setup_TMR1();
	setup_lcd();
	setup_adc();
	_delay_ms(10);
	lcd_set_cursor(1,1);
	lcd_print("Initializing...");
	_delay_ms(3);
	setup_usart0(BR_500000); // for FTDI debugging (terminal)
	mirf_init();
	_delay_ms(2000);	
	lcd_send_cmd(CLEAR_DISPLAY);
	
	LED_check();
	
	sei(); // enable global interrupts
	
	mirf_config();
	
	 /* Set the device addresses */
	mirf_set_TADDR(tx_address);
	mirf_set_RADDR(rx_address);
	
	_delay_ms(10);
	
	print_0("System initialized...;");
	
	lcd_send_cmd(CLEAR_DISPLAY);
	_delay_ms(3);

	lcd_set_cursor(1,1);
	lcd_print("mtr:  ");
	
	lcd_set_cursor(2,1);
	lcd_print("srv: ");
	lcd_print_int(mtr_cmd);
	
	lcd_set_cursor(1,9);
	lcd_print("RF: ");
	lcd_print_int(mtr_cmd);

    while (1) 
    {
		
		TOGGLE_LED1;
		lcd_set_cursor(1,5);
		lcd_print_int(mtr_cmd);
		lcd_print(" ");
		lcd_set_cursor(2,5);
		lcd_print_int(srv_cmd);
		lcd_print(" ");
		if (comm_lost_count == 0)
		{
			lcd_set_cursor(1,13);
			lcd_print("GOOD");
		}
		else if (((comm_lost_count > 6) && (comm_lost_count < 15)))
		{
			lcd_set_cursor(1,13);
			lcd_print("BAD  ");
		}
		else if (comm_lost_count > 15)
		{
			lcd_set_cursor(1,13);
			lcd_print("LOST");
		}
		mtr_cmd = analog_get_average(POT1, 5);
		mtr_cmd /= ADC_SCALING;
		buffer[0] = mtr_cmd;
		srv_cmd = analog_get_average(POT2, 5);
		srv_cmd /= ADC_SCALING;
		if (srv_cmd < 5)
			srv_cmd = 0;
		else if (srv_cmd > 45)
			srv_cmd = 45;
		buffer[1] = srv_cmd;
		
		mirf_send(buffer, mirf_PAYLOAD);
		_delay_us(10);
		reset_TMR1();
		//TOGGLE_LED6;
		
		while (!mirf_data_sent())
		{
			if (TCNT1 > 62499) // timeout of one second
			{
				comm_lost_count++;
				comm_lost = 1;
				TOGGLE_LED6;
				break;
			}
		}
		if (!comm_lost) // a successful transaction decrements comm_lost_count while a time-out increments it
		{
			comm_lost_count--; // decrement comm_lost_count
			if (comm_lost_count < 0) // prevent comm_lost_count from being negative
				comm_lost_count = 0;
		}
		comm_lost = 0;
		
		_delay_ms(LOOP_DELAY);
    }
}

void setup_TMR1()
{
	TCCR1B |= (1<<CS12); // 256 prescaler, CTC mode

}

void reset_TMR1()
{
	TCNT1 = 0;
}
