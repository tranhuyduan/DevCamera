#include "PTC06.h"
#include "eeprom.h"
//#include "delay32.h"
BYTE length_photo[2];
DWORD photo_len;
BYTE reset_cmd[] =
{
	0x56,
	0x00,
	0x26,
	0x00,
        0x00
};
BYTE reset_rcv [] =
{
	0x76,
	0x00,
	0x26,
	0x00
};
BYTE start_photo_cmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x00
};
BYTE start_photo_rcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};
BYTE read_len_cmd [] =
{
	0x56,
	0x00,
	0x34,
	0x01,
	0x00
};
BYTE read_len_rcv [] =
{
	0x76,
	0x00,
	0x34,
	0x00,
	0x04,
	0x00,
	0x00
};
BYTE get_photo_cmd [] =
{
	0x56,
	0x00,
	0x32,
	0x0C,
	0x00,
	0x0A,

	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0xFF

};
BYTE get_photo_rcv [] =
{
	0x76,
	0x00,
	0x32,
	0x00,
	0x00
};

#ifdef ZOOM_SIZE
BYTE zoom_size_cmd [] =
{
	0x56,
	0x00,
	0x54,
	0x01,
	0x11,
        0x00
};
#endif

BYTE zoom_size_rcv [] =
{
	0x76,
	0x00,
	0x54,
	0x00,
	0x00,
};
BYTE stop_photo_cmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x03
};

BYTE stop_photo_rcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};
#ifdef SIZE_640x480
BYTE photo_size_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x00,
	0x00
};

#endif
#ifdef SIZE_320x240
BYTE photo_size_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x11,
	0x00
};
#endif
BYTE photo_size_rcv [] =
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00
};
BYTE save_power_cmd [] =
{
	0x56,
	0x00,
	0x3E,
	0x03,
	0x00,
	0x01,
	0x00
};
BYTE save_power_rcv [] =
{
	0x76,
	0x00,
	0x3E,
	0x00,
	0x00
};
BYTE compress_rate_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x01,
	0x01,
	0x12,
	0x04,
	0x80
};

BYTE compress_rate_rcv [] =
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00
};
BYTE set_bps_cmd [] =
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
#ifdef BAUD_RATE_115temp
	0x0D,
	0xA6
#endif
#ifdef  BAUD_RATE_9600
        0xAE,
        0xC8
#endif
#ifdef  BAUD_RATE_19temp
        0x56,
        0xE4
#endif
#ifdef  BAUD_RATE_38400
        0x2A,
        0xF2
#endif
#ifdef  BAUD_RATE_57600
        0x1C,
        0x4C
#endif
};
BYTE set_bps_rcv [] =
{
	0x76,
	0x00,
	0x24,
	0x00,
	0x00
};

BYTE clean_cache_cmd[]={
        0x56,
        0x00,
        0x36,
        0x01,
        0x03
};
BYTE clean_cache_rcv[]={
        0x76,
        0x00,
        0x36,
        0x00,
        0x00
};


void sendByte(BYTE byte){
    ConsolePut(byte);
}
// gui lenh xuong cmr
void sendComand(BYTE buffer[], WORD len){
    WORD i = 0;
    for(i = 0; i<len; i++){
        sendByte(buffer[i]);
    }
   
}
// nhan data tu cmr
BYTE getCommand( BYTE buffer[], WORD len){
    DWORD i=0;
    for(i = 0;i<len; i++){
          buffer[i]= ConsoleGet();
        
    }
   if(buffer[0] == CMD_CMR)
        return 1;
    else
        return 0;
}
BYTE getComand1(DWORD len){
    while(len-->0){
        UART1PrintChar(ConsoleGet());
    }
     return 0;
}
// reset cmr
BYTE sendReset(void){
    BYTE buffer[10];
    BYTE i = 0,j=0,k=0;
    UART1PutROMString("send Reset Photo\n");
    sendComand(reset_cmd, 5);
   // __delay_ms(2000);
    /*if(!getCommand(buffer,10)){
       for(i =0;i<4;i++)
         UART1Put(buffer[i]);
       UART1PutROMString("send Reset Photo Error\n");
       return 0;
    }*/
    for(i=0;i<2;i++){
        getCommand(buffer,10);
        for(j=0;j<10;j++){
            EEPROM_Write(j+k,buffer[j]);
        }
        k=k+j;
    }
    UART1PutROMString("sssssssssssssss\n");
    for(i=0;i<20;i++){
    UART1PrintChar(EEPROM_Read(i));
    }
    
    for( i =0; i<4; i++){
        //UART1Put(buffer[i]);
        if(buffer[i]!= reset_rcv[i]) return 0;
      }
    for( i =0; i<10; i++){
        UART1PrintChar(buffer[i]);
              }
    UART1PutROMString("send Reset Photo End\n");
    return 1;
    }
   
