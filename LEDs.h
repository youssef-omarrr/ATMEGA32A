/* 
 * File:   LEDs.h
 * Author: User
 *
 * Created on August 3, 2024, 8:19 PM
 */

#include <avr/io.h>

#ifndef LEDS_H
#define	LEDS_H

#define LED0    PC2
#define LED1    PC7
#define LED2    PD3

void initLED();
void LED_on (int led);
void LED_off (int led);
void LED_toggle (int led);


#endif	/* LEDS_H */

