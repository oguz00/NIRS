/*
 * NIRSV001.c
 *
 * Created: 20.09.2023 14:12:08
 * Author : Oguz
 */ 

//#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "NIRS_func.h"
#include <avr/interrupt.h>

// Global Variable
volatile uint8_t vect_cnt;
volatile uint16_t timer_ms_counter=0;
volatile uint16_t  count=0;
volatile uint8_t ch_number=1;

ISR(INT1_vect){
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
}
/*ISR(TIMER0_COMPA_vect){	
	if(timer_ms_counter>=40){
		timer_ms_counter=0;
		vect_cnt=0;
		disable_MUX();
	}
	else
	timer_ms_counter++;
}*/

int main(void)
{
	DDRA= 0b00111111;						// MUX Control Signal (PA0-PA3, Output), LED Signal (PA4-PA5, Output), ADC7 Signal Monitor (PA7, Input)
	DDRC= 0xFF;								// DAC Control Signal (Output, 8Bit word)
	DDRD= 0b11000000;						// Control Signal from Mainboard (PD3-1), PWM Lock-In-Signal (PD7, Output)
	
	uint8_t current_DAC_level;
	uint16_t Gain_controller=0x4000;	// 0b0100 0000 Gain:1
	uint16_t Channel_controller=0x4101; // 0b0100 0001 Select CH1
	uint8_t shutdown= 0x00;
	
	//Initialize External Control Interrupt INT1on PD3
	EICRA |= (1<<ISC10) | (1<<ISC11);			// Rising edges of INT1 generate an IRequest
	EIMSK |= (1<<INT1);							// Enable INT1
	
	PWM_init();
	//Timer_init();
	SPI_MasterInit();
	set_DAC_level(1,&current_DAC_level);
	_delay_ms(1000);
	
	vect_cnt=0;
	timer_ms_counter=0;
	_delay_ms(1);
	disable_MUX();
	sei();
	set_MUX(1);
	enable_MUX(1);
	SPI_MasterTransmit(Gain_controller);
	SPI_MasterTransmit(Channel_controller);
	while (1)
	{		
		/*// reset?
		if (PIND & 0x10){
			vect_cnt =0;
			disable_MUX();
		}
		*/
		disable_MUX();
	}
}

		





