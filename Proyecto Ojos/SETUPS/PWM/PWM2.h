/*
 * PWM2.h
 *
 * Created: 08/05/2024 04:57:36 p. m.
 *  Author: josei
 */ 

#ifndef PWM2_H_
#define PWM2_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define invertido 1
#define no_invertido 0

void resetPWM2(void);

float map2(float duty, float Imin, float Imax, float Omin, float Omax);

//Función para configurar PWM1 Modo Fast, Canal A
void initPWM2FastA(uint8_t inverted, uint16_t prescaler);

//Función para configurar PWM1 Modo Fast, Canal B
void initPWM2FastB(uint8_t inverted, uint16_t prescaler);

void updateDutyCycleA2(uint8_t duty);

void updateDutyCycleB2(uint8_t duty);

#endif /* PWM2_H_ */