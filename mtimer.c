#include <avr/io.h>
#include "mtimer.h"
#include "DIO.h"

/////////////////////////////////////////////////////////////////////////
////////////////////////////timer0///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////
////////////////////////////timer2///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/**
 * 
 * @param wave TIMER_NORMAL      
               TIMER_PWM         
               TIMER_CTC         
               TIMER_FAST_PWM    
 * 
 * @param clk   //Clock Select Bit Description
        #define CLK2_NO           0
        #define CLK2              1
        #define CLK2_8            2
        #define CLK2_32           3
        #define CLK2_64           4
        #define CLK2_128          5
        #define CLK2_256          6
        #define CLK2_1024         7
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
            timer2_TOV_int_en(); //enable overflow toggle 
            break;

        case(TIMER_PWM)://01
            TCCR2 |= (1 << WGM20);
            break;

        case(TIMER_CTC)://10
            OCR2 = CTC_TOP;
            TCCR2 |= (1 << WGM21);
            timer2_COMP_int_en(); //enable compare output 
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
    setPIND_dir(OC2, OUT); //make sure we set it to output first;
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
 *             clk_osc oscillator clock (Asynch)
 */
void timer2_asynch(char mode){ //choose which clock to use
    
    char x,y,z;
    x = TCNT2;
    y = OCR2;
    z = TCCR2;
    
    //Configure TOSC1 (PB6) and TOSC2 (PB7) as input pins for the crystal
    DDRB &= ~((1 << PB6) | (1 << PB7)); // Set TOSC1 and TOSC2 as input
    
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
    TCNT2 = x;
    OCR2  = y;
    TCCR2 = z;
    
    // 4) wait for TCN2UB, OCR2UB, TCR2UB to synchronize
    while (ASSR & ((1 << TCR2UB) | (1 << OCR2UB) | (1 << TCN2UB)));
    
    // 5) clear interrupt flags
    TIFR &= ~(1<<OCF2 | 1<<TOV2);
    
    // 6) enable interrupt, if needed
    //timer2_TOV_int_en();
    //timer2_COMP_int_en();
}

void timer2_prescaler_reset(){
    SFIOR |= (1<<PSR2);
}
/////////////////////////////////////////////////////////////////////////
////////////////////////////timer1///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/**
 * 
 * @param wave  TIMER_NORMAL        
 * 
                TIMER_PWM_8BIT      
                TIMER_PWM_9BIT      
                TIMER_PWM_10BIT     

                TIMER_CTC_OCR1A     

                TIMER_FAST_PWM_8BIT 
                TIMER_FAST_PWM_9BIT 
                TIMER_FAST_PWM_10BIT

                TIMER_PWM_FREQ_ICR1 
                TIMER_PWM_FREQ_OCR1A

                TIMER_PWM_ICR1      
                TIMER_PWM_OCR1      

                TIMER_CTC_ICR1      

                TIMER_FAST_PWM_ICR1 
                TIMER_FAST_PWM_OCR1     
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
void initTimer1(char wave, char clk){
    //choose Waveform Generation Mode
    timer1_selectMode(wave);

    //choose Clock Select Bit Description
    timer1_select_clock(clk);
}
void timer1_selectMode(char wave){
    //reset them first
    TCCR1A &= ~(1<<WGM11 | 1<<WGM10);
    TCCR1B &= ~(1<<WGM13 | 1<<WGM12);
                  //WGM13 WGM12   WGM11 WGM10
    switch(wave){
        case(TIMER_NORMAL)://00 00
            break;
            
        case(TIMER_PWM_8BIT)://00 01
            TCCR1A |= (1<<WGM10);
            break;
        case(TIMER_PWM_9BIT)://00 10
            TCCR1A |= (1<<WGM11);
            break;
        case(TIMER_PWM_10BIT)://00 11
            TCCR1A |= (1<<WGM11 | 1<<WGM10);
            break;
            
        case(TIMER_CTC_OCR1A)://01 00
            TCCR1B |= (1<<WGM12);
            break;
            
        case(TIMER_FAST_PWM_8BIT)://01 01
            TCCR1B |= (1<<WGM12);
            TCCR1A |= (1<<WGM10);
            break;
        case(TIMER_FAST_PWM_9BIT)://01 10
            TCCR1B |= (1<<WGM12);
            TCCR1A |= (1<<WGM11);
            break;
        case(TIMER_FAST_PWM_10BIT)://01 11
            TCCR1B |= (1<<WGM12);
            TCCR1A |= (1<<WGM11 | 1<<WGM10);
            break;
            
        case(TIMER_PWM_FREQ_ICR1)://10 00
            TCCR1B |= (1<<WGM13);
            break;
        case(TIMER_PWM_FREQ_OCR1A)://10 01
            TCCR1B |= (1<<WGM13);
             TCCR1A |= (1<<WGM10);
            break;
            
        case(TIMER_PWM_ICR1)://10 10
            TCCR1B |= (1<<WGM13);
            TCCR1A |= (1<<WGM11);
            break;
        case(TIMER_PWM_OCR1)://10 11
            TCCR1B |= (1<<WGM13);
            TCCR1A |= (1<<WGM11 | 1<<WGM10);
            break;
            
        case(TIMER_CTC_ICR1)://11 00
            TCCR1B |= (1<<WGM13 | 1<<WGM12);
            break;
                             //11 01 RESERVED
        case(TIMER_FAST_PWM_ICR1)://11 10
            TCCR1B |= (1<<WGM13 | 1<<WGM12);
            TCCR1A |= (1<<WGM11);
            break;
        case(TIMER_FAST_PWM_OCR1)://11 11
            TCCR1B |= (1<<WGM13 | 1<<WGM12);
            TCCR1A |= (1<<WGM11 | 1<<WGM10);
            break;
    }
}
void timer1_select_clock(char clk){
    TCCR1B &= ~((1<<CS10) | (1<<CS11) | (1<CS12)); //reset first
    TCCR1B |= clk;
}

