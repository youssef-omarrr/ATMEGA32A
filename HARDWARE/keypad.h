/* 
 * File:   keypad.h
 * Author: User
 *
 * Created on August 9, 2024, 8:31 PM
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#ifndef KEYPAD_H
#define	KEYPAD_H


#define KEYPAD_PORT_DIR     DDRA
#define KEYPAD_PORT         PORTA
#define READ_KEY            readPORTA()

void init_KEYPAD();
char readkey();

#endif	/* KEYPAD_H */

