/*
 * EEPROM.h
 *
 * Created: 17/05/2024 05:09:15 p. m.
 *  Author: josei
 */ 

#include <avr/io.h>
#include <stdint.h>

#ifndef EEPROM_H_
#define EEPROM_H_

uint8_t EEPROM_read(uint8_t uiAdress);

void EEPROM_write(uint8_t uiAdress, uint8_t ucData);

#endif /* EEPROM_H_ */