
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include<avr/interrupt.h>

#include "DIO.h"
#include "LEDs.h"
#include "buttons.h"
#include "outdev.h"
#include "mLCD.h"
#include "mLCD4.h"
#include "keypad.h"
#include "intr.h"
#include "mADC.h"

char x;

ISR(ADC_vect) {
    LCD4_data_num(ADC);
    _delay_ms(1000);
    LCD4_CLEAR();
    input();
}

int main() {
    
    initLCD4();
    initKEYPAD();
    LCD4_CLEAR();
    
    // ADC
    initADC (0,AVCC, ADC_freq_128);
    
    //global interrupt enable
        sei();

    //pooling //same idea as delay
        //checkFlag; //checks if flag is one and keeps looping until it is one

    while (1) {
        input();
    }

    return 0;
}

void input(){
    
    LCD4_goto(0, 0);
        LCD4_data_str("Choose (1,2,3)");
        LCD4_goto(1, 0);
        
        char x = readkey();
        LCD4_data(x);
        _delay_ms(200);
        LCD4_goto(0, 0);
        
        //select channel according to button pressed
        if (x == '1'){
            LCD4_CLEAR();
            LCD4_data_str("Channel 1 data:");
            LCD4_goto(1, 0);
            
            ADC_select_ch(0);
            _delay_us(50);
            ADC_startConv();
            
        }
        else if (x == '2'){
            LCD4_CLEAR();
            LCD4_data_str("Channel 2 data:");
            LCD4_goto(1, 0);
            
            ADC_select_ch(1);
            _delay_us(50);
            ADC_startConv();
        }
        else if (x == '3'){
            LCD4_CLEAR();
            LCD4_data_str("Channel 3 data:");
            LCD4_goto(1, 0);
            
            ADC_select_ch(2);
            _delay_us(50);
            ADC_startConv();
        }
        else {
            LCD4_CLEAR();
            LCD4_data_str("Wrong input!");
            _delay_ms (500);
            LCD4_CLEAR();
            input();
        }
    
}
