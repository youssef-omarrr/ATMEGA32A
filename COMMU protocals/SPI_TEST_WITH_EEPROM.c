#include <avr/io.h>
#include "mHEADERS.h"
#include "SPI_SLAVE_TEST.h"

int main() {
    
    initBUTTON();
    initLCD4();
    initSPI(MASTER);
    SPIclk(CLK_128);  
    
    char* str = "hello world";
    EE_SEND_DATA_STR(0x01, str);

    //must put delay to give time for SS to go back to HIGH
    _delay_us(100);
    EE_SEND_DATA(0xF0, 'z');
    
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
