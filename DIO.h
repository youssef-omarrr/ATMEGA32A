/* 
 * File:   DIO.h
 * Author: Youssef Omar the first
 *
 * Created on August 3, 2024, 8:19 PM
 */
#include <avr/io.h>

#ifndef DIO_H
#define	DIO_H

#define HIGH    1
#define LOW     0
#define TOGGLE  2

#define IN      0
#define OUT     1

//sets a certain pin wither INPUT or OUTPUT
void setPINA_dir(int pinNUM, int status);
void setPINB_dir(int pinNUM, int status);
void setPINC_dir(int pinNUM, int status);
void setPIND_dir(int pinNUM, int status);

//sets a certain pin wither HIGH or LOW
void setPINA(int pinNUM, int status);
void setPINB(int pinNUM, int status);
void setPINC(int pinNUM, int status);
void setPIND(int pinNUM, int status);

//sets all port wither INPUT or OUTPUT
void setPORTA_dir(int status);
void setPORTB_dir(int status);
void setPORTC_dir(int status);
void setPORTD_dir(int status);

//sets multiple pins in a port to INPUT or OUTPUT
//takes a hexadecimal value for all 8 bits for the port
void setPORTA_dir_val(int val, int status);
void setPORTB_dir_val(int val, int status);
void setPORTC_dir_val(int val, int status);
void setPORTD_dir_val(int val, int status);

//reads a certain pin and returns it (HIGH or LOW)
char readPINA(int pinNUM);
char readPINB(int pinNUM);
char readPINC(int pinNUM);
char readPIND(int pinNUM);

//reads all port and returns it
char readPORTA();
char readPORTB();
char readPORTC();
char readPORTD();

#endif	/* DIO_H */

