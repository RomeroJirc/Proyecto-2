/*
 * PWM0.h
 *
 * Created: 14/05/2024 04:52:39 p. m.
 *  Author: josei
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define invertido 1
#define no_invertido 0

void resetPWM0(void);

float map0(float duty, float Imin, float Imax, float Omin, float Omax);

//Función para configurar PWM1 Modo Fast, Canal A
void initPWM0FastA(uint8_t inverted, uint16_t prescaler);

//Función para configurar PWM1 Modo Fast, Canal B
void initPWM0FastB(uint8_t inverted, uint16_t prescaler);

void updateDutyCycleA0(uint8_t duty);

void updateDutyCycleB0(uint8_t duty);



#endif /* PWM0_H_ */