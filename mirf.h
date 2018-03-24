#ifndef _MIRF_H_
#define _MIRF_H_

#define F_CPU 16000000

#include <avr/io.h>
#include "nRF24L01.h"
#include "spi.h"
#include <util/delay.h>

// Mirf settings
#define mirf_CH 2
#define mirf_PAYLOAD 16
#define mirf_CONFIG ((1 << MASK_RX_DR) | (1 << EN_CRC) | (0 << CRCO))

// Pin definitions for chip select and chip enabled of the MiRF module
#define CE 4

// Definitions for selecting and enabling MiRF module
#define mirf_CSN_hi PORTB |= (1 << SS);  // PB0 is Chip Enable
#define mirf_CSN_lo PORTB &= ~(1 << SS);
#define mirf_CE_hi PORTD |= (1 << CE); // PD6 is Chip Select
#define mirf_CE_lo PORTD &= ~(1 << CE);

// Public standard functions
extern void mirf_init();
extern void mirf_config();
extern void mirf_send(char *value, char len);
extern void mirf_set_RADDR(char *adr);
extern void mirf_set_TADDR(char *adr);
extern char mirf_data_ready();
extern void mirf_get_data(char *data);

// Public extended functions
extern void mirf_config_register(char reg, char value);
extern void mirf_read_register(char reg, char *value, char len);
extern void mirf_write_register(char reg, char *value, char len);

#endif /* _MIRF_H_ */