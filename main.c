/*
 * automation.c
 *
 * Created: 24-04-2025 11:30:33
 * Author : MUHAMMED SALEEKH K
 */ 

#include <avr/io.h>
#include <util/delay.h>

// functions for LCD
void instruction(char a);
void data(char a);
void pointer(const char *ptr);

void main(){
	DDRA= 0xff;// LCD data lines
	DDRB = 0xf0; // 2 switches as input and en,rs for LCD
	DDRC = 0xff; // for motor and leds 
	
	
	PORTB = 0xf0;
	PORTC = 0x00;
	
	instruction(0x38);
	instruction(0x0f);
	
	while(1)
	{
		PORTC=0x00;
		// code for AC ( temperature above 20°C )
		if ((PINB & 0x01)==0) { 
			_delay_ms(50); // debounce delay
			if ((PINB & 0x01)==0) { 
				PORTC = 0x15; // LED1 ON , motor forward (AC ON)
				
				// LCD 
				instruction(0x01);
				instruction(0x80);
				pointer("TEMP > 20°C");
				instruction(0xC0);
				pointer("AC ON");
				while((PINB & 0x01)==0); // wait until button released
			}else
			{
				PORTC=0x00;
			}
		}
		
		// code for HEATER (temperature below 20°C)

		if ((PINB & 0x04)==0) { 
			_delay_ms(50); // debounce delay
			if ((PINB & 0x04)==0) { 
				PORTC = 0x26; // LED2 ON , motor reverse (HEATER ON)
				
				// LCD
				instruction(0x01);
				instruction(0x80);
				pointer("TEMP < 20°C");
				instruction(0xc0);
				pointer("HEATER ON");
				while ((PINB & 0x04 )==0 ); // wait until button released
			}
			else
			{
				PORTC=0x00;
			}
		}
	}
	
}

void instruction(char a)
{
	PORTA = a;
	PORTB= 0x20;
	_delay_ms(50);
	PORTB=0x00;
}

void data(char a)
{
	PORTA =a;
	PORTB= 0x30;
	_delay_ms(50);
	PORTB=0x10;
}

void pointer(const char *ptr)
{
	while(*ptr)
	{
		data(*ptr);
		ptr++;
	}
}
