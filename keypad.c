#include <avr/io.h>
#include "DIO.h"
#include "keypad.h"

void initKEYPAD(){
    KEYPAD_PORT_DIR = 0x07;
    KEYPAD_PORT |= 0x07;
}

char readkey(){
    
    
    return '.';
}