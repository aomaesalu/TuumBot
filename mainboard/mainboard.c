/*
 * mainboard.c
 *
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "pins.h"
#include "USART.h"
#include "macros.h"


//KICKTIME
#define KICKTIME 5

//FAILSAFE
#define FAILSAFE 100


bool failsafe = true;
volatile uint8_t failsafe_counter = 0;

char response[16];

bool streq(const char* str1, const char* str2)
{
	return strcmp(str1, str2) == 0;
}

bool strpref(const char *str, const char *pre)
{
	return strncmp(str, pre, strlen(pre)) == 0;
}

void parse_and_execute_command(char *buf)
{
	bit_flip(PORTF, BIT(LED2R));

	char *command;
	int16_t par1;
	command = buf;

	reply_func_t reply_func, reply_raw_func;
	reply_func = usart_reply;
	reply_raw_func = usart_reply_raw;
	
	uint8_t id = 0;//eeprom_read_byte(EEPROM_ID);
	par1 = atoi(command);
	if(par1 != id)
	{
		return;
	}
	else
	{
		while (*command != ':') command++;
		command++;
	}

	if (strpref(command, "id"))
	{
		// set ID
		par1 = atoi(command + 2);
		eeprom_update_byte(EEPROM_ID, par1);
	}
	else if (streq(command, "?"))
	{
		// get ID
		par1 = eeprom_read_byte(EEPROM_ID);
		sprintf(response, "id:%d", par1);
		reply_func(response);
	}
	else if (strpref(command, "k"))
	{
		// kick
		if (streq(command, "k"))
		par1 = KICKTIME;
		else
		par1 = atoi(command + 1);

		bit_clear(PORTD, BIT(CHARGE));

		bit_set(PORTD, BIT(KICK));
		_delay_ms(par1);
		bit_clear(PORTD, BIT(KICK));
	}
	else if (streq(command, "c"))
	{
		// charge
		bit_clear(PORTD, BIT(KICK));

		bit_set(PORTD, BIT(CHARGE));
		
		bit_clear(PORTF, BIT(LED2B));
	}
	else if (streq(command, "bl"))
	{
		// ball detector input
		sprintf(response, "bl:%d", bit_get(PINE, BIT(BALL)) != 0);
		reply_func(response);
	}
	else if (strpref(command, "b"))
	{
		// get button state
		par1 = atoi(command + 1);

		int state = -1;
		switch (par1)
		{
			case 1:
			state = bit_get(PIND, BIT(BTN1)) == 0;
			break;

			case 2:
			state = bit_get(PIND, BIT(BTN2)) == 0;
			break;
		}
		sprintf(response, "b%d:%d", par1, state);
		reply_func(response);
	}
	else if (strpref(command, "io"))
	{
		// get IO state
		par1 = atoi(command + 2);

		int state = -1;
		switch (par1)
		{
			case 1:
			state = bit_get(PINB, BIT(IO1)) != 0;
			break;

			case 2:
			state = bit_get(PINC, BIT(IO2)) != 0;
			break;
		}
		sprintf(response, "io%d:%d", par1, state);
		reply_func(response);
	}
	else if (strpref(command, "dr"))
	{
		// dribbler motor
		par1 = atoi(command + 2);
		OCR3AL = par1;
		
		bit_clear(PORTF, BIT(LED1G));
	}
	else if (strpref(command, "fs"))
	{
		// set failsafe
		par1 = atoi(command + 2);
		failsafe = par1;
	}
	else if (streq(command, "p"))
	{
		// ping, keep failsafe from triggering
		failsafe_counter = 0;
	}
	else
	{
		reply_raw_func(command);
	}
}

ISR(PCINT0_vect)
{
	if (bit_get(PINB, BIT(DONE)) == 0) // DONE changed to LOW
	{
		bit_clear(PORTD, BIT(CHARGE));
	}
}

int main(void)
{
	// clock prescaler
	CLKPR = BIT(CLKPCE); // enable prescaler change
	CLKPR = BITS(0b0000, CLKPS0); // divider 1 instead of default 8
	
	// disable JTAG - control F port
	MCUCR = BIT(JTD);
	MCUCR = BIT(JTD);

	// LED outputs
	bit_set(DDRF, LEDS);
	bit_set(PORTF, LEDS);


	// coil outputs
	bit_set(DDRD, BIT(KICK));
	bit_set(DDRD, BIT(CHARGE));
	bit_clear(PORTD, BIT(KICK));
	bit_clear(PORTD, BIT(CHARGE));

	// coil input
	bit_clear(DDRB, BIT(DONE));
	bit_set(PCICR, BIT(PCIE0));
	bit_set(PCMSK0, BIT(PCINT4));

	// button inputs
	bit_clear(DDRD, BIT(BTN1) | BIT(BTN2));

	// ball detector input
	bit_clear(DDRE, BIT(BALL));
	EICRB = BITS(0b01, ISC60);
	bit_set(EIMSK, BIT(INT6));

	// IO inputs
	bit_clear(DDRB, BIT(IO1));
	bit_clear(DDRC, BIT(IO2));

	// initialize usart
	usart_init();
	_delay_ms(1000);
	
	bit_set(DDRC, BIT(EXTPWM)); // ext PWM pin as output
	TCCR3A = BITS(0b10, COM3A0) | BITS(0b01, WGM30); // fast PWM, 8-bit (mode 5)
	TCCR3B = BITS(0b01, WGM32) | BITS(0b100, CS30); // divider 256
	OCR3AL = 0;
	
	sei();

	uint8_t n;
	char buf[16];
	
    while(1)
    {
        if (failsafe && (failsafe_counter >= FAILSAFE))
		{
			 // failsafe
	        bit_clear(PORTD, BIT(CHARGE));
	        bit_clear(PORTD, BIT(KICK));
	        OCR3AL = 0;
			failsafe_counter = 0;
		}
		else if (usart_data_ready)
		{
			bit_flip(PORTF, BIT(LED1B));
			parse_and_execute_command(usart_buf);
			usart_data_ready = false;
		}			
	}
}
