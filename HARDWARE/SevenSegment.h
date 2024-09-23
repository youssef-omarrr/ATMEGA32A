/* 
 * File:   SevenSegment.h
 * Author: User
 *
 * Created on September 21, 2024, 6:43 PM
 */

#ifndef SEVENSEGMENT_H
#define	SEVENSEGMENT_H

#include <avr/io.h>

#define SS_A    PA4
#define SS_B    PA5
#define SS_C    PA6
#define SS_D    PA7

#define SS_EN1  PB1
#define SS_EN2  PB2
#define SS_DIP  PB3

void init_SS();
void SS_init_EN1();
void SS_init_EN2();
void SS_init_BOTH();
void SS_write(char number);
#endif	/* SEVENSEGMENT_H */

