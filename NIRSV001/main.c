/*
 * NIRSV001.c
 *
 * Created: 20.09.2023 14:12:08
 * Author : Oguz
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "NIRS_func.h"
#include <avr/interrupt.h>

// Global Variable
volatile uint8_t vect_cnt;
volatile uint16_t timer_ms_counter;

ISR(TIMER0_COMPA_vect) {
	if (timer_ms_counter > 500){
		timer_ms_counter =0;
		vect_cnt =0;
		disable_MUX();
	}
	else
		timer_ms_counter++;
}

ISR(INT1_vect){
	uint8_t portnr = ((PIND>>1)&0x01);		// Bitmask: Checking a bit PD1 for choice channel.
	switch(vect_cnt){
		case 0:{
				disable_MUX();
				timer_ms_counter =0;
				if (portnr==0)
				{
					set_MUX(1);			// CH1 LED_IR 850nm
					enable_MUX();
				}
				vect_cnt=1;
				break;
		}
		case 1:{
				disable_MUX();
				timer_ms_counter =0;
				if (portnr==1)
				{
					set_MUX(2);			// CH2 LED_RED 750nm
					enable_MUX();
				}
				vect_cnt =0;
				break;
		}
	}
}

int main(void)
{
	DDRA= 0b00111111;						// MUX Control Signal (PA0-PA3, Output), LED Signal (PA4-PA5, Output), ADC7 Signal Monitor (PA7, Input)
	DDRB= 0b11100000;						// PGA Control Signal (PB0-PB4, Output)
	DDRC= 0xFF;								// DAC Control Signal (Output, 8Bit word)
	DDRD= 0b11000000;						// Control Signal from Mainboard (PD3-1), PWM Lock-In-Signal (PD7, Output), PGA Error Flag (PD0)
	//DDRE= 0b00000000;						// SPI MISO PIN:PE2 INPUT others user defines
	uint8_t current_DAC_level;
	uint8_t PGA_gain;
	
	PWM_init();
	//Timer_init();
	enable_LOCK_PWM();
	_delay_ms(1000);
	
	set_DAC_level(1,&current_DAC_level);
	/* Replace with your application code */
	
	enable_MUX();
	_delay_ms(10);
	//set_MUX(1);
	PORTD|=(1<<7);
	_delay_ms(1000);
	sei();
	while (1)
	{	
		set_MUX(1);
		_delay_ms(20);
		set_MUX(2);
		_delay_ms(20);
	}
}



