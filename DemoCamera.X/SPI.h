/* 
 * File:   EESPI.h
 * Author: bienphongvnn
 *
 * Created on September 22, 2015, 2:29 PM
 */

#ifndef SPI_H
#define	SPI_H
#include "console.h"

#include "p24FJ128GA306.h"
#include "generic.h"

#define EXTERNAL_NVM_BYTES 32768
#define EEPROM_PAGE_SIZE 64
#define EEPROM_nCS LATDbits.LATD11
#define EEPROM_nCS_TRIS TRISDbits.TRISD11
//EE is on SPI2
#define EE_SSPIF_BIT IFS2bits.SPI2IF
#define EE_SSPBUF_REG SPI2BUF


    void SPIInit(void);
    void EE_SPIPut(BYTE v);
    BYTE EE_SPIGet(void);
    #define EE_SPIInit()   EE_SSPIF_BIT = 1

#endif

