/* 
 * File:   LCD.h
 * Author: User
 *
 * Created on August 10, 2024, 5:30 PM
 */
#include<avr/io.h>

#ifndef LCD_H
#define	LCD_H

//for 8 data pins
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
#define MODE_4BITS               0x28
#define CLEAR                    0x01
#define CURSOR_OFF_DISPLAY_ON    0x0C
#define ENTRY_MODE               0x06

void initLCD();

void LCD_cmd(char); //LCD does a command

void LCD_data(char); //data
void LCD_data_str (char*); //data as string not char only
void LCD_data_num(int); //data as a number

void LCD_en();
void LCD_CLEAR();


#endif	/* LCD_H */

