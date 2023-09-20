/*
 * NIRS_func.h
 *
 * Created: 20.09.2023 14:13:39
 *  Author: Oguz
 */ 



#ifndef NIRS_FUNC_H_
#define NIRS_FUNC_H_

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7
#define DDR_SPI DDRB


void set_DAC_level(uint8_t val, uint8_t *currDAClvl);
void enable_MUX();
void disable_MUX();
void set_MUX(uint8_t channelnumber);		// Channel number 0-7 (Y0-Y7)

void PWM_init();
void enable_LOCK_PWM();
void disable_LOCK_PWM();
void set_LOCK_PWM(uint8_t pwm_val);
void Timer_init();

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);






/***DEBUG LEDS***/
void LED1_on();
void LED2_on();
void LED1_off();
void LED2_off();








#endif /* NIRS_FUNC_H_ */