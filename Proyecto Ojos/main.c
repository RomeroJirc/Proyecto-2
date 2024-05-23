/*
 * Proyecto Ojos.c
 *
 * Created: 05/05/2024 07:30:18 p. m.
 * Author : josei
 */ 

//LIBRERIAS
#include <avr/io.h>
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SETUPS/PWM/PWM1.h"
#include "SETUPS/PWM/PWM2.h"
#include "SETUPS/PWM/PWM0.h"
#include "SETUPS/ADC/ADC.h"
#include "SETUPS/UART/UART.h"
#include "SETUPS/EEPROM/EEPROM.h"

//VARIABLES
uint8_t Modo = 0;
uint8_t duty1 = 0;
uint8_t duty2 = 0;
uint8_t duty3 = 0;
uint8_t duty4 = 0;
uint8_t servo0 = 0;
uint8_t canal = 1;
uint8_t valorUART = 0;
float payload = 0;

//PROTOTIPOS
//void initADC(void);
void initTimer0(void);
void initPCint1(void);
void initPCint0(void);
void ChangeMode(void);
void comprobarque(char valorUART);
void UARTCHANEL(char valorUART);
void UARTBOTON(char valorUART);
void AccionBoton1(void);
void AccionBoton2(void);


int main(void)
{
		cli();															//DESHABILITAR INTERRUPCIONES
		//BOTONES
		DDRC &= ~((1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3));				//PINES C0, C1, C2 Y C3 COMO ENTRADAS
		PORTC |= ((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));		//PULLUPS
		DDRB &= ~((1<<DDB0)|(1<<DDB4));									//PINES B4 Y B0 COMO ENTRADAS
		PORTB |= ((1<<PORTB0)|(1<<PORTB4));								//PULLUPS
		
		DDRB |= (1<<DDB5);
		
		initPCint1();
		initPCint0();
		//ADC
		initADC();									//LLAMAR A LA INICIALIZACIÓN DEL ADC
		initUART9600();
		
		//PWM1 Y PWM2
		resetPWM1();
		resetPWM0();
		resetPWM2();
		initPWM2FastB(no_invertido, 1024);				//CONFIGURANDO CON LIBRERIAS EL PWM1
		initPWM0FastB(no_invertido, 1024);				//CONFIGURANDO CON LIBRERIAS EL PWM1
		initPWM2FastA(no_invertido, 1024);				//CONFIGURANDO CON LIBRERIAS EL PWM2
		initPWM0FastA(no_invertido, 1024);				//CONFIGURANDO CON LIBRERIAS EL PWM2	
		
		DDRD |=((1<<DDD4)|(1<<DDD7)|(1<<DDD2));			//PIN D4, D3 y D2 COMO SALIDAS
		sei();											//HABILITAR INTERRUPCIONES
		
		
	//LOOP PRINCIPAL
	//COMPROBACION DE MODO
    while (1) 
    {
		if(Modo==0)
		{
			PORTD &= ~(1<<PORTD7);					//ENCENDER LED
			PORTD |= (1<<PORTD2);					//ENCENDER LED

			_delay_ms(10);
			duty1 = ADC_CHANEL_SELECT(5);
			updateDutyCycleB0(duty1);		// SALIDA D5 O PD5
			_delay_ms(10);
			duty2 = ADC_CHANEL_SELECT(6);
			updateDutyCycleB2(duty2);		// SALIDA D3 O PD3
			_delay_ms(10);
			duty3 = ADC_CHANEL_SELECT(7);
			updateDutyCycleA2(duty3);		// SALIDA D11 O PB3
			_delay_ms(10);
			duty4 = ADC_CHANEL_SELECT(4);	
			updateDutyCycleA0(duty4);		// SALIDA D6 O PD6
			
		}
		else if (Modo==1)
		{
			PORTD &= ~(1<<PORTD2);					//ENCENDER LED
			PORTD |= (1<<PORTD4);					//ENCENDER LED
			
			PORTB |= (1<<PORTB5);

		}
		else if (Modo==2)
		{
			PORTD &= ~(1<<PORTD4);					//ENCENDER LED
			PORTD |= (1<<PORTD7);					//ENCENDER LED
			//comprobarque(valorUART);
			if (canal == 1){
				_delay_ms(10);
				updateDutyCycleB0(payload);		// SALIDA D5 O PD5
			}
			else if (canal == 2){
				_delay_ms(10);
				updateDutyCycleB2(payload);		// SALIDA D5 O PD5
			}
			else if (canal == 3){
				_delay_ms(10);
				updateDutyCycleA2(payload);		// SALIDA D5 O PD5
			}
			else if (canal == 4){
				_delay_ms(10);
				updateDutyCycleA0(payload);		// SALIDA D5 O PD5
			}
			//_delay_ms(10);
			//UARTCHANEL(valorUART);
			//updateDutyCycleB0(payload);		// SALIDA D5 O PD5
			//_delay_ms(10);
			//updateDutyCycleB2(payload);		// SALIDA D3 O PD3
			//_delay_ms(10);
			//duty3 = ADC_CHANEL_SELECT(7);
			//updateDutyCycleA2(duty3);		// SALIDA D11 O PB3
			//_delay_ms(10);
			//duty4 = ADC_CHANEL_SELECT(4);
			//updateDutyCycleA0(duty4);		// SALIDA D6 O PD6
		}
		//ADCSRA |= (1<<ADIF);						
    }
}

