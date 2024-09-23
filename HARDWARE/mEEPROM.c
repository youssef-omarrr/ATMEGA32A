#include <avr/io.h>
#include "mEEPROM.h"

///////////////  1) SPI EXTERNALL EEPROM   ///////////////////////////////

//before any instruction you must set SS to low and after every instruction set it to HIGH

void EE_SEND_DATA (char address, char data){
    /*steps to write data
     * 1) enable write
     * 2) send write instruction
     * 3) send address 
     * 4) send data
     * 
     * before every instruction we must set SS HIGH then LOW*/
    
    setPINB(SS, HIGH);
    setPINB(SS, LOW);
    
    SPIsend(EE_WREN);
    
    setPINB(SS, HIGH);
    _delay_ms(5);
    ////////////////////////////
    setPINB(SS, LOW);
    
    SPIsend(EE_WRITE);
    SPIsend(address);
    SPIsend(data);
    
    setPINB(SS, HIGH);
    _delay_ms(5); 
}

void EE_SEND_DATA_STR (char address, char* data){
    /*steps to write data
     * 1) enable write
     * 2) send write instruction
     * 3) send address 
     * 4) send data
     * 
     * before every instruction we must set SS HIGH then LOW*/
    
    setPINB(SS, HIGH);
    setPINB(SS, LOW);
    
    SPIsend(EE_WREN);
    
    setPINB(SS, HIGH);
    _delay_ms(5);
    
    ////////////////////////////
    
    setPINB(SS, LOW);

    SPIsend(EE_WRITE);
    SPIsend(address);
    
    for (int i=0; data[i] != '\0'; i++){
        SPIsend(data[i]);
//        LCD4_data(data[i]);
    }
        
    
    
    setPINB(SS, HIGH);
    _delay_ms(5); 
}

//array you want to write from your code to EEPROM
void EE_SEND_DATA_ARR (char address, char* data, char size){
    /*steps to write data
     * 1) enable write
     * 2) send write instruction
     * 3) send address 
     * 4) send data
     * 
     * before every instruction we must set SS HIGH then LOW*/
    
    if (check_error(size))
        return;
    
    setPINB(SS, HIGH);
    setPINB(SS, LOW);
    
    SPIsend(EE_WREN);
    
    setPINB(SS, HIGH);
    _delay_ms(5);
    
    ////////////////////////////
    
    setPINB(SS, LOW);

    SPIsend(EE_WRITE);
    SPIsend(address);
    
    for (int i=0; i < size; i++){
        SPIsend(data[i]);
    }
        
    setPINB(SS, HIGH);
    _delay_ms(5); 
}

//read by sending dummy variable and pushing data to master
char EE_READ_DATA (char address){ 
    setPINB(SS, HIGH);
    setPINB(SS, LOW);
    
    SPIsend(EE_READ);
    SPIsend(address);
    //we will not send any real data we just put anything as a place holder to push the shift registers
    char data = SPI_MASTER_receive();
    setPINB(SS, HIGH);
    
    return data;
    
}

//array you want to write to from EEPROM to code
void EE_READ_DATA_ARR (char address, char* data, char size){
    if (check_error(size))
        return;
    
    setPINB(SS, HIGH);
    setPINB(SS, LOW);
    
    SPIsend(EE_READ);
    SPIsend(address);
    
    for (int i=0; i<size; i++){
        data[i] = SPI_MASTER_receive();
    }
    
    setPINB(SS, HIGH);
    
}

char check_error(char size){
    if (size > 16)
        return 1;
    else
        return 0;
}

///////////////  1) SPI EXTERNALL EEPROM   ///////////////////////////////

///////////////  2) I2C EXTERNALL EEPROM   ///////////////////////////////

/*STEPS to WRITE on EEPROM:
 * 1) send start condition
 * 2) send EEPROM address as WRITE (the main address of the EEPROM device)
 * 3) send memory location address (the sub-address inside the main EEPROM)
 * NOTE: EEPROM address is sent as ADDRESS but memory location address is sent as DATA
 * 4) send data
 * 5) send stop condition
 
 * if we wanted to READ from EEPROM:
 * 1) send start condition
 * 2) send EEPROM address as WRITE first
 * 3) send memory location address 
 * NOTE: EEPROM address is sent as ADDRESS but memory location address is sent as DATA
 * 4) send REPEATED START
 * 5) send EEPROM address as READ (also sent as ADDRESS not DATA)
 * 6) send ACK and READ data 
 * 7) send stop condition

 * addresses in EEPROM is as follows:
 * 1010         A0A1A2                               R/W
 * constant     3 pins in EEPROM to choose PAGE      read (1) or write (0) instruction
 * 
 * ex: if we want to choose the FIRST PAGE to WRITE to the address will be:
 * 1010 000 0
 
 */


