/* 
 * File:   mtimer.h
 * Author: User
 *
 * Created on August 23, 2024, 8:56 PM
 */

#ifndef MTIMER_H
#define	MTIMER_H
#include <avr/io.h>
//Waveform Generation Mode
#define TIMER_NORMAL      0 //clear timer when it reaches max 255
#define TIMER_PWM         1 //0 to 255 to 0
#define TIMER_CTC         2 //clear timer when it reaches the compare value in OCR0
#define TIMER_FAST_PWM    3 //0 to 255

//Clock Select Bit Description
#define CLK_NO           0
#define CLK              1
#define CLK_8            2
#define CLK_64           3
#define CLK_256          4
#define CLK_1024         5
#define CLK_EX_FALLING   6
#define CLK_EX_RISING    7

#define CTC_TOP          100
#define OC0              PB3 //is output and can be any value (triggered when a compare match occurs)

//type of output in OC0 after compare or overflow (according to wave and compare types)
//o is disconnected for all
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

//timer 0
void initTimer0(char wave, char clk);
void timer0_selectMode(char wave);
void timer0_select_clock(char clk);
void timer0_TOV_int_en(); //to enable Timer0 Overflow Interrupt
void timer0_COMP_int_en(); //to enable Timer0 Output Compare Match Interrupt
void timer0_OC0_init();
void timer0_OC0_selectMode(char mode);
void timer0_prescaler_reset();

//timer 1


//timer2
#define OC2         PD7
#define clk_io        0
#define clk_osc       1    
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

