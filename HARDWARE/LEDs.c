#include <avr/io.h>
#include "LEDs.h"
#include "DIO.h"

void initLED(){
    setPINC_dir(LED0, OUT);
    setPINC_dir(LED1, OUT);
    setPIND_dir(LED2, OUT);
}

void LED_on (int led){
    switch (led){
        case LED0:
            setPINC(LED0, HIGH);
            break;
        case LED1:
            setPINC(LED1, HIGH);
            break;
        case LED2:
            setPIND(LED2, HIGH);
            break;
        default:
            break; 
    }
}

void LED_off (int led){
    switch (led){
        case LED0:
            setPINC(LED0, LOW);
            break;
        case LED1:
            setPINC(LED1, LOW);
            break;
        case LED2:
            setPIND(LED2, LOW);
            break;
        default:
            break; 
    }
}

void LED_toggle (int led){
    switch (led){
        case LED0:
            setPINC(LED0, TOGGLE);
            break;
        case LED1:
            setPINC(LED1, TOGGLE);
            break;
        case LED2:
            setPIND(LED2, TOGGLE);
            break;
        default:
            break; 
    }
    
}
