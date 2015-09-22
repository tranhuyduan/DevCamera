/* 
 * File:   eeprom.h
 * Author: bienphongvnn
 *
 * Created on September 22, 2015, 3:33 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include "SPI.h"
void EEPROM_Write(WORD add, BYTE data);
BYTE EEPROM_Read(WORD add);
void EEPROM_ReadArray(WORD add, BYTE * buffer, WORD len);
#endif	/* EEPROM_H */

