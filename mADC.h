/* 
 * File:   mADC.h
 * Author: User
 *
 * Created on August 17, 2024, 8:33 PM
 */

#ifndef MADC_H
#define	MADC_H

#define CH0     0
#define CH1     1
#define CH2     2
#define CH3     3
#define CH4     4
#define CH5     5
#define CH6     6
#define CH7     7

#define AREF        0   //00
#define AVCC        1   //01
#define INTERNAL    3   //11
                        //10 RESERVED!!

#define ADC_freq_128    7
#define ADC_freq_64     6
#define ADC_freq_32     5
#define ADC_freq_16     4
#define ADC_freq_8      3
#define ADC_freq_4      2
#define ADC_freq_2      1

#define checkFlag       while (!(ADCSRA & (1<<ADIF))) //checks if flag is one and keeps looping until it is one (variable delay)

void initADC(char ch, char ref, char freq);
void ADC_select_ch(char);
void ADC_select_ref(char);
void ADC_select_freq(char);
void ADC_startConv(); //start conversion
void ADC_en(); //enable ADC

void ADC_int_en();

//to get data from data register write "ADCW"


#endif	/* MADC_H */

