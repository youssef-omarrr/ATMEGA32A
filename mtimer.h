/* 
 * File:   mtimer.h
 * Author: User
 *
 * Created on August 23, 2024, 8:56 PM
 */

#ifndef MTIMER_H
#define	MTIMER_H
#include <avr/io.h>

//timer 0

//Waveform Generation Mode
#define TIMER_NORMAL      0 //clear timer when it reaches max 255
#define TIMER_PWM         1 //0 to 255 to 0
#define TIMER_CTC         2 //clear timer when it reaches the compare value in OCR0
#define TIMER_FAST_PWM    3 //0 to 255

//Clock Select Bit Description
#define CLK01_NO           0
#define CLK01              1
#define CLK01_8            2
#define CLK01_64           3
#define CLK01_256          4
#define CLK01_1024         5
#define CLK01_EX_FALLING   6
#define CLK01_EX_RISING    7

#define CTC_TOP          100
#define OC0              PB3 //is output and can be any value (triggered when a compare match occurs)

//type of output in OC0 after compare or overflow (according to wave and compare types)
//0 is disconnected for all
#define OC0_DISCONNECTED          0
//CTC
#define OC0_CTC_TOGGLE            1 //Toggle OC0 on compare match
#define OC0_CTC_CLEAR             2 //Clear OC0 on compare match
#define OC0_CTC_SET               3 //Set OC0 on compare match

//1 is reserved for both
//PWM
#define OC0_PWM_CLEAR_UP    2 //Clear OC0 on compare match when up-counting. Set OC0 on compare match when downcounting
#define OC0_PWM_SET_UP      3 //Set OC0 on compare match when up-counting. Clear OC0 on compare match when downcounting.
//fast PWM
#define OC0_FPWM_CLEAR      2 //Clear OC0 on compare match, set OC0 at BOTTOM
#define OC0_FPWM_SET        3 //Set OC0 on compare match, clear OC0 at BOTTOM

void initTimer0(char wave, char clk);
void timer0_selectMode(char wave);
void timer0_select_clock(char clk);
void timer0_TOV_int_en(); //to enable Timer0 Overflow Interrupt
void timer0_COMP_int_en(); //to enable Timer0 Output Compare Match Interrupt
void timer0_OC0_init();
void timer0_OC0_selectMode(char mode);
void timer0_prescaler_reset();

//timer 1

//Waveform Generation Mode
//to see when is the OCR1A and TOV1 flags are updated see data sheet page 114
#define TIMER_NORMAL                0 //0 to FFFF

#define TIMER_PWM_8BIT              1 //0 to 00FF to 0
#define TIMER_PWM_9BIT              2 //0 to 01FF to 0
#define TIMER_PWM_10BIT             3 //0 to 03FF to 0

#define TIMER_CTC_OCR1A             4 //0 TO OCR1A

#define TIMER_FAST_PWM_8BIT         5 //0 to 00FF
#define TIMER_FAST_PWM_9BIT         6 //0 to 01FF
#define TIMER_FAST_PWM_10BIT        7 //0 to 03FF

#define TIMER_PWM_FREQ_ICR1         8 //0 TO ICR1
#define TIMER_PWM_FREQ_OCR1A        9 //0 TO OCR1A

#define TIMER_PWM_ICR1              10 //0 TO ICR1
#define TIMER_PWM_OCR1              11 //0 TO OCR1A

#define TIMER_CTC_ICR1              12 //0 TO ICR1

#define TIMER_FAST_PWM_ICR1         13 //0 TO ICR1
#define TIMER_FAST_PWM_OCR1         14 //0 TO OCR1A

//type of output in OC1A/B after compare or overflow (according to wave and compare types)
//0 is disconnected for all
//A
#define OC1A_DISCONNECTED          0
//B
#define OC1B_DISCONNECTED          0

