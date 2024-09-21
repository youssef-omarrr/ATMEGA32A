
#include <avr/io.h>
#include "mI2C.h"

void initTWI(char my_address){
    TWAR = my_address; //assign device address
    TWBR = 18; //to generate 100kHz while TWPS bits are zero
}

char TWI_MTX(char slave_add_write, char data){
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
    
    //3)send slave address i want to write to
    TWDR = slave_add_write;       //write means LSB is zero
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    //4)read status register
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x18)
    //if (!(statuscode == I2C_STATUS_MT_SLA_ACK) || (statuscode == I2C_STATUS_MT_SLA_NO_ACK))
    if (statuscode != I2C_STATUS_MT_SLA_ACK)
        return ERROR;
    //send stop condition if not ACK
    if (statuscode == I2C_STATUS_MT_SLA_NO_ACK)
    {
        //send stop condition
        ENABLE_FUNCTION | (1<< TWSTO);
        while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
        return ERROR;
        
    }
    
    //5)send data to slave
    TWDR = data;
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    //6)read status register
    statuscode = TWSR & 0xF8;
    //check for error 
    if (statuscode != I2C_STATUS_MT_DATA_ACK)
        return ERROR;
    
    //send stop condition if not ACK
    if (statuscode == I2C_STATUS_MT_DATA_NO_ACK)
    {
        //send stop condition
        ENABLE_FUNCTION | (1<< TWSTO);
        while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
        return ERROR;
        
    }
    
    //7)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    return 1;
}

char TWI_MTX_buf(char slave_add_write, char* pdata, char size){
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
    
    //3)send slave address i want to write to
    TWDR = slave_add_write;       //write means LSB is zero
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    //4)read status register
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x18)
    //if (!(statuscode == I2C_STATUS_MT_SLA_ACK) || (statuscode == I2C_STATUS_MT_SLA_NO_ACK))
    if (statuscode != I2C_STATUS_MT_SLA_ACK)
        return ERROR;
    //send stop condition if not ACK
    if (statuscode == I2C_STATUS_MT_SLA_NO_ACK)
    {
        //send stop condition
        ENABLE_FUNCTION | (1<< TWSTO);
        while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
        return ERROR;
        
    }
    
    ////////////// NEW //////////////////////
    //5)send data to slave byte by byte
    for (int i=0; i< size; i++){
        TWDR = pdata[i];
    
        ENABLE_FUNCTION;
        WAIT_TWINT;
    
        //6)read status register
        statuscode = TWSR & 0xF8;
        //check for error 
        if (statuscode != I2C_STATUS_MT_DATA_ACK)
            return ERROR;

        //send stop condition if not ACK
        if (statuscode == I2C_STATUS_MT_DATA_NO_ACK)
        {
            //send stop condition
            ENABLE_FUNCTION | (1<< TWSTO);
            while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
            return ERROR;

         }
    }
    ////////////// NEW //////////////////////
    
    //7)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    return 1;
    
}
///////////////////////////////////////////////////////////////////////////////////////////
char TWI_MRX(char slave_add_read){
    char statuscode = 0;
    char data = 0;
    
    //1)start condition (enable function ORed with start condition (TWSTA) bit)
    ENABLE_FUNCTION | (1<< TWSTA);
    //wait for flag to be 1 (data bus is free after sending all data)
    WAIT_TWINT;
    
    //2)read status register (& 0xF8 to clear pre-scaler effect)
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x08)
    if (statuscode != I2C_STATUS_MT_START)
        return ERROR;
    
    //3)send slave address i want to read from
    TWDR = slave_add_read;       //read means LSB is one
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    // 4) Check for SLA+R ACK
    statuscode = TWSR & 0xF8;
    if (statuscode == I2C_STATUS_MR_SLA_NO_ACK) {
        // Send stop condition if no ACK
        ENABLE_FUNCTION | (1<< TWSTO);
        WAIT_TWINT; // Wait until stop condition is executed
        return ERROR;
    }
    else if (statuscode != I2C_STATUS_MR_SLA_ACK)
        return ERROR;
    
    //5) enable ACK (for next data byte) BEFORE reading data
    
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
   
    // 6) Check if data was received with ACK (0x50)
    statuscode = TWSR & 0xF8;
    if (statuscode == I2C_STATUS_MR_DATA_NO_ACK) {
        // Send stop condition if no ACK
        ENABLE_FUNCTION | (1<< TWSTO);
        while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
        return ERROR;
    }
    else if (statuscode != I2C_STATUS_MR_DATA_ACK)
        return ERROR;
    
    //7) if not error read and save data
    data = TWDR;
    
    //8)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    // Return the received data
    return data;
}

