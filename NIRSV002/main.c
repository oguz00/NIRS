/*
 * NIRSV002.c
 *
 * Created: 4.10.2023 09:39:19
 * Author : Oguz
 */ 

#include <avr/io.h>
#include "NIRS_func.h"
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

// Global Variable
volatile uint8_t vect_cnt;
volatile uint16_t timer_ms_counter=0;
volatile uint16_t  count=0;
volatile uint8_t ch_number=1;


ISR(INT1_vect){
	/*
	switch(vect_cnt){
		case 0:{
			disable_MUX();
			timer_ms_counter =0;
			set_MUX(1);				// CH1 LED_IR 850nm
			enable_MUX();
			vect_cnt=1;
			break;
		}
		case 1:{
			disable_MUX();
			timer_ms_counter =0;
			set_MUX(2);				// CH2 LED_RED 750nm
			enable_MUX();
			vect_cnt =0;
			break;
		}
		
	}*/
}

ISR(TIMER0_COMPA_vect){
	/*if(timer_ms_counter>=4000){
		timer_ms_counter=0;
		vect_cnt=0;																																																						
		//disable_MUX();
		LED_toggle(1);
	}
	else
	timer_ms_counter++;*/
	}
int main(void)
{
	uint8_t  current_DAC_level;
	uint16_t Gain_controller=0x4007;		// Instruction Byte: 0b0100 0000 / Gain:1	: 0b0100 0000
	uint16_t Channel_controller=0x4101;		// Instruction Byte: 0b0100 0000 / Channel-1: 0b0100 0001
	//uint16_t shutdown= 0x1000;

	DDRA= 0b00111111;						// MUX Control Signal (PA0-PA3, Output), LED Signal (PA4-PA5, Output), ADC7 Signal Monitor (PA7, Input)
	DDRC= 0xFF;								// DAC Control Signal (Output, 8Bit word)
	DDRD= 0b11000000;						// Control Signal from Mainboard (PD3-1), PWM Lock-In-Signal (PD7, Output)
	
	//Initialize External Control Interrupt INT1on PD3
	EICRA |= (1<<ISC10) | (1<<ISC11);			// Rising edges of INT1 generate an IRequest
	EIMSK |= (1<<INT1);							// Enable INT1
	
	//PWM_init();   
	//Timer_init();
	SPI_MasterInit();
	set_DAC_level(1,&current_DAC_level);
	vect_cnt=0;
	timer_ms_counter=0;
	_delay_ms(1000);
	disable_MUX();
	sei();
	_delay_ms(2);
	
	
    while (1) 
    {
		SPI_MasterTransmit(Gain_controller);
		SPI_MasterTransmit(Channel_controller);
		LED_toggle(4);
		_delay_ms(1000);
    }
}

