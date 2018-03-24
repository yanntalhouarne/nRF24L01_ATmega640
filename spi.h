/*
 * spi.h
 *
 * Created: 10/16/2017 7:12:34 PM
 *  Author: talho_000
 */

#include <avr/io.h>

#ifndef SPI_H_
#define SPI_H_

//SPI port and pins
#define DDR_SPI DDRB
#define PORTD_SPI PORTB
#define MISO 3 // PB3
#define MOSI 2 // PB2
#define SCK 1  // PB1
#define SS 0   // PB0



// setup
	void spi_master_initialize();
	void spi_slave_initialize();
	
// char (one byte)
	void spi_send_char(char data); // send a single char (one byte)
	char spi_exchange_char(char data); // exchange single char (one byte)
	
	
// array of char (multiple bytes)
	void spi_send_bytes(char *pdata,char bytes); // send array of char of size "bytes"
	void spi_exchange_bytes(char *mosi, char *miso, char bytes); // exchange array of char of size "bytes"
	
// int (single int)
	void spi_send_int(unsigned int mosi); // send a int value LSB first
	unsigned int spi_exchange_int(unsigned int mosi); // exchange int value

#endif /* SPI_H_ */