#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "DIO.h"
#include "LEDs.h"
#include "buttons.h"
#include "outdev.h"
#include "mLCD.h"
#include "mLCD4.h"
#include "keypad.h"
//or we can define messege [] = "hello"

int main(){
//    initBUTTON();
//    initLED();
//    initRELAY();
//    initBUZZER();
    initKEYPAD();
    _delay_ms(5);
    initLCD4(); 
    _delay_ms(50);
     //and write here LCD_DATA_STR(messege)
    while(1){
//        LCD4_goto(1,3);
        char x = readkey();
        if (x == 'c')
            LCD4_CLEAR();
        else
            LCD4_data(x);
        _delay_ms(200);


      }
    return 0;
}