#include <avr/io.h>
#include "mtimer.h"

void initTimer(char wave, char clk){
    
    //choose Waveform Generation Mode
    timer0_selectMode (wave);
    
    //choose Clock Select Bit Description
    timer0_select_clock (clk);
}

void timer0_selectMode (char wave){
    //choose Waveform Generation Mode
    switch (wave){
        TCCR0 &= ~(1<<WGM01 | 1<< WGM00); //set them to zero first
        
        case(TIMER_NORMAL)://00
            //no change already zero
            timer0_TOV_int_en();
            break;
            
        case(TIMER_PWM)://01
            TCCR0 |= (1<<WGM00);
            break;
            
        case(TIMER_CTC)://10
            TCCR0 |= (1<<WGM01);
            timer0_COMP_int_en();
            break;
            
        case(TIMER_FAST_PWM)://11
            TCCR0 |= (1<<WGM01 | 1<< WGM00);
            break;         
    }
    
}
void timer0_select_clock (char clk){
    //choose Clock Select Bit Description
    switch (clk){
        TCCR0 &= ~(1<<CS02 | 1<<CS01 | 1<<CS00); //set them to zero first
                 
        case(CLK_NO)://000
            //no change already zero
            break;
            
        case(CLK)://001
            TCCR0 |= (1<<CS00);
            break;
            
        case(CLK_8)://010
            TCCR0 |= (1<<CS01);
            break;
            
        case(CLK_64)://011
            TCCR0 |= (1<<CS01 | 1<<CS00);
            break;
            
        case(CLK_256)://100
            TCCR0 |= (1<<CS02);
            break;
            
        case(CLK_1024)://101
            TCCR0 |= (1<<CS02 | 1<<CS00);
            break;
            
        case(CLK_EX_FALLING)://110
            TCCR0 |= (1<<CS02 | 1<<CS01);
            break;
            
        case(CLK_EX_RISING)://111
            TCCR0 |= (1<<CS02 | 1<<CS01 | 1<<CS00);
            break;
}
    //or we could have just ORed the clk with the TCCR0 3ala tol
    //TCCR0 |= clk
}

void timer0_TOV_int_en(){
    TIMSK |= (1<<TOIE0);
}
void timer0_COMP_int_en(){
    TIMSK |= (1<<OCIE0);
}