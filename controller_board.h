/*
 * controller_board.h
 *
 * Created: 2/1/2018 11:13:35 AM
 *  Author: Yann
 */ 


#ifndef CONTROLLER_BOARD_H_
#define CONTROLLER_BOARD_H_

#include <avr/io.h>

// LEDs
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 4
#define LED6 5

 // LEDs ON
#define LED1_ON PORTL |= (1<<LED1);
#define LED2_ON PORTL |= (1<<LED2);
#define LED3_ON PORTL |= (1<<LED3);
#define LED4_ON PORTL |= (1<<LED4);
#define LED5_ON PORTL |= (1<<LED5);
#define LED6_ON PORTL |= (1<<LED6);
 // LEDs OFF
#define LED1_OFF PORTL &= ~(1<<LED1);
#define LED2_OFF PORTL &= ~(1<<LED2);
#define LED3_OFF PORTL &= ~(1<<LED3);
#define LED4_OFF PORTL &= ~(1<<LED4);
#define LED5_OFF PORTL &= ~(1<<LED5);
#define LED6_OFF PORTL &= ~(1<<LED6);
 // toggle LEDs
 #define TOGGLE_LED1 PORTL ^= (1<<LED1);
 #define TOGGLE_LED2 PORTL ^= (1<<LED2);
 #define TOGGLE_LED3 PORTL ^= (1<<LED3);
 #define TOGGLE_LED4 PORTL ^= (1<<LED4);
 #define TOGGLE_LED5 PORTL ^= (1<<LED5);
 #define TOGGLE_LED6 PORTL ^= (1<<LED6);

// pots
#define LIFT_SPEED_POT 6 // ADC6
#define LIFT_ACC_POT 7 // ADC7
#define SWING_SPEED_POT 4 // ADC4
#define SWING_ACC_POT 5 // ADC5

// encoders
#define ENC_1_A 0 // PD0
#define ENC_2_A 1 // PD1
#define ENC_1_B 6 // PL6
#define ENC_2_B 7 // PL7
#define ENC_1_SWITCH 4 // PE4
#define ENC_2_SWITCH 5 // PE5

// current sense
#define CURRENT_SENSE_PIN 0  // ADC8 (PK0)
#define CURRENT_SENSE_CHANNEL 8
#define CURRENT_SCALING_FACTOR .24438


void setup_LEDs(); // set LED's as outputs
void LED_check(); // sanity check
void setup_ports();

#endif /* CONTROLLER_BOARD_H_ */