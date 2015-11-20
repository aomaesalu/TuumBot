/*
 * pins.h
 *
 */ 


#ifndef PINS_H_
#define PINS_H_

// RS485
#define RS485RX PD2
#define RS485RXEN PD7
#define RS485TX PD3
#define RS485TXEN PD6

// RGB leds
#define LED1R PF6
#define LED1G PF5
#define LED1B PF7
#define LED2R PF1
#define LED2G PF0
#define LED2B PF4
#define LEDS 0b11110011

// coil driver
#define CHARGE PD4
#define DONE PB4
#define KICK PD5

// BTN inputs
#define BTN1 PD0
#define BTN2 PD1

// dribbler PWM
#define EXTPWM PC6

// BALL input
#define BALL PE6

// IO inputs
#define IO1 PB7
#define IO2 PC7



#endif /* PINS_H_ */
