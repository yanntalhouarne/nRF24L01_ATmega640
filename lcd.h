/*
 * lcd.h
 *
 * Created: 1/27/2018 4:16:07 PM
 *  Author: Yann
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_LCD_STRING_SIZE 16

// pins
#define RS  2 // PB0
#define RW  1 // PB1
#define E   0 // PB2
#define DB0 3 // PB3
#define DB1 4 // PD4
#define DB2 5 // PD5
#define DB3 6 // PC6
#define DB4 7 // PC7
#define DB5 0 // PA0
#define DB6 1 // PA1
#define DB7 7 // PA2

#define RS_HIGH PORTC |= (1<<RS);
#define RS_LOW PORTC &= ~(1<<RS);
#define RW_HIGH PORTC |= (1<<RW);
#define RW_LOW PORTC &= ~(1<<RW);
#define E_HIGH PORTC  |= (1<< E);
#define E_LOW PORTC &= ~(1<< E);

// commands

// initialize
#define WAKE_UP 0x30
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02

// Entry mode
#define INCREMENT_CURSOR 0x06
#define DISPLAY_SHIFT 0x05

// Display ON/OFF control
#define DISPLAY_ON 0x0C
#define CURSOR_ON 0x02
#define BLINK_CURSOR 0x01

#define MODE_8_BIT 0x30
#define MODE_2_LINE 0x28
#define SMALL_FONT 0x24

#define SET_CGRAM 0x40
#define LINE_1 0x80
#define LINE_2 0xC0

void setup_lcd();
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void data_to_port(char data);
void lcd_set_cursor(char row, char column);
void lcd_print(char *str);
void lcd_print_int(int number);



#endif /* LCD_H_ */