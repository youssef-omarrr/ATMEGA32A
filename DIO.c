#include <avr/io.h>
#include "DIO.h"

/**
 * 
 * @param pinNUM Pin Number
 * @param status IN/OUT or HIGH/LOW
 */
void setPINA_dir(int pinNUM, int status) {
    switch (status) {
        case IN:
            DDRA &= ~(1 << pinNUM);
            break;
        case OUT:
            DDRA |= (1 << pinNUM);
            break;
        default:
            break;
    }
}

/**
 * 
 * @param pinNUM Pin Number
 * @param status IN/OUT or HIGH/LOW
 */
void setPINB_dir(int pinNUM, int status) {
    switch (status) {
        case IN:
            DDRB &= ~(1 << pinNUM);
            break;
        case OUT:
            DDRB |= (1 << pinNUM);
            break;
        default:
            break;
    }
}

/**
 * 
 * @param pinNUM Pin Number
 * @param status IN/OUT or HIGH/LOW
 */
void setPINC_dir(int pinNUM, int status) {
    switch (status) {
        case IN:
            DDRC &= ~(1 << pinNUM);
            break;
        case OUT:
            DDRC |= (1 << pinNUM);
            break;
        default:
            break;
    }
}

/**
 * 
 * @param pinNUM Pin Number
 * @param status IN/OUT or HIGH/LOW
 */
void setPIND_dir(int pinNUM, int status) {
    switch (status) {
        case IN:
            DDRD &= ~(1 << pinNUM);
            break;
        case OUT:
            DDRD |= (1 << pinNUM);
            break;
        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////

/**
 * 
 * @param pinNUM Pin Number
 * @param status HIGH/LOW/TOGGLE
 */
void setPINA(int pinNUM, int status) {
    switch (status) {
        case LOW:
            PORTA &= ~(1 << pinNUM);
            break;
        case HIGH:
            PORTA |= (1 << pinNUM);
            break;
        case TOGGLE:
            PORTA ^= (1 << pinNUM);
            break;
        default:
            break;
    }
}

/**
 * 
 * @param pinNUM Pin Number
 * @param status HIGH/LOW/TOGGLE
 */
void setPINB(int pinNUM, int status) {
    switch (status) {
        case LOW:
            PORTB &= ~(1 << pinNUM);
            break;
        case HIGH:
            PORTB |= (1 << pinNUM);
            break;
        case TOGGLE:
            PORTB ^= (1 << pinNUM);
            break;
        default:
            break;
    }

}

/**
 * 
 * @param pinNUM Pin Number
 * @param status HIGH/LOW/TOGGLE
 */
void setPINC(int pinNUM, int status) {
    switch (status) {
        case LOW:
            PORTC &= ~(1 << pinNUM);
            break;
        case HIGH:
            PORTC |= (1 << pinNUM);
            break;
        case TOGGLE:
            PORTC ^= (1 << pinNUM);
            break;
        default:
            break;
    }

}

/**
 * 
 * @param pinNUM Pin Number
 * @param status HIGH/LOW/TOGGLE
 */
void setPIND(int pinNUM, int status) {
    switch (status) {
        case LOW:
            PORTD &= ~(1 << pinNUM);
            break;
        case HIGH:
            PORTD |= (1 << pinNUM);
            break;
        case TOGGLE:
            PORTD ^= (1 << pinNUM);
            break;
        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////

/**
 * 
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTA_dir(int status) {
    switch (status) {
        case IN:
            DDRA = 0x00;
            break;
        case OUT:
            DDRA = 0xFF;
            break;
        default:
            break;
    }
}

/**
 * 
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTB_dir(int status) {
    switch (status) {
        case IN:
            DDRB = 0x00;
            break;
        case OUT:
            DDRB = 0xFF;
            break;
        default:
            break;
    }
}

/**
 * 
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTC_dir(int status) {
    switch (status) {
        case IN:
            DDRC = 0x00;
            break;
        case OUT:
            DDRC = 0xFF;
            break;
        default:
            break;
    }
}

/**
 * 
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTD_dir(int status) {
    switch (status) {
        case IN:
            DDRD = 0x00;
            break;
        case OUT:
            DDRD = 0xFF;
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
/**
 * 
 * @param val hexadecimal value of the whole port
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTA_dir_val(int val, int status) {
    switch (status) {
        case IN:
            DDRA &= ~val; //reset
            break;
        case OUT:
            DDRA |= val; //set
            break;
        default:
            break;
    }

}

/**
 * 
 * @param val hexadecimal value of the whole port
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTB_dir_val(int val, int status) {
    switch (status) {
        case IN:
            DDRB &= ~val; //reset
            break;
        case OUT:
            DDRB |= val; //set
            break;
        default:
            break;
    }

}

/**
 * 
 * @param val hexadecimal value of the whole port
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTC_dir_val(int val, int status) {
    switch (status) {
        case IN:
            DDRC &= ~val; //reset
            break;
        case OUT:
            DDRC |= val; //set
            break;
        default:
            break;
    }

}

/**
 * 
 * @param val hexadecimal value of the whole port
 * @param status IN/OUT or HIGH/LOW
 */
void setPORTD_dir_val(int val, int status) {
    switch (status) {
        case IN:
            DDRD &= ~val; //reset
            break;
        case OUT:
            DDRD |= val; //set
            break;
        default:
            break;
    }

}

///////////////////////////////////////////////////////////////////////////////
char readPINA(int pinNUM){
    return (PINA & (1<<pinNUM));
}
char readPINB(int pinNUM){
    return (PINB & (1<<pinNUM));
}
char readPINC(int pinNUM){
    return (PINC & (1<<pinNUM));
}
char readPIND(int pinNUM){
    return (PIND & (1<<pinNUM));
}

///////////////////////////////////////////////////////////////////////////////
char readPORTA(){
    return PINA;
}
char readPORTB(){
    return PINB;
}
char readPORTC(){
    return PINC;
}
char readPORTD(){
    return PIND;
}