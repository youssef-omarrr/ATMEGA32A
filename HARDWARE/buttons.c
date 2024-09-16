#include <avr/io.h>
#include "buttons.h"
#include "DIO.h"

void initBUTTON(){
    setPINB_dir(BUTTON0, IN);
    setPIND_dir(BUTTON1, IN);
    setPIND_dir(BUTTON2, IN);
}

char isPressedA(char pinNUM){
    return readPINA(pinNUM);
}
char isPressedB(char pinNUM){
    return readPINB(pinNUM);
}
char isPressedC(char pinNUM){
    return readPINC(pinNUM);
}
char isPressedD(char pinNUM){
    return readPIND(pinNUM);
}

int isPressed (int pinNUM){
    switch (pinNUM){
        case(BUTTON0):
            if (readPINB(pinNUM))
                return 1;
            else
                return 0;
            
        case(BUTTON1):
            if (readPIND(pinNUM))
                return 1;
            else
                return 0;
            
        case(BUTTON2):
            if (readPIND(pinNUM))
                return 1;
            else
                return 0;
    }
    return 0;
}
