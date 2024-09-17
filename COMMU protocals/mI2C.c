
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
    if (statuscode != I2C_STATUS_MT_SLA_ACK && statuscode != I2C_STATUS_MT_SLA_NO_ACK)
        return ERROR;
    
    //5)send data to slave
    TWDR = data;
    
    ENABLE_FUNCTION;
    WAIT_TWINT;
    
    //6)read status register
    statuscode = TWSR & 0xF8;
    //check for error 
    if (statuscode != I2C_STATUS_MT_DATA_ACK && statuscode != I2C_STATUS_MT_DATA_NO_ACK)
        return ERROR;
    
    //7)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    return 1;
}

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
    
    //4)read status register
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x40 or 0x48)
    if (statuscode != I2C_STATUS_MR_SLA_ACK && statuscode != I2C_STATUS_MR_SLA_NO_ACK)
        return ERROR;
    
    //5) enable ACK (for next data byte) BEFORE reading data
    
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    
    data = TWDR;
    
    //6)read status register
    statuscode = TWSR & 0xF8;
    //check for error (if it worked it must return 0x50 or 0x58)
    if (statuscode != I2C_STATUS_MR_DATA_ACK && statuscode != I2C_STATUS_MR_DATA_NO_ACK)
        return ERROR;
    
    //7)send stop condition
    ENABLE_FUNCTION | (1<< TWSTO);
    while (TWCR & (1<<TWSTO)); // Wait until stop condition is executed
    
    // Return the received data
    return data;
}




char TWI_SRX(){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ack(TWEA) bit)
    ENABLE_FUNCTION | (1<< TWEA);
    WAIT_TWINT;
    
    //2)read status register
    statuscode = TWSR & 0xF8;
    
    //wait till acknowledge is sent to master then receive data
    if (statuscode == I2C_STATUS_SR_SLA_ACK || statuscode == I2C_STATUS_SR_GEN_ACK){
        //3)ready to receive data (set acknowledge enable)
        ENABLE_FUNCTION | (1<< TWEA);
        WAIT_TWINT;

        //4)read status register
        statuscode = TWSR & 0xF8;
        //check for error 
        if (statuscode != I2C_STATUS_SR_DATA_ACK && statuscode != I2C_STATUS_SR_GEN_DATA_ACK)
            return ERROR;
    }
    else{
        return ERROR;
    }
 
    //5)return data
    return TWDR;
    
}

char TWI_STX(char data){
    char statuscode = 0;
    
    //1)ready to receive my address (enable function ORed with enable ack(TWEA) bit)
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
    
    return 1;
    
}





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