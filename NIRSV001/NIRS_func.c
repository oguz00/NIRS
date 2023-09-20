/*
 * NIRS_func.c
 *
 * Created: 20.09.2023 14:13:22
 *  Author: Oguz
 */ 
#include "NIRS_func.h"


/***DEBUG LEDS***/
void LED1_on(){      //Led Red
	PORTA |= (1<<4);
}
void LED2_on(){		////Led Green
	PORTA |= (1<<5);
}
void LED1_off(){
	PORTA &= ~(1<<4);
}
void LED2_off(){
	PORTA &= ~(1<<5);
}

void set_DAC_level(uint8_t val, uint8_t *currDAClvl){		// sets DAC-Word (4 steps: 1-4)
	switch (val){
		case 0:{											// reduce DAClvl by one step
			if (*currDAClvl == 4){
				PORTC = 0b10000000;
				*currDAClvl =3;
			}
			if (*currDAClvl == 3){
				PORTC = 0b01111111;
				*currDAClvl =2;
			}
			if (*currDAClvl == 2){
				PORTC = 0b00000000;
				*currDAClvl =1;
			}
			if (*currDAClvl == 1){
				PORTC = 0b00000000;
				*currDAClvl =1;
			}
			break;
		}
		case 1:{
			PORTC = 0b00000000;		// 440mV
			*currDAClvl =1;
			break;
		}
		case 2:{
			PORTC = 0b01111111;		// 960mV
			*currDAClvl =2;
			break;
		}
		case 3:{
			PORTC = 0b10000000;		// 1.44V
			*currDAClvl =3;
			// 				LED2_on();
			// 				LED1_on();
			break;

		}
		case 4:{
			PORTC = 0b11111111;		// 1.96V
			*currDAClvl =4;
			break;
		}
		default: break;
	}
}
void enable_MUX(){				// Enables MUX (E)
	PORTA &= ~(1<<PINA0);
}

void disable_MUX(){
	PORTA |= (1<<PINA0);
}


// S1: PA3
// S2: PA2
// S3: PA1
// E : PA0

//  E	S3	S2	S1	OUT
//  0	0	0	1	Y1
//  0	0	1	0	Y2
//  0	0	1	1	Y3

void set_MUX(uint8_t channelnumber){					// Sets MUX output channel
	switch (channelnumber){
		case 1:{
			PORTA &= ~((1<<PINA2) | (1<<PINA1));		// Y1: 0b001
			PORTA |= (1<<PINA3);
			LED1_on();
			break;
		}
		case 2:{
			PORTA &= ~((1<<PINA3) | (1<<PINA1));		// Y2: 0b010
			PORTA |= (1<<PINA2);
			LED2_on();
			break;
		}
		case 3:{
			PORTA &= ~(1<<PINA1);					// Y1+Y2: 0b011
			PORTA |= (1<<PINA2)|(1<<PINA3);
			LED1_on();
			LED2_on();
			break;
		}
		
		
		default:	break;
	}
}
/***PWM FUNCS***/
void PWM_init(){
	//Initialize Lock-In-PWM-Signal
	PRR0 &= ~(1<<PRTIM2);				// Power Reduction Register: Timer2 Enable
	TCCR2A=(1<<COM2A0) | (1<< WGM20);	// WGM22:0 = 101 PWM, Phase Correct Mode; COM2A0=1 Toggle on Compare Match
	TCCR2B=(1<<CS21) | (1<<WGM22);		// 1/8 prescaling,
	set_LOCK_PWM(62);				// PWM: 10,08 kHz, 50% Duty Cycle
	//set_LOCK_PWM(250);				// PWM: 2,50 kHz
	// set_LOCK_PWM(200);					// PWM: 3,125 kHz
}
void enable_LOCK_PWM(){
	TCCR2A |= (1<<COM2A0);				// Phase correct PWM Mode, Toggle OC1A on Compare Match
}

void disable_LOCK_PWM(){
	TCCR2A &= ~(1<<COM2A0);				// Non PWM Mode, Normal port operation, OC1A disconnected
}

void set_LOCK_PWM(uint8_t pwm_val){
	OCR2A = pwm_val;
}
/***TIMER FUNCS***/
void Timer_init() {
	OCR0A = 195;						// Output Compare Match with prescaler 1/1024: Every ~10ms
	TCCR0A |= (1<<WGM01);				// Clear Timer on Compare match (CTC), Mode 2
	TCCR0B |= (1<<CS02) | (1<<CS00);	// Prescaler 1/1024
	TIMSK0 |= (1<<OCIE0A);				// Timer/Counter1 Output Compare Interrupt Request
}

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR0 = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR0 = cData;
	/* Wait for transmission complete */
	while(!(SPSR0 & (1<<SPIF)))
	;
}