void initPCint1(void){
	//INTERRUPCIONES EN LOS BOTONES PC0, PC1, PC2 y PC3
	PCMSK1 |= ((1<<PCINT11)|(1<<PCINT10)|(1<<PCINT9)|(1<<PCINT8));
	//HABILITANDO LA ISR PCINT[14:8]
	PCICR |= (1 << PCIE1);
}

void initPCint0(void){
	
	PCMSK0 |= ((1<<PCINT0)|(1<<PCINT4));			//INTERRUPCIONES EN LOS BOTONES PB0 y PB4
	PCICR |= ((1 << PCIE0));						//HABILITANDO LA ISR PCINT[14:8] E ISR PCINT[7:0]
}

void ChangeMode(void){
	if(Modo==0)
	{
		Modo=1;					//CAMBIAR A MODO 1
	}
	else if (Modo==1)
	{
		Modo=2;					//CAMBIAR A MODO 2
	}
	else if (Modo==2)
	{
		Modo=0;					//CAMBIAR A MODO 0
	}
}

void UARTBOTON(char valorUART){
	switch(valorUART){
		case 'r':
		ChangeMode();
		break;
		case 't':
		AccionBoton1();
		case 'v':
		AccionBoton2();
	}
}

void AccionBoton1(void){
	if(Modo==0)
	{
		EEPROM_write(0, duty1);
		EEPROM_write(1, duty2);
		EEPROM_write(2, duty3);
		EEPROM_write(3, duty4);
	}
	else if (Modo==1)
	{
		duty1 = EEPROM_read(0);		//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
		updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
		duty2 = EEPROM_read(1);		//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
		updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
		duty3 = EEPROM_read(2);		//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
		updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
		duty4 = EEPROM_read(3);		//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
		updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
	}
	else if (Modo==2)
	{
		//Modo=0;					//CAMBIAR A MODO 0
	}
}

void AccionBoton2(void){
	if(Modo==0)
	{
		EEPROM_write(4, duty1);
		EEPROM_write(5, duty2);
		EEPROM_write(6, duty3);
		EEPROM_write(7, duty4);
	}
	else if (Modo==1)
	{
		duty1 = EEPROM_read(4);		//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
		updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
		duty2 = EEPROM_read(5);		//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
		updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
		duty3 = EEPROM_read(6);		//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
		updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
		duty4 = EEPROM_read(7);		//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
		updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
	}
	else if (Modo==2)
	{
		//Modo=0;					//CAMBIAR A MODO 0
	}
}


void comprobarque(char valorUART){
	switch(valorUART){
		case '0':
		payload = 0;
		break;
		
		case '1':
		payload = 10;
		break;
		
		case '2':
		payload = 20;
		break;
		
		case '3':
		payload = 30;
		break;
		
		case '4':
		payload = 40;
		break;
		
		case '5':
		payload = 50;
		break;
		
		case '6':
		payload = 60;
		break;
		
		case '7':
		payload = 70;
		break;
		
		case '8':
		payload = 80;
		break;
		
		case '9':
		payload = 90;
		break;
		
		case 'a':
		payload = 100;
		break;
		
		case 'b':
		payload = 110;
		break;
		
		case 'c':
		payload = 120;
		break;
		
		case 'd':
		payload = 130;
		break;
		
		case 'e':
		payload = 140;
		break;
		
		case 'f':
		payload = 150;
		break;
		
		case 'g':
		payload = 160;
		break;
		
		case 'h':
		payload = 170;
		break;
		
		case 'i':
		payload = 180;
		break;
		
		case 'j':
		payload = 190;
		break;
		
		case 'k':
		payload = 200;
		break;
		
		case 'l':
		payload = 210;
		break;
		
		case 'm':
		payload = 220;
		break;
		
		case 'n':
		payload = 230;
		break;
		
		case 'o':
		payload = 240;
		break;
		
		case 'p':
		payload = 250;
		break;
		
		case 'q':
		payload = 255;
		break;
	}
}

void UARTCHANEL(char valorUART){
	switch(valorUART){
		case 'x':
		canal = 1;
		break;
		case 'y':
		canal = 2;
		break;
		case 'z':
		canal = 3;
		break;
		case 'w':
		canal = 4;
		break;
	}
}

