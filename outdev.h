/* 
 * File:   outdev.h
 * Author: User
 *
 * Created on August 9, 2024, 7:59 PM
 */
#include <avr/io.h>

#ifndef OUTDEV_H
#define	OUTDEV_H

#define BUZZER  PB0
#define RELAY   PA2

void initBUZZER();
void initRELAY();

void BUZZER_on();
void BUZZER_off();
void RELAY_on();
void RELAY_off();



#endif	/* OUTDEV_H */

