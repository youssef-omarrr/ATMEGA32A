#include <avr/io.h>
#include "mADC.h"

void initADC(char ch, char ref, char freq){
    //select channel
    ADC_select_ch(ch);
    
    //select reference
    ADC_select_ref(ref);
    
    //select frequency
    ADC_select_freq(freq);
    
    //enable interrupt if needed
    ADC_int_en();
    
    //auto trigger enable
    ADC_auto_triggering(TIMER0_TOV);
    
    //enable the ADC peripheral
    ADC_en();
}

/**
 * 
 * @param ch from CH0 to CH7
 */
void ADC_select_ch(char ch) {
    ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0)); //reset bits first before changing the channel
    ADMUX |= ch;
}

/**
 * 
 * @param ref   AREF        0   //00
                AVCC        1   //01
                INTERNAL    3   //11
                                //10 RESERVED!!
 */
void ADC_select_ref(char ref) {
    ADMUX &= ~((1 << REFS1) | (1 << REFS0)); //reset them first

    ADMUX |= (ref << 6); //REF0 is bit number 6
}

void ADC_select_freq(char freq) {
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); //reset bits first before changing the frequency

    ADCSRA |= freq;
}

void ADC_startConv() { //start conversion
    ADCSRA |= (1 << ADSC);
}

void ADC_en() { //enable ADC
    ADCSRA |= (1 << ADEN);
}

void ADC_int_en(){ //enable the ADC interrupt
    ADCSRA |= (1 << ADIE);
}

/**
 * 
 * @param mode  EXTERNAL_INTO   
                TIMER0_TOV      
                TIMER0_COM      
                TIMER1_TOV     
                TIMER1_COMB     
 */
void ADC_auto_triggering(char mode){
    SFIOR &= ~(1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0); //reset first
    switch (mode){
        case (EXTERNAL_INTO)://010
            SFIOR |= (1<<ADTS1);
            break;
        case(TIMER0_TOV)://100
            SFIOR |= (1<<ADTS2);
            break;
        case(TIMER0_COM)://011
            SFIOR |= (1<<ADTS0);
            SFIOR |= (1<<ADTS1);
            break;
        case(TIMER1_TOV)://110
            SFIOR |= (1<<ADTS1);
            SFIOR |= (1<<ADTS2);
            break;
        case(TIMER1_COMB)://101
            SFIOR |= (1<<ADTS0);
            SFIOR |= (1<<ADTS2);
            break;
    }
}

int ADC_read() {
    int data = 0;
    if (ADMUX & (1 << ADLAR)) {
        // If ADLAR is set (left adjusted)
        data = (ADCH << 2); // Take the 8 most significant bits from ADCH
        data |= (ADCL >> 6); // Take the 2 least significant bits from ADCL
    } else {
        // If ADLAR is clear (right adjusted)
        data = ADCL; // Take the entire ADCL (low byte)
        data |= (ADCH << 8); // Add the 2 bits from ADCH shifted to their correct positions
    }
    return data;
}