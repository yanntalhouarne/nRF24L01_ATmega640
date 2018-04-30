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
void setup_TMR2();
void setup_TMR3();
void lcd_print_position();

char buffer[mirf_PAYLOAD] = {0,0,0};
int mtr_cmd = 0;
int old_mtr_cmd = 0;
int old_srv_cmd = 0;
int srv_cmd = 0;
uint8_t comm_lost = 0;
uint8_t lat_deg = 0;
uint8_t lat_min = 0;
uint8_t lat_sec = 0;
uint8_t lon_deg = 0;
uint8_t lon_min = 0;
uint8_t lon_sec = 0;
uint8_t comm_lost_count = 0;
int8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
int8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint16_t loop_delay_counter = 0;
uint8_t GPS_ON = 0;
uint8_t BOAT_ON = 0;
uint8_t BOAT_OFF = 0;

int main(void)
{
	setup_ports();
	setup_TMR1();
	setup_TMR3();
	setup_lcd();
	setup_adc();
	_delay_ms(10);
	lcd_set_cursor(1,1);
	lcd_print("Initializing...");
	_delay_ms(3);
	setup_usart0(BR_500000); // for FTDI debugging (terminal)
	mirf_init();	
	lcd_send_cmd(CLEAR_DISPLAY);
	
	LED_check();
	
	mirf_config();
	
	 /* Set the device addresses */
	mirf_set_TADDR(tx_address);
	mirf_set_RADDR(rx_address);
	
	_delay_ms(10);
	
	lcd_send_cmd(CLEAR_DISPLAY);
	_delay_ms(3);
	lcd_set_cursor(1,1);
	lcd_print("Initialization  ");
	_delay_ms(1000);
	sei(); // enable global interrupts
	
	lcd_send_cmd(CLEAR_DISPLAY);
	_delay_ms(3);
	
	lcd_set_cursor(1,1);
	lcd_print("NO CONNECTION");

    while (1) 
    {
		if (comm_lost_count > 50)
		{
			comm_lost_count = 0;
			mirf_config();
		}
		
		if (GPS_ON && (BOAT_OFF == 0))
		{
			lcd_print_position();
		}
		
		if (BOAT_ON && (BOAT_OFF == 0))
		{
			if (!GPS_ON)
			{
				lcd_set_cursor(1,1);
				lcd_print("Connected,   ");
				lcd_set_cursor(2,1);
				lcd_print("Waiting on GPS  ");	
			}

		}
		
		loop_delay_counter++;
		
		TOGGLE_LED1;


	
	
		if (loop_delay_counter == 50)
		{
			buffer[0] = 'A';
			mirf_send(buffer, mirf_PAYLOAD);
			reset_TMR1();
			while (!mirf_data_sent())
			{
				if (TCNT1 > 3000) // timeout of one second
				{
					comm_lost = 1;
					comm_lost_count++;
					TOGGLE_LED3;
					break;
				}
			}
			if (!comm_lost)
			{
				set_RX_MODE();
				reset_TMR1();
				while(!mirf_data_ready())
				{
					if (TCNT1 > 3000) // timeout of one second
					{
						comm_lost = 1;
						comm_lost_count++;
						TOGGLE_LED6;
						break;
					}
				}
				if (!comm_lost)
				{
					mirf_get_data(buffer); // get the data, put it in buffer
					if(buffer[0] != 0)
					{
						GPS_ON = 1;
						lat_deg = buffer[0];
						lat_min = buffer[1];
						lat_sec = buffer[2];
					}
					else
						GPS_ON = 0;
				}
				else
				{
					GPS_ON = 0;
					comm_lost = 0;
				}
			}
			else
			{
				comm_lost = 0;
			}
		}
		if (loop_delay_counter > 100)
		{
			loop_delay_counter = 0;
			buffer[0] = 'O';
			mirf_send(buffer, mirf_PAYLOAD);
			reset_TMR1();
			while (!mirf_data_sent())
			{
				if (TCNT1 > 3000) // timeout of one second
				{
					comm_lost = 1;
					comm_lost_count++;
					TOGGLE_LED3;
					break;
				}
			}
			if (!comm_lost)
			{
				set_RX_MODE();
				reset_TMR1();
				while(!mirf_data_ready())
				{
					if (TCNT1 > 3000) // timeout of one second
					{
						comm_lost = 1;
						comm_lost_count++;
						TOGGLE_LED6;
						break;
					}
				}
				if (!comm_lost)
				{
					mirf_get_data(buffer); // get the data, put it in buffer
					lon_deg = buffer[0];
					lon_min = buffer[1];
					lon_sec = buffer[2];
					
				}
				else
				comm_lost = 0;
			}
			else
			{
				comm_lost = 0;
			}
		}
		else 
		{
			mtr_cmd = analog_get_average(JOYSTICK2_Y, 5);
			mtr_cmd = .75 * mtr_cmd + .25 * old_mtr_cmd;
			old_mtr_cmd = mtr_cmd;
			mtr_cmd = js_mtr_scaling(mtr_cmd); 
			buffer[0] = (mtr_cmd >> 8); // MSB
			buffer[1] = mtr_cmd; // LSB
			
			srv_cmd = analog_get_average(JOYSTICK2_X, 5);
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
			while (!mirf_data_sent())
			{
				if (TCNT1 > 3000) // timeout of one second
				{
					comm_lost = 1;
					comm_lost_count++;
					TOGGLE_LED3;
					break;
				}
			}
			if (!comm_lost)
			{
				BOAT_ON = 1; // one time flag
				BOAT_OFF = 0;
			}
			else
			{
				comm_lost = 0;	
				BOAT_OFF = 1;			
			}

		}
		
		_delay_ms(LOOP_DELAY);
    }
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& JOYSTICK SCALING &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
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

void lcd_print_position()
{
	lcd_set_cursor(1,1);
	lcd_print("LAT:");
	lcd_set_cursor(1,7);
	lcd_print(",");
	lcd_set_cursor(1,10);
	lcd_print(",");

	lcd_set_cursor(2,1);
	lcd_print("LON:");
	lcd_set_cursor(2,7);
	lcd_print(",");
	lcd_set_cursor(2,10);
	lcd_print(",");
				
	lcd_set_cursor(1,5);
	lcd_print_int(lat_deg);
	lcd_set_cursor(1,8);
	lcd_print_int(lat_min);
	lcd_set_cursor(1,11);
	lcd_print_int(lat_sec);
	lcd_print(" ");
				
	lcd_set_cursor(2,5);
	lcd_print_int(lon_deg);
	lcd_set_cursor(2,8);
	lcd_print_int(lon_min);
	lcd_set_cursor(2,11);
	lcd_print_int(lon_sec);
	lcd_print("     ");
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& TIMER 1 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
void setup_TMR1()
{
	TCCR1B |= (1<<CS12); // 256 prescaler, CTC mode

}

void reset_TMR1()
{
	TCNT1 = 0;
}

void setup_TMR3()
{
	TCCR3A |= (1<<COM3A1); // set OC2A on compare match
	TCCR3B |= (1<<WGM32) | (1<<CS32) | (1<<CS30); // 1024 prescaler, CTC mode
	OCR3A = 31248; 
	TIMSK3 |= (1<<OCIE3A);
}

ISR(TIMER3_COMPA_vect)
{
	if (BOAT_ON)
	{
		lcd_set_cursor(1,14);
		if( comm_lost_count <= 0)
		lcd_print("***");
		else if ( (comm_lost_count>0) && (comm_lost_count<3) )
		lcd_print("** ");
		else if ( (comm_lost_count>=3) && (comm_lost_count<6) )
		lcd_print("*  ");
		else if ( (comm_lost_count>=6) && (comm_lost_count<10))
		lcd_print("OFF");
		else 
		{
			lcd_set_cursor(1,1);
			lcd_print("CONNECTION LOST,");
			lcd_set_cursor(2,1);
			lcd_print("RESET CONTROLLER");             
		}
		comm_lost_count = 0;
	}
}