#include <avr/io.h>
#include "mHEADERS.h"
#include "SPI_SLAVE_TEST.h"


////define addresses for now (can change it later)
//#define MYADDRESS       0x07
////slave address  0x0A  -> 0000 1010
//#define SLA_W   0x0A //write means LSB is zero
//#define SLA_R   0x0B //read means LSB is one


int main() {
    
    initBUTTON();
    initLCD4();
    initSPI(MASTER);
    SPIclk(CLK_128);
    
    
    //i have to write this first else it prints 'h' every time
    EE_SEND_DATA(0xF0, 'z');
    
    char* str = "hello world";
    EE_SEND_DATA_STR(0x01, str);
    
    
    while (1) {
        if (isPressed(BUTTON0))
            {
            //to read a string you must read char by char
            for (int i=0; str[i] != '\0'; i++){
                LCD4_data(EE_READ_DATA(0x01 + i));
                _delay_ms(100);
            }
        }
        
        if (isPressed(BUTTON1)){
            LCD4_data(EE_READ_DATA(0xF0));
            _delay_ms(200);
        }
       
    }

    return 0;
}
