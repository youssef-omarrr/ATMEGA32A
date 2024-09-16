#include <avr/io.h>
#include "DIO.h"
#include "keypad.h"

void initKEYPAD(){
    KEYPAD_PORT_DIR = 0x0F;
    KEYPAD_PORT |= 0x0F;
}

char readkey(){
    
    KEYPAD_PORT |= 0x0F;
    while(1){
        
    if (READ_KEY & 0xF0){ //this means any button is pressed
        KEYPAD_PORT &= 0xF0;   //we will try each row until we get it right
        KEYPAD_PORT |= (1<<3); //so first we need to make the outputs zero and make only one of them one
        _delay_ms(1);
        if (READ_KEY & 0x80){
            _delay_us(500);
            return '7';
        }
        else if (READ_KEY & 0x40){
            _delay_us(500);
            return '4';
        }
        else if (READ_KEY & 0x20){
            _delay_us(500);
            return '1';
        }
        else if (READ_KEY & 0x10){
            _delay_us(500);
            return 'c';
        }
        ////////////////////////////////////////////////////////////////////////
        else{
            KEYPAD_PORT &= 0xF0; //move to he next row
            KEYPAD_PORT |= (1<<2);
            _delay_ms(1);
        if (READ_KEY & 0x80){
            _delay_us(500);
            return '8';
        }
        else if (READ_KEY & 0x40){
            _delay_us(500);
            return '5';
        }
        else if (READ_KEY & 0x20){
            _delay_us(500);
            return '2';
        }
        else if (READ_KEY & 0x10){
            _delay_us(500);
            return '0';
        }
        ////////////////////////////////////////////////////////////////////////
            
        else{
            KEYPAD_PORT &= 0xF0; //move to he next row
            KEYPAD_PORT |= (1<<1);
            _delay_ms(1);
        if (READ_KEY & 0x80){
            _delay_us(500);
            return '9';
        }
        else if (READ_KEY & 0x40){
            _delay_us(500);
            return '6';
        }
        else if (READ_KEY & 0x20){
            _delay_us(500);
            return '3';
        }
        else if (READ_KEY & 0x10){
            _delay_us(500);
            return '=';
        }
        ////////////////////////////////////////////////////////////////////////
        else{
            KEYPAD_PORT &= 0xF0; //move to he next row
            KEYPAD_PORT |= (1<<0);
            _delay_ms(1);
        if (READ_KEY & 0x80){
            _delay_us(500);
            return '/';
        }
        else if (READ_KEY & 0x40){
            _delay_us(500);
            return 'x';
        }
        else if (READ_KEY & 0x20){
            _delay_us(500);
            return '-';
        }
        else if (READ_KEY & 0x10){
            _delay_us(500);
            return '+';
        }//I
        }//LOST
        }//COUNT
        }//OF
        }//THE
    KEYPAD_PORT |= 0x0F;
    _delay_ms(1);
        }//CURLY BRACES 
    return '.';
}