char EEPROM_TWI_WRITE(char address, char data){
    char statuscode = 0;
    
    //1)start condition (enable function ORed with start condition (TWSTA) bit)
    ENABLE_FUNCTION | (1<< TWSTA);
    //wait for flag to be 1 (data bus is free after sending all data)
    WAIT_TWINT;
    
    
    //2)read status register (& 0xF8 to clear pre-scaler effect)
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x08)
    if (statuscode != I2C_STATUS_MT_START)
        return ERROR;
    
    //3) Send EEPROM address + Write (to set memory address)
    TWDR = EEPROM_ADDRESS_WRITE; 
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x18)
    if (statuscode != I2C_STATUS_MT_SLA_ACK)
        return ERROR;
    
    //4) send memory location address
    TWDR = address; 
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x28)
    if (statuscode != I2C_STATUS_MT_DATA_ACK)
        return ERROR;
    
    //5) send data
    TWDR = data;
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x28)
    if (statuscode != I2C_STATUS_MT_DATA_ACK)
        return ERROR;
    
    //6)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    return 1;
    
}

char EEPROM_TWI_READ(char address){
    char statuscode = 0;
    //FIRST WE ARE MASTER TRANSMITER
    
    //1)start condition (enable function ORed with start condition (TWSTA) bit)
    ENABLE_FUNCTION | (1<< TWSTA);
    //wait for flag to be 1 (data bus is free after sending all data)
    WAIT_TWINT;
    
    
    //2)read status register (& 0xF8 to clear pre-scaler effect)
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x08)
    if (statuscode != I2C_STATUS_MT_START)
        return ERROR;
    
    //3) Send EEPROM address + Write (to set memory address)
    TWDR = EEPROM_ADDRESS_WRITE; 
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x18)
    if (statuscode != I2C_STATUS_MT_SLA_ACK)
        return ERROR;
    
    
    //4) send memory location address
    TWDR = address; 
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x28)
    if (statuscode != I2C_STATUS_MT_DATA_ACK)
        return ERROR;
    
    //5) REPEATED address
    ENABLE_FUNCTION | (1<< TWSTA);
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x10)
    if (statuscode != I2C_STATUS_MT_REP_START)
        return ERROR;
    
    //NOW WE ARE MASTER RECIVER
    
    //6) Send EEPROM address + Read
    TWDR = EEPROM_ADDRESS_READ; 
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x40)
    if (statuscode != I2C_STATUS_MR_SLA_ACK)
        return ERROR;
    
    //7) send ACK if needed and wait for data
    ENABLE_FUNCTION | (1<< TWSTA);
    WAIT_TWINT;
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x50)
    if (statuscode != I2C_STATUS_MR_DATA_ACK)
        return ERROR;
    
    //8) receive data
    char data = TWDR;
    
    //9) send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
   
    return data;
    
}

///////////////  2) I2C EXTERNALL EEPROM   ///////////////////////////////

///////////////  3) INTERNALL EEPROM   ///////////////////////////////
/* To WRITE on internal EEPROM
 * 1. Wait until EEWE becomes zero.
 * 2. Wait until SPMEN in SPMCR becomes zero.
 * 3. Write new EEPROM address to EEAR (optional).
 * 4. Write new EEPROM data to EEDR (optional).
 * 5. Write a logical ONE to the EEMWE bit while writing a ZERO to EEWE in EECR.
 * 6. Within four clock cycles after setting EEMWE, write a logical one to EEWE.
 */

void EEPROM_INTERNAL_WRITE(short int address, char data){
    if (address > 0x3FF)
        return;
    
    WAIT_EEWE;
    
    EEAR = address;
    EEAR = data;
    
    EECR = (EECR | 1<<EEMWE) & ~(1<<EEWE);
    EECR |= 1<<EEWE;
}
char EEPROM_INTERNAL_READ(short int address){
    if (address > 0x3FF)
        return -1;
    
    WAIT_EEWE;
    
    EEAR = address;
    EECR |= (1<<EERE);
    
    return EEDR;
}
void EEPROM_INTERNAL_INT_EN(){
    //The EEPROM Ready interrupt generates a constant interrupt when EEWE is cleared.
    EECR |= (1<<EERIE);
}
void EEPROM_INTERNAL_INT_DIS(){
    EECR &= ~(1<<EERIE);
}

///////////////  3) INTERNALL EEPROM   ///////////////////////////////