/* 
 * File:   mtimer.h
 * Author: User
 *
 * Created on August 23, 2024, 8:56 PM
 */

#ifndef MTIMER_H
#define	MTIMER_H

//Waveform Generation Mode
#define TIMER_NORMAL      0
#define TIMER_PWM         1
#define TIMER_CTC         2
#define TIMER_FAST_PWM    3

//Clock Select Bit Description
#define CLK_NO           0
#define CLK              1
#define CLK_8            2
#define CLK_64           3
#define CLK_256          4
#define CLK_1024         5
#define CLK_EX_FALLING   6
#define CLK_EX_RISING    7


void initTimer0(char wave, char clk);
void timer0_selectMode (char wave);
void timer0_select_clock (char clk);
void timer0_TOV_int_en(); //to enable Timer0 Overflow Interrupt
void timer0_COMP_int_en(); //to enable Timer0 Output Compare Match Interrupt


#endif	/* MTIMER_H */

