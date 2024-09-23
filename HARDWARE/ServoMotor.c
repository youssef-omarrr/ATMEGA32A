#include <avr/io.h>
#include "mHEADERS.h"

#define OCR2_MIN 50  // Corresponds to 1 ms pulse (0°)
#define OCR2_MAX 250  // Corresponds to 2 ms pulse (180°)
#define OCR2_STEP 100   // Adjust step for smoother control

//servo motor works by just adjusting a pwm to its pin
//we will do this by a timer

int main() {
    timer2_OC2_init();
    initTimer2(TIMER_CTC, CLK2_128);
    
    timer2_OC2_selectMode(OC2_CTC_TOGGLE);
    initBUTTON();
    
    OCR2 = 70;  // Start at 0° position (1 ms pulse)
    LCD4_data_num(OCR2);
    
    while (1) {
        
        for (int i=70; i<250; i++){
            OCR2 = i;
            _delay_us(1000);
        }
        _delay_ms(1000);
        for (int i=250; i>70; i--){
            OCR2 = i;
            _delay_us(1000);
        }
        _delay_ms(1000);
        
//        if (isPressed(BUTTON0)) {
//            if (OCR2 + OCR2_STEP <= OCR2_MAX) {
//                OCR2 += OCR2_STEP;   
//                LCD4_CLEAR();
//                LCD4_data_num(OCR2);
//            }
//        }
//
//        if (isPressed(BUTTON1)) {
//            if (OCR2 - OCR2_STEP >= OCR2_MIN) {
//                OCR2 -= OCR2_STEP;   
//                LCD4_CLEAR();
//                LCD4_data_num(OCR2);
//            }
//        }
    }

    return 0;
}
