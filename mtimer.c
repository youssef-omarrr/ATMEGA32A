#include <avr/io.h>
#include "mtimer.h"
#include "DIO.h"

////////////////////////////timer0///////////////////////////////////////
/**
 * 
 * @param wave TIMER_NORMAL      
               TIMER_PWM         
               TIMER_CTC         
               TIMER_FAST_PWM    
 * 
 * @param clk   CLK_NO           
                CLK              
                CLK_8            
                CLK_64           
                CLK_256          
                CLK_1024         
                CLK_EX_FALLING   
                CLK_EX_RISING
 */
void initTimer0(char wave, char clk) { //we should make sure that everything is init before we init the timer

    //choose Waveform Generation Mode
    timer0_selectMode(wave);

    //choose Clock Select Bit Description
    timer0_select_clock(clk);
}

void timer0_selectMode(char wave) {
    //choose Waveform Generation Mode
    switch (wave) {
            TCCR0 &= ~(1 << WGM01 | 1 << WGM00); //set them to zero first

        case(TIMER_NORMAL)://00
            //no change already zero
            timer0_TOV_int_en(); //enable overflow toggle 
            break;

        case(TIMER_PWM)://01
            TCCR0 |= (1 << WGM00);
            break;

        case(TIMER_CTC)://10
            OCR0 = CTC_TOP;
            TCCR0 |= (1 << WGM01);
            timer0_COMP_int_en(); //enable compare output 
            break;

        case(TIMER_FAST_PWM)://11
            TCCR0 |= (1 << WGM01 | 1 << WGM00);
            break;
    }

}

void timer0_select_clock(char clk) {
    //choose Clock Select Bit Description0
    TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); //set them to zero first
    TCCR0 |= clk;
}

void timer0_TOV_int_en() {
    TIMSK |= (1 << TOIE0);
}

void timer0_COMP_int_en() {
    TIMSK |= (1 << OCIE0);
}

void timer0_OC0_init(){ //in the main we should init this first before the timer
    setPINB_dir(OC0, OUT); //make sure we set it to out put first;
}

/**
 * 
 * @param mode  OC0_CTC_NORMAL      
                OC0_CTC_TOGGLE      
                OC0_CTC_CLEAR       
                OC0_CTC_SET         

                OC0_PWM_CLEAR_UP    
                OC0_PWM_SET_UP      

                OC0_FPWM_CLEAR      
                OC0_FPWM_SET        
 */

void timer0_OC0_selectMode(char mode){
    
    TCCR0 &= ~(1<<COM00 | 1<<COM01); //reset first
    TCCR0 |= (mode<<COM00); //then OR the selected mode shifted by the place of the select mode bit
    
}

void timer0_prescaler_reset(){
    SFIOR |= (1<<PSR10); //When this bit is written to one, the Timer/Counter1 and Timer/Counter0 prescaler will be reset. The bit will be
                         //cleared by hardware after the operation is performed.
}
////////////////////////////timer2///////////////////////////////////////
/**
 * 
 * @param wave TIMER_NORMAL      
               TIMER_PWM         
               TIMER_CTC         
               TIMER_FAST_PWM    
 * 
 * @param clk   CLK_NO           
                CLK              
                CLK_8            
                CLK_64           
                CLK_256          
                CLK_1024         
                CLK_EX_FALLING   
                CLK_EX_RISING
 */
void initTimer2(char wave, char clk) { //we should make sure that everything is init before we init the timer

    //choose Waveform Generation Mode
    timer2_selectMode(wave);

    //choose Clock Select Bit Description
    timer2_select_clock(clk);
}

void timer2_selectMode(char wave) {
    //choose Waveform Generation Mode
    switch (wave) {
            TCCR2 &= ~(1 << WGM21 | 1 << WGM20); //set them to zero first

        case(TIMER_NORMAL)://00
            //no change already zero
            timer0_TOV_int_en(); //enable overflow toggle 
            break;

        case(TIMER_PWM)://01
            TCCR2 |= (1 << WGM20);
            break;

        case(TIMER_CTC)://10
            OCR0 = CTC_TOP;
            TCCR2 |= (1 << WGM21);
            timer0_COMP_int_en(); //enable compare output 
            break;

        case(TIMER_FAST_PWM)://11
            TCCR2 |= (1 << WGM21 | 1 << WGM20);
            break;
    }

}

void timer2_select_clock(char clk) {
    //choose Clock Select Bit Description0
    TCCR2 &= ~(1 << CS22 | 1 << CS21 | 1 << CS20); //set them to zero first
    TCCR2 |= clk;
}

void timer2_TOV_int_en() {
    TIMSK |= (1 << TOIE2);
}

void timer2_COMP_int_en() {
    TIMSK |= (1 << OCIE2);
}

void timer2_OC2_init(){ //in the main we should init this first before the timer
    setPINB_dir(OC2, OUT); //make sure we set it to out put first;
}

/**
 * 
 * @param mode  OC2_CTC_NORMAL      
                OC2_CTC_TOGGLE      
                OC2_CTC_CLEAR       
                OC2_CTC_SET         

                OC2_PWM_CLEAR_UP    
                OC2_PWM_SET_UP      

                OC2_FPWM_CLEAR      
                OC2_FPWM_SET        
 */

void timer2_OC2_selectMode(char mode){
    
    TCCR2 &= ~(1<<COM21 | 1<<COM20); //reset first
    TCCR2 |= (mode<<COM20); //then OR the selected mode shifted by the place of the select mode bit
    
}

/**
 * 
 * @param mode clk_io input/output clock
 *             clk_osc oscillator clock
 */
void timer2_asynch(char mode){ //choose which clock to use
    
    // 1) clear interrupts enables
    TIMSK &= ~(1<<TOIE2 | 1<<OCIE2); 
    // 2) select clock mode
    switch (mode){
        case(clk_io):
            ASSR &= ~(1<<AS2);
            break;
        case (clk_osc):
            ASSR |= (1<<AS2);
            break;
    }
    
    // 3) write new values for TCNT2, OCR2, TCCR2
    // 4) wait for TCN2UB, OCR2UB, TCR2UB
    // 5) clear interrupt flags
    TIFR &= ~(1<<OCF2 | 1<<TOV2);
    
    // 6) enable interrupt, if needed
}

void timer2_prescaler_reset(){
    SFIOR |= (1<<PSR2);
}
