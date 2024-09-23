#include "SevenSegment.h"
#include "DIO.h"
#include <avr/io.h>
#include <util/delay.h>


void init_SS(){
    setPINA_dir(SS_A, OUT);
    setPINA_dir(SS_B, OUT);
    setPINA_dir(SS_C, OUT);
    setPINA_dir(SS_D, OUT);
    
    setPINA_dir(SS_EN1, OUT);
    setPINA_dir(SS_EN2, OUT);
    setPINA_dir(SS_DIP, OUT);
}

void SS_init_EN1(){
    setPINB(SS_EN2, LOW);
    setPINB(SS_EN1, HIGH);
}
void SS_init_EN2(){
    setPINB(SS_EN1, LOW);
    setPINB(SS_EN2, HIGH);
}
void SS_init_BOTH(){
    setPINB(SS_EN1, HIGH);
    setPINB(SS_EN2, HIGH);
}

void SS_write(char number){
    //clear first before writing
    PORTA &= 0x0F; //0000 1111
    
    PORTA |= (number<<4);
}

void counter(){
    for (int j=0; j<9; j++){
        SS_init_EN1();
        SS_write(j);
        SS_init_EN2();
        for (int i=1; i<10; i++){
            SS_write(i);
            _delay_ms(1000);
        }
        SS_write(0);
    }
}