///////////////////////////////////////////////////////////////////////////////
void timer1_TOV_int_en(){
    TIMSK |= 1<<TOIE1;
}

///////////////////////////////////////////////////////////////////////////////
void timer1A_COMP_int_en(){
    TIMSK |= 1<<OCIE1A;
}
void timer1_OC1A_init(){
    setPIND_dir(OC1A, OUT); //make sure we set it to output first;
}
/**
 * 
 * @param mode  OC1A_CTC_NORMAL      
                OC1A_CTC_TOGGLE      
                OC1A_CTC_CLEAR       
                OC1A_CTC_SET         

                OC1A_PWM_CLEAR_UP    
                OC1A_PWM_SET_UP      

                OC1A_FPWM_CLEAR      
                OC1A_FPWM_SET        
 */
void timer1_OC1A_selectMode(char mode){
    TCCR1A &= ~(1<<COM1A1 | 1<<COM1A0); //reset first
    TCCR1A |= (mode<<COM1A0); //then OR the selected mode shifted by the place of the select mode bit
}

///////////////////////////////////////////////////////////////////////////////
void timer1B_COMP_int_en(){
    TIMSK |= 1<<OCIE1B;
}
void timer1_OC1B_init(){
    setPIND_dir(OC1B, OUT); //make sure we set it to output first;
}
/**
 * 
 * @param mode  OC1B_CTC_NORMAL      
                OC1B_CTC_TOGGLE      
                OC1B_CTC_CLEAR       
                OC1B_CTC_SET         

                OC1B_PWM_CLEAR_UP    
                OC1B_PWM_SET_UP      

                OC1B_FPWM_CLEAR      
                OC1B_FPWM_SET        
 */
void timer1_OC1B_selectMode(char mode){
    TCCR1A &= ~(1<<COM1B1 | 1<<COM1B0); //reset first
    TCCR1A |= (mode<<COM1B0); //then OR the selected mode shifted by the place of the select mode bit
}

///////////////////////////////////////////////////////////////////////////////
void timer1_ICP_init(){
    setPIND_dir(ICP1, IN); //make sure we set it to input first;
    ACSR &= ~(1<<ACIC);
}
void timer1_ICP_int_en(){
    TIMSK |= 1<<TICIE1;
}
void timer1_ICP_noise_filter(){
    TCCR1B |= 1<<ICNC1;
}
/**
 * 
 * @param edge FALLING_EDGE
 *             RISING_EDGE
 */
void timer1_ICP_selectEdge(char edge){
    TCCR1B &= ~(1<<ICES1); //reset first
    TCCR1B |= edge<<ICES1;
}
short unsigned int timer1_ICP_getValue(){
    return ICR1;
}
void timer1_ACO_init(){
    setPINB_dir(AIN0, IN);
    setPINB_dir(AIN1, IN);
    ACSR |= (1<<ACIC);
}