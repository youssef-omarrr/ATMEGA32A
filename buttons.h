/* 
 * File:   buttons.h
 * Author: User
 *
 * Created on August 7, 2024, 3:25 PM
 */
#include <avr/io.h>

#ifndef BUTTONS_H
#define	BUTTONS_H

#define BUTTON0     PB0
#define BUTTON1     PD6
#define BUTTON2     PD2

void initBUTTON();

char isPressedA(char pinNUM);
char isPressedB(char pinNUM);
char isPressedC(char pinNUM);
char isPressedD(char pinNUM);

int isPressed (int pinNUM);

#endif	/* BUTTONS_H */

