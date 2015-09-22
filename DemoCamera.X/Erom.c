#include "eeprom.h"

void EEPROM_Write(WORD add, BYTE data)
{
    BYTE sr;

    // Kiem tra bit WIP - Write in Process trong thanh ghi trang thai
   /* do{
      EEPROM_nCS = 0;
      EE_SPIPut(0x05);       // Gui lenh doc thanh ghi trang thai
      sr = EE_SPIGet();   // Doc ve thanh ghi trang thai
      EEPROM_nCS = 1;
    }while(sr&0x01);
*/
    // Ghi WORD wrdata len EEPROM tai dia chi add
    EEPROM_nCS = 0;
    EE_SPIPut(0x06);       // Gui lenh WREN sang cho EEPROM
    EEPROM_nCS = 1;

    EEPROM_nCS = 0;
    EE_SPIPut(0x02);       // Gui lenh WRITE sang cho EEPROM
    EE_SPIPut(add>>8);     // Gui 8 bit cao cua dia chi
    EE_SPIPut(add&0x00FF); // Gui 8 bit thap cua dia chi
    EE_SPIPut(data);     // Gui WORD can ghi
    EEPROM_nCS = 1;
}

BYTE EEPROM_Read(WORD add)
{
    BYTE kq, sr;

    // Kiem tra bit WIP - Write in Process trong thanh ghi trang thai
    do{
      EEPROM_nCS = 0;
      EE_SPIPut(0x05);       // Gui lenh doc thanh ghi trang thai
      sr = EE_SPIGet();   // Doc ve thanh ghi trang thai
      EEPROM_nCS = 1;
    }while(sr&0x01);

    EEPROM_nCS = 0;
    EE_SPIPut(0x03);           // Gui lenh READ
    EE_SPIPut(add>>8);         // Gui 8 bit cao cua dia chi
    EE_SPIPut(add&0x00FF);     // Gui 8 bit thap cua dia chi
    kq = EE_SPIGet();       // Doc du lieu tu EEPROM
    EEPROM_nCS = 1;

    return kq;
}

void EEPROM_ReadArray(WORD add, BYTE * buffer, WORD len)
{
    WORD i;
    BYTE sr;

    // Kiem tra bit WIP - Write in Process trong thanh ghi trang thai
    do{
      EEPROM_nCS = 0;
      EE_SPIPut(0x05);       // Gui lenh doc thanh ghi trang thai
      sr = EE_SPIGet();   // Doc ve thanh ghi trang thai
      EEPROM_nCS = 1;
    }while(sr&0x01);

    EEPROM_nCS = 0;
    EE_SPIPut(0x03);           // Gui lenh READ
    EE_SPIPut(add>>8);         // Gui 8 bit cao cua dia chi
    EE_SPIPut(add&0x00FF);     // Gui 8 bit thap cua dia chi

    for(i=0;i<len;i++)
    {
        buffer[i] = EE_SPIGet();
        // *(buffer + i) = EE_SPIGet(0x00);
    }
    EEPROM_nCS = 1;
}


