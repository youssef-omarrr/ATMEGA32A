/* 
 * File:   intr.h
 * Author: User
 *
 * Created on August 17, 2024, 5:10 PM
 */
#include <avr/io.h>

#ifndef INTR_H
#define	INTR_H

#define rising_edge       0
#define falling_edge      1
#define toggle            2
#define low_level         3

void INT0_selectmode(char );
void initINT0(char );
void disableINT0();
void INT0_changemode(char );

void INT1_selectmode(char );
void initINT1(char );
void disableINT1();
void INT1_changemode(char );

void INT2_selectmode(char );
void initINT2(char );
void disableINT2();
void INT2_changemode(char );

#endif	/* INTR_H */

