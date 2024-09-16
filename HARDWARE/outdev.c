#include <avr/io.h>
#include "DIO.h"
#include "outdev.h"

void initBUZZER(){
    setPINB_dir(BUZZER, OUT);
}
void initRELAY(){
    setPINA_dir(RELAY, OUT);
}

void BUZZER_on(){
    setPINB(BUZZER, HIGH);
}
void BUZZER_off(){
    setPINB(BUZZER, LOW);
}
void RELAY_on(){
    setPINA(RELAY, HIGH);
}
void RELAY_off(){
    setPINA(RELAY, LOW);
}
