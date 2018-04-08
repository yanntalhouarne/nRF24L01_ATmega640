/*
 * nRF24L01_Atmega324PB.c
 *
 * Created: 3/17/2018 11:41:05 PM
 * Author : Yann
 */ 
#define F_CPU 16000000
#define LOOP_DELAY 30

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

char buffer[mirf_PAYLOAD] = {0,0};
int mtr_cmd = 0;

uint8_t status = 0;
int8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
int8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

int main(void)
{
	setup_ports();
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
	
	//mirf_config_register(EN_AA, (1<<0));
	//	mirf_config_register(EN_AA, (1<<1));
	
	mirf_config();
	
	 /* Set the device addresses */
	mirf_set_TADDR(tx_address);
	mirf_set_RADDR(rx_address);
	
	
	_delay_ms(10);
	
	print_0("System initialized...;");

    while (1) 
    {
		// to UART
		//println_0("Waiting for data...;");
		// to LCD
		//lcd_send_cmd(CLEAR_DISPLAY);
		//_delay_ms(3);
		//lcd_set_cursor(1,1);
		//lcd_print("Waiting for data");
		//_delay_ms(3);
		
		mtr_cmd = analog_get_average(POT1, 5);
		mtr_cmd /= ADC_SCALING;
		
		buffer[0] = mtr_cmd;
		
		//println_int_0(mtr_cmd);
		
		TOGGLE_LED1;
		mirf_send(buffer, mirf_PAYLOAD);
		_delay_us(10);
		while (!mirf_data_sent());
		TOGGLE_LED1
		//mirf_config_register(STATUS, (1 << TX_DS) | (1 << MAX_RT)); // Reset status register
		//lcd_send_cmd(CLEAR_DISPLAY);
		//mirf_config_register(STATUS, (1 << RX_DR) | (1 << MAX_RT)); // Reset status register
		//LED1_ON; // turn on LED when data is received
		
		//mirf_get_data(buffer);
		
		//println_int_0(buffer[0]);
		//lcd_send_cmd(CLEAR_DISPLAY);
		//lcd_set_cursor(1,1);
		//lcd_print_int(buffer[0]);
		
		
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
		//lcd_send_cmd(CLEAR_DISPLAY);
		//_delay_ms(3);
		//lcd_set_cursor(1,1);
		//lcd_print("Data received:");
		//_delay_ms(3);
		//lcd_set_cursor(2,1);
		//lcd_print_int(buffer[0]);
		//_delay_ms(3);
		//lcd_print(", ");
		//_delay_ms(3);
		//lcd_print_int(buffer[1]);
		//_delay_ms(3);
		
	//	_delay_ms(5000);
		
		//buffer[0] = 'K';
		
		//lcd_send_cmd(CLEAR_DISPLAY);
		//_delay_ms(3);
		//lcd_set_cursor(1,1);
		//_delay_ms(3);
		//lcd_print("Sending data...");
		//_delay_us(10);
		//_delay_us(10);
		//lcd_send_cmd(CLEAR_DISPLAY);
		///lcd_set_cursor(1,1);
		//lcd_print("waiting on TX");
		//mirf_send(buffer, mirf_PAYLOAD);
		//while (!mirf_data_sent());
		//lcd_send_cmd(CLEAR_DISPLAY);
		//mirf_config_register(STATUS, (1 << TX_DS) | (1 << MAX_RT)); // Reset status register
		//TOGGLE_LED1;
		//lcd_set_cursor(2,1);
		//_delay_ms(3);
		//lcd_print("Data sent.");
		
		//_delay_ms(500);
		
		//while(1);
		
		_delay_ms(LOOP_DELAY);
    }
}
