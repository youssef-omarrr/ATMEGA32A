#include <avr/io.h>
#include <avr/interrupt.h>
#include "intr.h"

////////////////////////////////////////////////////////////////////////////////
void INT0_selectmode(char Mode){
    MCUCR &= ~(1<<ISC00); //reset bits first
    MCUCR &= ~(1<<ISC01);
    
    switch (Mode){
        case(rising_edge): //11
            
            MCUCR |= (1<<ISC00); //11
            MCUCR |= (1<<ISC01);
            break;
            
        case(falling_edge): //10
            
            MCUCR |= (1<<ISC01); //10
            break;
            
        case(toggle): //01 any logic change
            MCUCR |= (1<<ISC00); //01
            break;
            
        case(low_level): //00 low level
            //already 00
            break;
    }
}

void initINT0(char Mode){
    //select mode
    INT0_selectmode(Mode);
    
    //enable interrupt 0
    GICR |= (1<<INT0);
}

void disableINT0(){
    //disables interrupt 0
    GICR &= ~(1<<INT0);
}

void INT0_changemode(char Mode){
    //disables the interrupt before changing the select mode
    disableINT0();
    
    //select mode
    INT0_selectmode(Mode);
    
    //re-enable interrupt 0
    GICR |= (1<<INT0);
}

////////////////////////////////////////////////////////////////////////////////
void INT1_selectmode(char Mode){
    MCUCR &= ~(1<<ISC10); //reset bits first
    MCUCR &= ~(1<<ISC11);
    
    switch (Mode){
        case(rising_edge): //11
            
            MCUCR |= (1<<ISC10); //11
            MCUCR |= (1<<ISC11);
            break;
            
        case(falling_edge): //10
            
            MCUCR |= (1<<ISC11); //10
            break;
            
        case(toggle): //01 any logic change
            MCUCR |= (1<<ISC10); //01
            break;
            
        case(low_level): //00 low level
            //already 00
            break;
    }
}

void initINT1(char Mode){
    //select mode
    INT1_selectmode(Mode);
    
    //enable interrupt 0
    GICR |= (1<<INT1);
}

void disableINT1(){
    //disables interrupt 0
    GICR &= ~(1<<INT1);
}

void INT1_changemode(char Mode){
    //disables the interrupt before changing the select mode
    disableINT1();
    
    //select mode
    INT1_selectmode(Mode);
    
    //re-enable interrupt 0
    GICR |= (1<<INT1);
}
///////////////////////////////////////////////////////////////////////////////

void INT2_selectmode(char Mode){
    MCUCSR &= ~(1<<ISC2); //reset bit first
    
    switch (Mode){
        case(rising_edge): //1
            
            MCUCSR |= (1<<ISC2); //1
            break;
            
        case(falling_edge): //0
            break;
            
    }
}

void initINT2(char Mode){
    //select mode
    INT1_selectmode(Mode);
    
    //enable interrupt 0
    GICR |= (1<<INT2);
}

void disableINT2(){
    //disables interrupt 0
    GICR &= ~(1<<INT2);
}

void INT2_changemode(char Mode){
    //disables the interrupt before changing the select mode
    disableINT2();
    
    //select mode
    INT2_selectmode(Mode);
    
    //re-enable interrupt 0
    GICR |= (1<<INT2);
}

///////////////////////////////////////////////////////////////////////////////