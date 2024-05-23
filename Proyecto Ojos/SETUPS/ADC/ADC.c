/*
 * ADC.c
 *
 * Created: 10/05/2024 11:22:50 a. m.
 *  Author: josei
 */ 

#include "ADC.h"

void initADC(void){
	ADMUX = 0;											//REINICIAR ADMUX
	ADMUX |= (1<<ADLAR);								//JUSTIFICACI�N A LA IZQUIERDA
	ADMUX |= (1<<REFS0);								//REFERENCIA AVCC
	
	DIDR0 |= (1<<ADC4D);								//DESHABILITAR PIN DIGITAL 4
	DIDR0 |= (1<<ADC5D);								//DESHABILITAR PIN DIGITAL 5
	
	ADCSRA = 0;											//REINICIAR ADCSRA
	//ADCSRA |= (1<<ADIE);								//HABILITaR INTERRUPCIONES DE ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		//PRESCALER DE 128 ----> 125kHz
	//ADCSRA |= (1<<ADEN);								//HABILITANDO EL ADC
}

uint8_t ADC_CHANEL_SELECT(uint8_t chanel){
	ADMUX = (ADMUX & 0xF0)|chanel;						//SEPARAR LOS VALORES DEL ADMUX DEL RESTO DEL REGISTRO
	ADCSRA |= (1<<ADEN);								//HABILITANDO EL ADC
	ADCSRA |= (1<<ADSC);								//START CONVERSION
	while ((ADCSRA)&(1<<ADSC));							//REPETIR HASTA QUE LA CONVERSION TERMINE
	return ADCH;										//RETORNAR EL ADCH
}