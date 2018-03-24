/*
 * lcd.c
 *
 * Created: 1/27/2018 4:16:18 PM
 *  Author: Yann
 */ 

#define F_CPU 16000000
#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>

void setup_lcd()
{	
	// PORT setup
	DDRC |= (1<<RS)  | (1<<RW)  | (1<<E) | (1<<DB0) | (1<<DB1) | (1<<DB2) | (1<<DB3) | (1<<DB4) ;
	DDRG |= (1<<DB5) | (1<<DB6);
	DDRD |= (1<<DB7);
	
	
	E_LOW;
		_delay_ms(1000); // at least 40 ms
	
	lcd_send_cmd(WAKE_UP);
	_delay_ms(10); // at least 5 ms
		
	lcd_send_cmd(MODE_8_BIT | MODE_2_LINE); // 8-bit mode, 2-line mode
	lcd_send_cmd(0x10); // set cursor
	lcd_send_cmd(DISPLAY_ON); // turn on entire display, cursor is on
	lcd_send_cmd(CLEAR_DISPLAY);
	lcd_send_cmd(INCREMENT_CURSOR); // cursor automatically increments after a character is written
	
}

void lcd_send_cmd(char cmd)
{
	data_to_port(cmd); //put data on PORT
	
	RS_LOW; // send command
	RW_LOW; // write operation
		
	// enable pulse
	E_HIGH;
		_delay_us(100); 
	E_LOW;
}

void lcd_send_data(char data)
{
	data_to_port(data); //put data on PORT

	RS_HIGH; // send data
	RW_LOW; // write operation

	// enable pulse
	E_HIGH;
		_delay_us(100);
	E_LOW;	
}


void data_to_port(char data)
{
	// DB0
	if ( (data >> 0)&(0x01) )
		PORTC |= (1<<DB0);
	else
		PORTC &= ~(1<<DB0);
	// DB1
	if ( (data >> 1)&(0x01) )
		PORTC |= (1<<DB1);
	else
		PORTC &= ~(1<<DB1);
	// DB2
	if ( (data >> 2)&(0x01) )
		PORTC |= (1<<DB2);
	else
		PORTC &= ~(1<<DB2);
	// DB3
	if ( (data >> 3)&(0x01) )
		PORTC |= (1<<DB3);
	else
		PORTC &= ~(1<<DB3);
	// DB4
	if ( (data >> 4)&(0x01) )
		PORTC |= (1<<DB4);
	else
		PORTC &= ~(1<<DB4);
	// DB5
	if ( (data >> 5)&(0x01) )
		PORTG |= (1<<DB5);
	else
		PORTG &= ~(1<<DB5);
	// DB6
	if ( (data >> 6)&(0x01) )
		PORTG |= (1<<DB6);
	else
		PORTG &= ~(1<<DB6);
	// DB7
	if ( (data >> 7)&(0x01) )
		PORTD |= (1<<DB7);
	else
		PORTD &= ~(1<<DB7);
}

void lcd_set_cursor(char row, char column)
{
	unsigned char address[2] = {LINE_1, LINE_2};
	lcd_send_cmd(address[row-1] + (column-1));
	_delay_us(100);
}

void lcd_print(char *str)
{
	unsigned char i =0;
	while(str[i] != 0)
	{
		lcd_send_data(str[i]);
		i++;
	}
}

void lcd_print_int(int number)
{
	int temp = number;
	int num_digit = 0;
	
	while (temp != 0)
	{
		temp = temp /10;
		num_digit++;
	}
	if (number<0) num_digit++;
	char char_array[MAX_LCD_STRING_SIZE];
	itoa(number, char_array,10);

	lcd_print(char_array);
}
