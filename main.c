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

/* joystick reading and scaling */
#define CMD_SCALE 1.44
#define NEUTRAL_CMD 493
#define DEADBAND_MIN 460
#define DEADBAND_MAX 550
#define OFFSET 7

#define BUFFER_SIZE 2

void setup_TMR1();
void reset_TMR1();
int js_mtr_scaling(int value);
int js_srv_scaling(float value);

char buffer[mirf_PAYLOAD] = {0,0,0};
int mtr_cmd = 0;
int old_mtr_cmd = 0;
int old_srv_cmd = 0;
int srv_cmd = 0;

uint8_t comm_lost_count = 0;
int8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
int8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
	
uint8_t status = 0;

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
	
	lcd_set_cursor(1,11);
	lcd_print("RF: ");

    while (1) 
    {
		
		TOGGLE_LED1;
		lcd_set_cursor(1,5);
		lcd_print_int(mtr_cmd);
		lcd_print("  ");
		lcd_set_cursor(2,5);
		lcd_print_int(srv_cmd);
		lcd_print("   ");
		lcd_set_cursor(1,15);
		lcd_print_int(comm_lost_count);
		lcd_print(" ");
		
		mtr_cmd = analog_get_average(JOYSTICK1_Y, 5);
		mtr_cmd = .75 * mtr_cmd + .25 * old_mtr_cmd;
		old_mtr_cmd = mtr_cmd;
		mtr_cmd = js_mtr_scaling(mtr_cmd); 
		buffer[0] = (mtr_cmd >> 8); // MSB
		buffer[1] = mtr_cmd; // LSB
		srv_cmd = analog_get_average(JOYSTICK1_X, 5);
		srv_cmd = .75 * srv_cmd + .25 * old_srv_cmd;
		old_srv_cmd = srv_cmd;
		srv_cmd = js_srv_scaling(srv_cmd);
		if ((srv_cmd < 5) && (srv_cmd > -5))
			srv_cmd = 0;
		else if (srv_cmd > 45)
			srv_cmd = 45;
		else if (srv_cmd < -45)
			srv_cmd = -45;
		buffer[2] = srv_cmd;

		mirf_send(buffer, mirf_PAYLOAD);
		_delay_us(10);
		reset_TMR1();
		//TOGGLE_LED6;
		
		while (!mirf_data_sent())
		{
			if (TCNT1 > 62499) // timeout of one second
			{
				comm_lost_count++;
				TOGGLE_LED6;
				break;
			}
		}
		
		_delay_ms(LOOP_DELAY);
    }
}

int js_mtr_scaling(int value) // scales the result to commands from -1000 to 1000.
{
	value = value * CMD_SCALE - OFFSET; // scale to 0 -> 1000

	if ((value < DEADBAND_MAX) && (value > DEADBAND_MIN)) // if within the dead band, send neutral command (0)
	value = 0;
	else if (value > DEADBAND_MAX)            // if joystick is higher than deadband
	value = 2 * (value - NEUTRAL_CMD);    // compute the change from the neutral position multiply by 2 to scale to 1000
	else if (value < DEADBAND_MIN)            // if joystick is lower than deadband
	value = (-2) * (NEUTRAL_CMD - value); // compute the change from the neutral position and invert (multiply by 2 to scale to -1000
	if (value > 1000)                         // do not send any value bigger than 1000 or smaller than -1000
	value = 1000;

	return value;
} // end of joystick_scaling

int js_srv_scaling(float value) // scales the result to commands from -1000 to 1000.
{
	value = value * CMD_SCALE - OFFSET; // scale to 0 -> 1000
	if ((value < DEADBAND_MAX) && (value > DEADBAND_MIN)) // if within the dead band, send neutral command (0)
	value = 0;
	else if (value > DEADBAND_MAX)            // if joystick is higher than deadband
	{
		value = 2 * (value - NEUTRAL_CMD);    // compute the change from the neutral position multiply by 2 to scale to 1000
		value = value  / 22.2; // scale to -45 45
	}
	else if (value < DEADBAND_MIN)            // if joystick is lower than deadband
	{
		value = (-2) * (NEUTRAL_CMD - value); // compute the change from the neutral position and invert (multiply by 2 to scale to -1000
		value = value  / 22.2; // scale to -45 45
	}
	if (value > 1000)                         // do not send any value bigger than 1000 
	value = 1000;
	if (value < -1000)                        // do not send any value smaller than -1000
	value = -1000;
	

	

	return value;
} // end of joystick_scaling

void setup_TMR1()
{
	TCCR1B |= (1<<CS12); // 256 prescaler, CTC mode

}

void reset_TMR1()
{
	TCNT1 = 0;
}
