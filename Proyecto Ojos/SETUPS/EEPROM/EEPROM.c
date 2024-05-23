/*
 * EEPROM.c
 *
 * Created: 17/05/2024 05:08:57 p. m.
 *  Author: josei
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "EEPROM.h"

uint8_t EEPROM_read(uint8_t uiAdress)
{
	//WAIT FOR COMPLETION OF PREVIOUS WRITE
	while (EECR & (1<<EEPE));
	
	//SET UP ADRESS REGISTER
	EEAR = uiAdress;
	
	//START EEPROM READ BY WRITING EERE
	EECR |= (1<<EERE);
	
	return EEDR;
	
}

void EEPROM_write(uint8_t uiAdress, uint8_t ucData)
{
	//WAIT FOR COMPLETION OF PREVIOUS WRITE
	while (EECR & (1<<EEPE));
	
	//SET UP ADRESS AND DATA REGISTERS
	EEAR = uiAdress;
	EEDR = ucData;
	
	//WRITE LOGICAL ONE TO EEMPE
	EECR |= (1<<EEMPE);
	
	//START EEPROM WRITE BY SETTING EEPE
	EECR |= (1<<EEPE);
}