BYTE sendGetLength(void){
    BYTE buffer[7];
    BYTE i = 0;
    UART1PutROMString("send Get Length\n");
    sendComand(read_len_cmd, 5);
    if(!getCommand(buffer, 9)) return 0;
    for(i = 0; i<7; i++){
        UART1Put(buffer[i]);
        if(buffer[i]!=read_len_rcv[i]){
             UART1PutROMString("send Get Length error\n");
            return 0;
             }
    }
    setPhotoLen(buffer[7], buffer[8]);
    UART1PutROMString("\n");
    for(i = 0; i<9; i++){
        UART1PrintChar(buffer[i]);
         }
    UART1PutROMString("\nsend Get Length end\n");
    return 1;
}
void setPhotoLen(BYTE len1, BYTE len2){
    DWORD temp;
    temp  = len1*0x2000 +len2;
    get_photo_cmd[12] =len1;
    get_photo_cmd[13] = len2;
    photo_len =temp;
    UART1PutROMString("photo_len=");
    UART1Put(photo_len);
   // UART1PrintChar(photo_len);
}
BYTE sendStartPhoto(void){
    BYTE buffer[5];
    BYTE i = 0;
    UART1PutROMString("\nSend start photo\n");
    sendComand(start_photo_cmd, 10);
    if(!getCommand(buffer, 5)) return 0;
    for(i = 0; i<5; i++){
        UART1PrintChar(buffer[i]);
        if(buffer[i]!= start_photo_rcv[i]){
             UART1PutROMString("Send start photo Error\n");
            return 0;
        }
          
    }
            //UART1PrintChar(buffer[i]);
    UART1PutROMString("Send start photo End\n");
    return 1;
}

/*DWORD GetData()
{
    DWORD i = 0;
    DWORD data_len;

    UART1PutROMString("\n Data_len = ");
    IFS0bits.U1RXIF = 0;//enable UART1 interrupt
    data_len = U1RXREG;
    UART1PutROMString(data_len);

    DWORD temp = 14 + data_len;
    BYTE txbuffer[temp];

    UART1PutROMString("\n Tong_len = ");
    UART1PutROMString(temp);

    UART1PutROMString("\n Command get data \r\t");
    for(i = 0;i<16;i++)
    UART1PrintChar(get_photo_cmd[i]);
}*/

DWORD sendGetPhoto(void){
    DWORD i = 0;
    DWORD temp;
    temp=14+photo_len;
    BYTE txbuffer1[2000];
    //BYTE txbuffer2[2000];
    for(i = 0;i<16;i++)
    UART1PrintChar(get_photo_cmd[i]);
    UART1PutROMString("start get photo \n");
    //UART1PrintChar(temp);
    sendComand(get_photo_cmd, 16);
   // if(photo_len >2000)
    //UART1PutROMString(">2000 \n");
    
        getCommand(txbuffer1,2000);

       for(i=0;i<2000;i++){
          EEPROM_Write(i,txbuffer1[i]);
           UART1PrintChar(txbuffer1[i]);
    }
        //__delay_ms(10);
    UART1PutROMString("\nsssssssssssssss\n");
   

          for(i = 0; i<5; i++){
        if(txbuffer1[i]!=get_photo_rcv[i]){
            UART1PutROMString("error \r\n");
            return 0;
        }
    }
         UART1PutROMString("Datas \r\n");
//     UART1PrintDWord(temp);

  //  for(i = 0; i<2000; i++){
     //    UART1PrintChar(txbuffer1[i]);
   //  }
      //for(i = 0; i<5; i++)
        //if(*(rxbuf
   // }
    //for(i = 0; i<5fer +i +5)!=get_photo_cmd[i]) return 0;
   /** if (txbuffer[5] != 0xFF || txbuffer[6] != 0xD8 ||
		txbuffer[3+*photo_len] != 0xFF ||txbuffer[4+*photo_len] != 0xD9)
    {
	UART1PutROMString("error 3\n");
        return 0;}
    //for (i = 0; i < *photo_len; i++)
		//buffer[i] = buffer[i+5];*/
    UART1PutROMString("\n get photo end\n");
    return 1;
}
DWORD takePhoto(BYTE *buffer){
    DWORD len;
    //if(!photoInit()) goto error_exit;
    if(!sendStartPhoto()) goto error_exit;
    if(!sendGetLength()) goto error_exit;
   // len = sendGetPhoto(buffer);
    if(len = 0) goto error_exit;
    return len;
    error_exit:
    return 0;
}
DWORD demotakePhoto(BYTE *buffer){
    sendStartPhoto();
     //__delay_ms(10);
    sendGetLength();
      //__delay_ms(10);
    //sendGetPhoto(buffer);
}

