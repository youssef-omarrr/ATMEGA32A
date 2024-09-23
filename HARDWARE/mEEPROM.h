/* 
 * File:   mEEPROM.h
 * Author: User
 *
 * Created on September 23, 2024, 8:36 PM
 */

#ifndef MEEPROM_H
#define	MEEPROM_H

#define  F_CPU 16000000UL
#include <util/delay.h>

#include "mSPI.h"
#include "mI2C.h"
#include "DIO.h"

///////////////  1) SPI EXTERNALL EEPROM   ///////////////////////////////

/*to send data to a device like memory card
1) send instruction (send data, go home, ...)
2) send address
3) send data
 
---- to receive data we send a dummy data like (0x00) just to push
     the shift registers to send the data in the device back to the master */

#define EE_READ     0x03       //Read data from memory array beginning at selected address
#define EE_WRITE    0x02       //Write data to memory array beginning at selected address
#define EE_WREN     0x06       //Set the write enable latch (enable write operations)
#define EE_RDSR     0x05       //Read STATUS register
#define EE_WRSR     0x01       //Write STATUS register

/* page address begins with XXXX 0000 
 * and ends with XXXX 1111.
 */

void EE_SEND_DATA (char address, char data);
void EE_SEND_DATA_STR (char address, char* data);
void EE_SEND_DATA_ARR (char address, char* data, char size); //array you want to write from your code to EEPROM
char EE_READ_DATA (char address);
void EE_READ_DATA_ARR (char address, char* data, char size); //array you want to write to from EEPROM to code
char check_error(char size);

///////////////  1) SPI EXTERNALL EEPROM   ///////////////////////////////

///////////////  2) I2C EXTERNALL EEPROM   ///////////////////////////////

#define EEPROM_ADDRESS_READ    0xA1 //1010 000 1
#define EEPROM_ADDRESS_WRITE   0xA0 //1010 000 0

char EEPROM_TWI_WRITE(char address, char data);
char EEPROM_TWI_READ(char address);

///////////////  2) I2C EXTERNALL EEPROM   ///////////////////////////////

///////////////  3) INTERNALL EEPROM   ///////////////////////////////

/*The AVR ATmega32A contains 1024bytes of data EEPROM memory
 
 * The EEPROM has an endurance of at least 100,000 write/erase
    cycles.
 */

/*
 REGISTERS:
 * EEARH and EEARL ? EEPROM Address Register
 * EEDR ? EEPROM Data Register
 * EECR ? EEPROM Control Register
 * Bit 3 ? EERIE: EEPROM Ready Interrupt Enable
 * Bit 2 ? EEMWE: EEPROM Master Write Enable
 * Bit 1 ? EEWE: EEPROM Write Enable
 * Bit 0 ? EERE: EEPROM Read Enable
 */

#define WAIT_EEWE     while (EECR & (1<<EEWE)) //Wait until EEWE becomes zero.

void EEPROM_INTERNAL_WRITE(short int address, char data);
char EEPROM_INTERNAL_READ(short int address);
void EEPROM_INTERNAL_INT_EN();
void EEPROM_INTERNAL_INT_DIS();

///////////////  3) INTERNALL EEPROM   ///////////////////////////////


#endif	/* MEEPROM_H */

