#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include<avr/interrupt.h>

#include "DIO.h"
#include "LEDs.h"
#include "buttons.h"
#include "outdev.h"
//#include "mLCD.h"
#include "mLCD4.h"
#include "keypad.h"
#include "intr.h"
#include "mADC.h"
#include "mtimer.h"

char x;

ISR(TIMER1_CAPT_vect){
    LCD4_CLEAR();
    LCD4_data_num(timer1_ICP_getValue());
}

int main() {
    
    //LCD
    initLCD4();
    LCD4_CLEAR();
    
    // ADC
    initADC(CH0, AVCC, ADC_freq_128);
    
    //timer
    timer1_ICP_init();
    timer1_ICP_noise_filter();
    timer1_ICP_int_en();
    
    initTimer1(TIMER_NORMAL, CLK_1024);
    //global interrupt enable
    sei();
    
    while (1) {
    }

    return 0;
}
