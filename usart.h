/*
 * usart.h
 *
 * Created: 10/12/2017 1:20:24 PM
 *  Author: talho_000
 */ 

#include <avr/io.h>
#define MAX_STRING_SIZE 64 // maximum string size for TX and RX is 64 bytes

#ifndef USART_H_
#define USART_H_

#define BR_9600 103
#define BR_38400 25
#define BR_76800 12

// globals for RX interrupts string
unsigned char rcv_string[MAX_STRING_SIZE];

void usart0_send_char(unsigned char data);
unsigned char usart0_receive_char();
void setup_usart0(unsigned char BR);

void usart1_send_char(unsigned char data);
unsigned char usart1_receive_char();
void usart1_receive_string();
void setup_usart1(unsigned char BR);

#endif /* USART_H_ */