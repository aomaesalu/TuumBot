/*
 * USART.h
 *
 */ 


#ifndef USART_H_
#define USART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__ // http://lists.gnu.org/archive/html/avr-gcc-list/2012-05/msg00030.html
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "pins.h"
#include "eeprom_id.h"
#include "macros.h"

extern volatile unsigned char usart_buf[16];
extern volatile uint8_t usart_i;
extern volatile bool usart_data_ready;

void usart_init();
void usart_write(unsigned char* data);


typedef void (*reply_func_t)(const char*);

void usart_reply_raw(const char *str);
void usart_reply(const char *str);



#endif /* USART_H_ */