void sendBuffer(BYTE const *buffer, WORD len){
    int i = 0;
    for(i = 0; i<len; i++){
        ConsolePut(buffer[i]);
    }
}
WORD getBuffer(BYTE *buffer, WORD len){
    WORD i =0;
    for(i = 0; i<len; i++)
        buffer[i] = ConsoleGet();
    if(buffer[0] == CMD_CMR)
        return 1;
    else
        return 0;
}

BYTE sendStopPhoto(void){
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send stop photo \n");
    sendComand(stop_photo_cmd, 5);
    if(!getCommand(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        //UART1Put(buffer[i]);
        if(buffer[i]!=stop_photo_rcv[i]){
         UART1PutROMString("Send stop photo error\n");
            return 0;
        }
    UART1PutROMString("Send stop photo end\n");
    return 1;
}
BYTE sendPhotoSize(void)
{
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send photo Size\n");
    sendComand(photo_size_cmd, 10);
    if(!getCommand(buffer, 5)){
        return 0;
    }
    for(i = 0; i<5; i++)
       // UART1Put(buffer[i]);
        if(buffer[i] !=photo_size_rcv[i]){
            UART1PutROMString("Send photo size Error\n");
            return 0;
            
        }
    UART1PutROMString("Send Photo Size End\n");
    return 1;
}
BYTE sendCleancache(void){
     BYTE i;
     BYTE buffer[5];
    UART1PutROMString("send Clean cache\n");
     sendComand(clean_cache_cmd,5);
     if (!getCommand(buffer, 5)) {
       return 0;
    }
	for (i = 0; i < 5; i++){
               // UART1Put(buffer[i]);
		if (buffer[i] != clean_cache_rcv[i]){
                 UART1PutROMString("send Clean cache error\n");
                    return 0;
                }
        }
     UART1PutROMString("send Clean cache  end\n");
	return 1;
}
BYTE sendZoomSize(void)
{
    BYTE i;
    BYTE buffer[5];
#ifdef ZOOM_SIZE
    sendComand(zoom_size_cmd, 6);
#endif
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= zoom_size_rcv[i]) return 0;
    return 1;
}
BYTE sendSavePower(void)
{
    BYTE i;
    BYTE buffer[5];
    sendComand(save_power_cmd, 7);
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= save_power_rcv[i]) return 0;
    return 1;
}
BYTE photoInit(void)
{
     UART1PutROMString("Photo Init \n");
     //if(!sendReset()) goto error;
#ifdef ZOOM_SIZE
    if(!sendZoomSize()) goto error;
#endif
//    if(!sendReset()) goto error;
    if(!sendPhotoSize()) goto error;
    if(!sendStopPhoto()) goto error;
    if(!sendCompressRate()) goto error;
     if(!sendCleancache()) goto error;
    UART1PutROMString("Photo Init End\n");
    return 1;
    error:
    UART1PutROMString("Photo Init Error\n");
    return 0;
}
BYTE sendCompressRate(void){
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send Compress Photo\n");
     sendComand(compress_rate_cmd, 9);
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++){
       // UART1Put(buffer[i]);
        if(buffer[i]!= compress_rate_rcv[i]){
            UART1PutROMString("Send Compress photo Error\n");
            return 0;
        }
    }
     UART1PutROMString("Send Compress Photo End\n");
    return 1;

}
BYTE setBaudRate(void)
{
    BYTE i;
    BYTE buffer[5];
    sendComand(set_bps_cmd, 7);
        

    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= set_bps_rcv[i]) return 0;
        //UART1PutROMString("set baurate1\n");

    return 1;

}
WORD convertHextoDec(BYTE msb, BYTE lsb){
    WORD result;
    //result = msb *
}