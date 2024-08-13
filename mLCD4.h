/* 
 * File:   mLCD4.h
 * Author: User
 *
 * Created on August 10, 2024, 8:28 PM
 */
#include<avr/io.h>

#ifndef MLCD4_H
#define	MLCD4_H //ONLY DIFFERNECE IS THAT THIS WORKS WITH 4 DATA PINS ONLY!!!!

//for 4 data pins
#define LCD_DATA_DIR        DDRC
#define LCD_DATA            PORTC

//for 3 control pins
#define LCD_CONTROL_DIR     DDRD
#define LCD_CONTROL         PORTD
#define RS                  PD0
#define RW                  PD1
#define EN                  PD2

//functions predefined in data sheet
#define HOME                     0x02
#define MODE_8BITS               0x38   
#define MODE_4BITS               0x28 //we will work with this one this time
#define CLEAR                    0x01
#define CURSOR_OFF_DISPLAY_ON    0x0C
#define ENTRY_MODE               0x06
#define MOVE_RIGHT               0x14 //moves right by one from where the cursor is
#define MOVE_LEFT                0x10//
#define LINE1                    0x80 //by changing the zero it will move right in this step count
#define LINE2                    0xC0 //the 7th bit changes to the next line


void initLCD4();

void LCD4_cmd(char); //LCD does a command

void LCD4_data(char); //data
void LCD4_data_str (char*); //data as string not char only
void LCD4_data_num(int); //data as a number

void LCD4_move_right(int);
void LCD4_move_left(int);
void LCD4_goto(int, int);

void LCD4_en();
void LCD4_CLEAR();

#endif	/* MLCD4_H */

