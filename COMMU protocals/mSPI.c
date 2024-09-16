#include <avr/io.h>
#include "DIO.h"
#include "mSPI.h"

/**
 * 
 * @param mode      MASTER
 *                  SLAVE
 */
void initSPI(char mode){
    
    //reset first then choose master or slave
    SPCR &= ~(1<<MSTR);
    SPCR |= (mode<<MSTR);
    
    //now we have to ensure that the user defined pins are outputs
    //(the rest of the pin are inputs by default) see page 137
    switch (mode){
        case(MASTER):
            setPORTB_dir_val((1<<MOSI) | (1<<SCK) | (1<<SS), OUT);
            break;
        case(SLAVE):
            setPINB_dir(MISO, OUT);
            break;
    }
    
    //enable SPI
    SPCR |= (1<<SPE);
}

/**
 * 
 * @param clk   CLK_2   
                CLK_4   
                CLK_8   
                CLK_16  
                CLK_32  
                CLK_64_  
                CLK_128 
 */
void SPIclk(char clk){
    //reset first
    SPCR &= ~(1<<SPR0 | 1<<SPR1);
    SPSR &= ~(1<<SPI2X);
    
    switch (clk){
        case(CLK_2)://100
            SPSR |= (1<<SPI2X);
            break;
        case(CLK_4)://000
            break;
        case(CLK_8)://101
            SPCR |= (1<<SPR0);
            SPSR |= (1<<SPI2X);
            break;
        case(CLK_16)://001 or //010
            SPCR |= (1<<SPR0);
            break;
        case(CLK_32)://110
            SPCR |= (1<<SPR1);
            SPSR |= (1<<SPI2X);
            break;
        case(CLK_64_)://111
            SPCR |= (1<<SPR0 | 1<<SPR1);
            SPSR |= (1<<SPI2X);
            break;
        case(CLK_128)://011
            SPCR |= (1<<SPR0 | 1<<SPR1);
            break;
    }
}

void SPI_INT_EN(){
    //enable interrupt
    SPCR |= (1<<SPIE);
}

/*A timeout is a predefined period of time after which a certain operation is stopped if it hasn't completed yet. 
 *It is used to prevent a system from waiting indefinitely for an event that might never happen, 
 *such as receiving data in communication or waiting for a process to finish.
 */

void SPIsend(char data){
    SPDR = data;
    uint16_t timeout = 1000;  // Set a timeout value
    //wait for transmission to complete
    while (!(SPSR & (1<<SPIF)) && --timeout); //When a serial transfer is complete, the SPIF Flag is set
}

void SPIsend_str(char* data){
    for (int i=0; data[i] != '\0'; i++){
        SPIsend (data [i]);
    }
}
char SPIreceive(){
    uint16_t timeout = 1000;  // Set a timeout value
    // Wait for reception complete 
    while (!(SPSR & (1<<SPIF)) && --timeout); //When a serial transfer is complete, the SPIF Flag is set
    return SPDR;
}

char SPI_MASTER_receive(){   
    SPIsend(0x00);
    char data = SPDR;
    return data;
}

/*to send data to a device like memory card
1) send instruction (send data, go home, ...)
2) send address
3) send data
 
---- to receive data we send a dummy data like (0x00) just to push
     the shift registers to send the data in the device back to the master */

/**
 * 
 * @param order MSB_FIRST, LSB_FIRST
 */
void SPI_DATA_ORDER(char order){
    switch (order){
        case(MSB_FIRST):
            SPCR &= ~(1<<DORD);
            break;
        case(LSB_FIRST):
            SPCR |= (1<<DORD);
            break;
    }
}

void SPI_selectSlave(int slavePin) {
    // Pull SS low to select the slave
    setPINB(slavePin, LOW);
}

void SPI_deselectSlave(int slavePin) {
    // Pull SS high to deselect the slave
    setPINB(slavePin, HIGH);
}

//for error handling
char SPI_check_collision() {
    return (SPSR & (1<<WCOL)) ? 1 : 0;  // Return 1 if write collision occurred
}
