/* 
 * File:   keypad.h
 * Author: User
 *
 * Created on August 9, 2024, 8:31 PM
 */
#include <avr/io.h>

#ifndef KEYPAD_H
#define	KEYPAD_H

#define KEYPAD_PORT_DIR     DDRA
#define KEYPAD_PORT         PORTA

void init_KEYPAD();
char readkey();

#endif	/* KEYPAD_H */

