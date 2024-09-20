#define  F_CPU 16000000UL
#include <util/delay.h>

#include "SPI_SLAVE_TEST.h"
#include "mSPI.h"
#include "DIO.h"
#include "mLCD4.h"

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