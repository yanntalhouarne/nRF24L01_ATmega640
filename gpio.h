/*
 * external_interrupt.h
 *
 * Created: 6/10/2018 6:41:27 PM
 *  Author: Yann
 */ 




#ifndef EXTERNAL_INTERRUPT_H_
#define EXTERNAL_INTERRUPT_H_

#define FALLING 0
#define RISING 1
#define ANY 1

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5
#define PORT_G 6
#define PORT_H 7
#define PORT_J 8
#define PORT_K 9
#define PORT_L 10


void setup_ext_int(uint8_t INT, uint8_t trigger);

uint8_t digital_read(uint8_t PORT, uint8_t PIN);

#endif /* EXTERNAL_INTERRUPT_H_ */