char TWI_MRX_buf(char slave_add_read, char* pdata, char size){
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
    
    //3)send slave address i want to read from
    TWDR = slave_add_read;       //read means LSB is one
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    //4)read status register
    statuscode = TWSR & 0xF8;
    
    //check for error (if it worked it must return 0x40 or 0x48) 
    if (statuscode != I2C_STATUS_MR_SLA_ACK)
        return ERROR;
    //send stop condition if not ACK
    if (statuscode == I2C_STATUS_MR_SLA_NO_ACK)
    {
        //send stop condition
        ENABLE_FUNCTION | (1<< TWSTO);
        while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
        return ERROR;
        
    }
    
    ////////////// NEW //////////////////////
    //NEW:repeat these steps to get all the data (or until NOT ACK is sent from slave)
    for (int i=0; i< size; i++){
        
        //5) enable ACK (for next data byte) BEFORE reading data
        if (i < size - 1) {
            // For all but the last byte, send ACK
            ENABLE_FUNCTION | (1<< TWEA);  // Enable ACK
        } else {
            // For the last byte, send NACK (to stop slave from sending anything else)
            ENABLE_FUNCTION; // Disable ACK (NACK will be sent)
        }
        WAIT_TWINT;

        //6)read status register
        statuscode = TWSR & 0xF8;
        //check for error (if it worked it must return 0x50 or 0x58)
        
        // Handle the last byte, expecting a NACK
        if (statuscode == I2C_STATUS_MR_DATA_NO_ACK && i == size - 1) {
            // Expected NACK for the last byte, proceed without error
        }

        // Handle intermediate bytes, expecting ACK
        else if (statuscode != I2C_STATUS_MR_DATA_ACK && i < size - 1) {
            // Expected ACK for intermediate bytes
            return ERROR;
        }

        // Handle NACK received unexpectedly for intermediate byte (error condition)
        else if (statuscode == I2C_STATUS_MR_DATA_NO_ACK && i < size - 1) {
            // Send stop condition and return error
            ENABLE_FUNCTION | (1<< TWSTO);
            WAIT_TWINT; // Wait until stop condition is executed
            return ERROR;
        }

        //7) if not error read and save data
        pdata[i] = TWDR;
    }
    ////////////// NEW //////////////////////
    
    //8)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    return 1; //data sent successfully
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

char TWI_SRX(){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ack(TWEA) bit)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    
    //2)read status register
    statuscode = TWSR & 0xF8;
    
    //wait till acknowledge is sent to master then receive data
    if (statuscode != I2C_STATUS_SR_SLA_ACK && statuscode != I2C_STATUS_SR_GEN_ACK)
        return ERROR;
    
    //3)ready to receive data (set acknowledge enable)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;

    //4)read status register
    statuscode = TWSR & 0xF8;
    //check for error 
    if (statuscode != I2C_STATUS_SR_DATA_ACK && statuscode != I2C_STATUS_SR_GEN_DATA_ACK)
        return ERROR;
 
    //5)return data
    return TWDR;
    
}

char TWI_SRX_buf(char* pdata, char size){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ack(TWEA) bit)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    
    //2)read status register
    statuscode = TWSR & 0xF8;
    
    //wait till acknowledge is sent to master then receive data
    if (statuscode != I2C_STATUS_SR_SLA_ACK && statuscode != I2C_STATUS_SR_GEN_ACK)
        return ERROR;
    
    ////////////// NEW //////////////////////
    for (int i=0; i<size; i++){
        //3)ready to receive data (set acknowledge enable)
        ENABLE_FUNCTION | (1<< TWEA);
        WAIT_TWINT;

        //4)read status register
        statuscode = TWSR & 0xF8;
        //check for error 
        if (statuscode != I2C_STATUS_SR_DATA_ACK && statuscode != I2C_STATUS_SR_GEN_DATA_ACK)
            return ERROR;

        //5)return data byte by byte
        pdata[i] = TWDR ;
    }
    ////////////// NEW //////////////////////
    
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////

char TWI_STX(char data){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ACK(TWEA) bit)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    //wait till acknowledge is sent to master then send data
    
    //2)read status register
    statuscode = TWSR & 0xF8;
    
    // Check if the slave address was acknowledged
    if (statuscode != I2C_STATUS_ST_SLA_ACK)
        return ERROR;
        
    
    //3)ready to send data
    TWDR = data;
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    //wait till acknowledge is sent from master that the data has been received
    
    //4)read status register
    statuscode = TWSR & 0xF8;
    
    // Check if the data was acknowledged
    if (statuscode != I2C_STATUS_ST_DATA_ACK)
        return ERROR;
    
    return 1; //data sent successfully
    
}

char TWI_STX_buf(char* pdata, char size){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ACK(TWEA) bit)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    //wait till acknowledge is sent to master then send data
    
    //2)read status register
    statuscode = TWSR & 0xF8;
    
    // Check if the slave address was acknowledged
    if (statuscode != I2C_STATUS_ST_SLA_ACK)
        return ERROR;
        
    ////////////// NEW //////////////////////
    
    //3)ready to send data byte by byte
    for (int i=0; i< size; i++){
        TWDR = pdata[i];

        ENABLE_FUNCTION;
        WAIT_TWINT;
        //wait till acknowledge is sent from master that the data has been received

        //4)read status register
        statuscode = TWSR & 0xF8;

        // Check if the data was acknowledged
        if (statuscode == I2C_STATUS_ST_DATA_NO_ACK) {
            return ERROR; // Data was not acknowledged
        } 
        else if (statuscode != I2C_STATUS_ST_DATA_ACK) {
            return ERROR; // Other error
        }
    }
    ////////////// NEW //////////////////////
    
    return 1; //data sent successfully
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

/**
 * 
 * @param clk   TWI_CLK_1
 *              TWI_CLK_4
 *              TWI_CLK_16
 *              TWI_CLK_64
 */

void TWI_CLK(char clk){
    //reset them first
    TWSR &= ~( (1<<TWPS0) | (1<<TWPS1));
    
    switch (clk){
        case (TWI_CLK_1):
            TWSR |= clk;
            break;
        case(TWI_CLK_4):
            TWSR |= clk;
            break;
        case(TWI_CLK_16):
            TWSR |= clk;
            break;
        case(TWI_CLK_64):
            TWSR |= clk;
            break;
    }
}


void TWI_INT_EN(){
    TWCR |= (1<<TWIE);
}

char TWI_ADDRESS_WRITE(char address){
    //write means LSB is zero
    address = (address << 1); // Shift left to make room for R/W bit
    address &= 0xFE; //1111 1110
    return address;
}

char TWI_ADDRESS_READ(char address){
    //read means LSB is one
    address = (address << 1); // Shift left to make room for R/W bit
    address |= 0x01; //0000 0001
    return address;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
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
 * 4) send REPEATED ADDRESS
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