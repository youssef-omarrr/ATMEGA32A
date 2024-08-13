#include <avr/io.h>
#include "DIO.h"
#include "outdev.h"

void initBUZZER(){
    setPINA_dir(BUZZER, OUT);
}
void initRELAY(){
    setPINA_dir(RELAY, OUT);
}

void BUZEER_on(){
    setPINA(BUZZER, HIGH);
}
void BUZEER_off(){
    setPINA(BUZZER, LOW);
}
void RELAY_on(){
    setPINA(RELAY, HIGH);
}
void RELAY_off(){
    setPINA(RELAY, LOW);
}