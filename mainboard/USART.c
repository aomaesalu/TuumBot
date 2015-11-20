ļ»æ/*
 * USART.c
 *
 */ 

#include "USART.h"

volatile unsigned char usart_buf[16];
volatile uint8_t usart_i = 0;
volatile bool usart_data_ready = false;
char reply[16];

bool iseol(char ch)
{
	return ch == '\r' || ch == '\n';
}

bool isdata(char ch)
{
	return ch >= ' ' && ch <= '~';
}

void usart_init()
{
	bit_set(DDRD, BIT(RS485TX) | BIT(RS485TXEN) | BIT(RS485RXEN));
	bit_clear(DDRD, BIT(RS485RX));

	bit_set(PORTD, BIT(RS485TXEN)); // enable Tx
	bit_clear(PORTD, BIT(RS485RXEN)); // enable Rx (inverted)

	UBRR1 = 51; // 19200, 8
	UCSR1B = BIT(RXEN1) | BIT(TXEN1) | BIT(RXCIE1); // enable Tx, Rx
	UCSR1C = BIT(USBS1) | BITS(0b11, UCSZ10); // 2-bit stop, 8-bit character
}

void usart_write(unsigned char* data)
{
	while (*data)
	{
		while (!bit_get(UCSR1A, BIT(UDRE1)));
		UDR1 = *data;
		data++;
	}
	_delay_ms(5);
}

void usart_reply_raw(const char *str)
{
	uint8_t id = eeprom_read_byte(EEPROM_ID);
	sprintf(reply, "%d:%s\n", id, str);
	usart_write(reply);
}

void usart_reply(const char *str)
{
	uint8_t id = eeprom_read_byte(EEPROM_ID);
	sprintf(reply, "<%d:%s>\n", id, str);
	usart_write(reply);
}

ISR(USART1_RX_vect)
{
	bit_flip(PORTF, BIT(LED2G));

	unsigned char ch = UDR1;

	if (iseol(ch))
	{
		usart_buf[usart_i] = '\0';
		usart_i = 0;
		usart_data_ready = true;
		bit_flip(PORTF, BIT(LED1G));
	}
	else if (isdata(ch))
	{
		usart_buf[usart_i++] = ch;
	}
}