/////////////CTC/////////////
//A
#define OC1A_CTC_TOGGLE            1 //Toggle OC1A/B on compare match
#define OC1A_CTC_CLEAR             2 //Clear OC1A/B on compare match
#define OC1A_CTC_SET               3 //Set OC1A/B on compare match
//B
#define OC1B_CTC_TOGGLE            1 
#define OC1B_CTC_CLEAR             2 
#define OC1B_CTC_SET               3 

//1 is reserved for both
/////////////PWM/////////////
//A
#define OC1A_PWM_CLEAR_UP    2 //Clear OC1A/B on compare match when up-counting. Set OC1A/B on compare match when downcounting
#define OC1A_PWM_SET_UP      3 //Set OC1A/B on compare match when up-counting. Clear OC1A/B on compare match when downcounting.
//B
#define OC1B_PWM_CLEAR_UP    2 
#define OC1B_PWM_SET_UP      3 
/////////////FAST PWM/////////////
//A
#define OC1A_FPWM_CLEAR      2 //Clear OC1A/B on compare match, set OC1A/B at BOTTOM
#define OC1A_FPWM_SET        3 //Set OC1A/B on compare match, clear OC1A/B at BOTTOM
//B
#define OC1B_FPWM_CLEAR      2 
#define OC1B_FPWM_SET        3 

#define OC1A        PD5
#define OC1B        PD4
#define ICP1        PD6

void initTimer1(char wave, char clk);
void timer1_selectMode(char wave);
void timer1_select_clock(char clk);

void timer1_TOV_int_en(); //to enable Timer1 Overflow Interrupt

void timer1A_COMP_int_en(); //to enable Timer1 A Output Compare Match Interrupt
void timer1_OC1A_init();
void timer1_OC1A_selectMode(char mode);

void timer1B_COMP_int_en(); //to enable Timer1 B Output Compare Match Interrupt
void timer1_OC1B_init();
void timer1_OC1B_selectMode(char mode);

void timer1_ICP_init();
void timer1_ICP_int_en(); //to enable input capture interrupt
void timer1_ICP_noise_filter(); //to enable nosie canceler
#define FALLING_EDGE    0
#define RISING_EDGE     1
void timer1_ICP_selectEdge(char edge); //to select edge select mode
short unsigned int timer1_ICP_getValue();

#define AIN0    PB2
#define AIN1    PB3
void timer1_ACO_init();


//timer2


//type of output in OC2 after compare or overflow (according to wave and compare types)
//0 is disconnected for all
#define OC2_DISCONNECTED          0
//CTC
#define OC2_CTC_TOGGLE            1 //Toggle OC2 on compare match
#define OC2_CTC_CLEAR             2 //Clear OC2 on compare match
#define OC2_CTC_SET               3 //Set OC2 on compare match

//1 is reserved for both
//PWM
#define OC2_PWM_CLEAR_UP    2 //Clear OC2 on compare match when up-counting. Set OC2 on compare match when downcounting
#define OC2_PWM_SET_UP      3 //Set OC2 on compare match when up-counting. Clear OC2 on compare match when downcounting.
//fast PWM
#define OC2_FPWM_CLEAR      2 //Clear OC2 on compare match, set OC2 at BOTTOM
#define OC2_FPWM_SET        3 //Set OC2 on compare match, clear OC2 at BOTTOM

#define OC2         PD7
#define clk_io        0
#define clk_osc       1    

//Clock Select Bit Description
#define CLK2_NO           0
#define CLK2              1
#define CLK2_8            2
#define CLK2_32           3
#define CLK2_64           4
#define CLK2_128          5
#define CLK2_256          6
#define CLK2_1024         7
         

void initTimer2(char wave, char clk);
void timer2_selectMode(char wave);
void timer2_select_clock(char clk);
void timer2_TOV_int_en(); //to enable Timer2 Overflow Interrupt
void timer2_COMP_int_en(); //to enable Timer2 Output Compare Match Interrupt
void timer2_OC2_init();
void timer2_OC2_selectMode(char mode);
void timer2_asynch(char mode);
void timer2_prescaler_reset();

#endif	/* MTIMER_H */

