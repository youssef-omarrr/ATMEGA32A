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
    //ADC_int_en();
    
    //enable the ADC peripheral
    ADC_en();
}

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
