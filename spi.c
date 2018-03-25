#include "spi.h"

void spi_master_initialize()
{
	DDR_SPI |= (1 << MOSI) | (1 << SCK) | (1 << SS) ;  // MOSI_0 is output
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, set clock rate fck/16, SPI_MODE0, MSB first
}
void spi_slave_initialize()
{
	DDRB |= (1 << MISO);	// MSIO_0 is output
	
	SPCR = (1 << SPE); // Enable SPI, Slave
}

void spi_send_char(char data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)))
	; // wait for transmission to complete
}


char spi_exchange_char(char data)
{
	SPDR = data; // start transmission
	
	while (!(SPSR & (1 << SPIF)))
	; // wait for transmission complete
	
	return SPDR;
}


void spi_send_bytes(char *pdata, char bytes)
{
	int i;

	for (i = 0; i < bytes; i++)
	{
		SPDR = pdata[i]; // start transmission
		while (!(SPSR & (1 << SPIF)))
		; // wait for transmission complete
	}
}

void spi_exchange_bytes(char *mosi, char *miso, char bytes)
{
	int i;

	for (i = 0; i < bytes; i++)
	{
		SPDR = mosi[i]; // start transmission

		while (!(SPSR & (1 << SPIF)))
		; // wait for transmission complete
		miso[i] = SPDR;
	}
}

unsigned int spi_exchange_int(unsigned int mosi) // not tested
{
	unsigned int miso = 0;
	
	// first send MSB
	//while (!(SPSR0 & (1 << SPIF)));
	SPDR = (char)(mosi >> 8); // start transmission by sending MSB of MOSI

	while (!(SPSR & (1 << SPIF))); // wait for transmission complete
	miso = (0xFF00) & (SPDR << 8); // store MSB of MISO

	//while (!(SPSR0 & (1 << SPIF)));
	SPDR = (char)mosi; // send LSB of MOSI

	while (!(SPSR & (1 << SPIF)));// wait for transmission complete
	miso |= (0x00FF)&(SPDR); // store LSB of MISO

	return miso;
}

void spi_send_int(unsigned int mosi) // not tested
{
	while (!(SPSR & (1 << SPIF)));
	SPDR = (char)(mosi >> 8); // start transmission by sending MSB of MOSI

	while (!(SPSR & (1 << SPIF)));
	SPDR = (char)mosi; // send LSB of MOSI
}

