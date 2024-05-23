/*
 * PWM1.c
 *
 * Created: 18/04/2024 09:14:13 a. m.
 *  Author: josei
 */ 

#include "PWM1.h"

void resetPWM1(void){
	TCCR1A = 0;								//RESET
	TCCR1B = 0;								//RESET
}

float map1(float duty, float Imin, float Imax, float Omin, float Omax){
	return ((duty - Imin)*(Omax - Omin)/(Imax - Imin)) + Omin;
}

void initPWM1FastA(uint8_t inverted, uint16_t prescaler){
	DDRB |= (1<<DDB1);						//PIN B1 COMO SALIDA
	if(inverted){
		TCCR1A |= (1<<COM1A1)|(1<<COM1A0);	//OC1A INVERTIDO
	}else{
		TCCR1A |= (1<<COM1A1);				//OC1A NO INVERTIDO	
	}
	
	TCCR1A |= (1<<WGM10);					//MODO PWM FAST, 8 bits
	TCCR1B |= (1<<WGM12);					//MODO PWM FAST, 8 bits
	if(prescaler==1024){
	TCCR1B |= (1<<CS12)|(1<<CS10);		//PRESCALER DE 1024	
	}
}

void initPWM1FastB(uint8_t inverted, uint16_t prescaler){
	DDRB |= (1<<DDB2);						//PIN B2 COMO SALIDA
	if(inverted){
		TCCR1A |= (1<<COM1B1)|(1<<COM1B0);	//OC1A INVERTIDO
	}else{
		TCCR1A |= (1<<COM1B1);				//OC1A NO INVERTIDO
		
	}
	
	TCCR1A |= (1<<WGM10);					//MODO PWM FAST, 8 bits
	TCCR1B |= (1<<WGM12);					//MODO PWM FAST, 8 bits
	if(prescaler==1024){
		TCCR1B |= (1<<CS12)|(1<<CS10);		//PRESCALER DE 1024
	}
}

void updateDutyCycleA1(uint8_t duty){
	//duty = map1(duty, 0, 255, 900, 2000);
	OCR1A = duty;
	
}
void updateDutyCycleB1(uint8_t duty){
	//duty = map1(duty, 0, 255, 900, 1800);
	OCR1B = duty;
	
}