ISR (PCINT1_vect){
	uint8_t puertoC = PINC;
	/*
	if((puertoC & (1<<PINC0))==0){
		ChangeMode();
	}*/
	
	if(Modo==0)
	{
		if((puertoC & (1<<PINC0))==0){
			EEPROM_write(0, duty1);
			EEPROM_write(1, duty2);
			EEPROM_write(2, duty3);
			EEPROM_write(3, duty4);	
		}
		else if ((puertoC & (1<<PINC1))==0){
			EEPROM_write(4, duty1);
			EEPROM_write(5, duty2);
			EEPROM_write(6, duty3);
			EEPROM_write(7, duty4);
		}
		else if ((puertoC & (1<<PINC2))==0){
			EEPROM_write(8, duty1);
			EEPROM_write(9, duty2);
			EEPROM_write(10, duty3);
			EEPROM_write(11, duty4);
		}
		else if ((puertoC & (1<<PINC3))==0){
			EEPROM_write(12, duty1);
			EEPROM_write(13, duty2);
			EEPROM_write(14, duty3);
			EEPROM_write(15, duty4);
		}
		
	}
	else if (Modo==1)
	{
		if((puertoC & (1<<PINC0))==0){
			duty1 = EEPROM_read(0);		//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
			updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
			duty2 = EEPROM_read(1);		//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
			updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
			duty3 = EEPROM_read(2);		//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
			updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
			duty4 = EEPROM_read(3);		//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
			updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
			
		}
		else if ((puertoC & (1<<PINC1))==0){
			duty1 = EEPROM_read(4);		//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
			updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
			duty2 = EEPROM_read(5);		//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
			updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
			duty3 = EEPROM_read(6);		//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
			updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
			duty4 = EEPROM_read(7);		//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
			updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
		}
		else if ((puertoC & (1<<PINC2))==0){
			duty1 = EEPROM_read(8);		//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
			updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
			duty2 = EEPROM_read(9);		//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
			updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
			duty3 = EEPROM_read(10);	//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
			updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
			duty4 = EEPROM_read(11);	//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
			updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
		}
		else if ((puertoC & (1<<PINC3))==0){
			duty1 = EEPROM_read(12);	//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
			updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
			duty2 = EEPROM_read(13);	//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
			updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
			duty3 = EEPROM_read(14);	//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
			updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
			duty4 = EEPROM_read(15);	//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
			updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
		}
	}
	else if (Modo==2)
	{
		if((puertoC & (1<<PINC1))==0){
			//PORTD &= ~(1<<PORTD2);					//APAGAR LED
			//PORTD |= (1<<PORTD2);					//ENCENDER LED
		}
	}
	PCIFR |= (1<<PCIF1);			//APAGAR BANDERA DE ISR PCINT1
}

ISR (PCINT0_vect){
	uint8_t puertoB = PINB;
	if((puertoB & (1<<PINB4))==0){
		ChangeMode();
	}
	
	if(Modo==0)
	{
		if((puertoB & (1<<PINB0))==0){
			EEPROM_write(16, duty1);
			EEPROM_write(17, duty2);
			EEPROM_write(18, duty3);
			EEPROM_write(19, duty4);
		}
	}
	else if (Modo==1)
	{
		if((puertoB & (1<<PINB0))==0){
			duty1 = EEPROM_read(16);	//GUARDAR EN duty1 EL VALOR 0 DE LA EEPROM
			updateDutyCycleB0(duty1);		//MOVER SERVO AL NUEVO VALOR DE duty1
			duty2 = EEPROM_read(17);	//GUARDAR EN duty2 EL VALOR 1 DE LA EEPROM
			updateDutyCycleB2(duty2);		//MOVER SERVO AL NUEVO VALOR DE duty2
			duty3 = EEPROM_read(18);	//GUARDAR EN duty3 EL VALOR 2 DE LA EEPROM
			updateDutyCycleA2(duty3);		//MOVER SERVO AL NUEVO VALOR DE duty3
			duty4 = EEPROM_read(19);	//GUARDAR EN duty4 EL VALOR 3 DE LA EEPROM
			updateDutyCycleA0(duty4);		//MOVER SERVO AL NUEVO VALOR DE duty4
		}
	}
	else if (Modo==2)
	{
		if((puertoB & (1<<PINB0))==0){
			//PORTD &= ~(1<<PORTD2);					//APAGAR LED
			//PORTD |= (1<<PORTD2);					//ENCENDER LED
		}
	}
	PCIFR |= (1<<PCIF1);			//APAGAR BANDERA DE ISR PCINT1
}

ISR(USART_RX_vect){
	PORTB &= ~(1<<PORTB5);
	valorUART = UDR0;
	UARTCHANEL(valorUART);
	comprobarque(valorUART);
	UARTBOTON(valorUART);
	
	while(!(UCSR0A & (1<<UDRE0)));		//HASTA QUE NO HAYAN CARACTERES
}