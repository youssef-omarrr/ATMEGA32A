#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "DIO.h"
#include "LEDs.h"
#include "buttons.h"
#include "outdev.h"
#include "mLCD.h"
#include "mLCD4.h"
//or we can define messege [] = "hello"

int main(){
//    initBUTTON();
//    initLED();
//    initRELAY();
//    initBUZZER();
    initLCD4(); 
     //and write here LCD_DATA_STR(messege)
    while(1){
        LCD4_goto(1,3);
        LCD4_data_str ((char*)"HELLO");
        
        _delay_ms(500);
//        LCD4_CLEAR();
//        for (int i=0;; i++){
//            LCD4_data_num(i);
//            _delay_ms(500);
//            LCD4_CLEAR();
//        }
//        if (isPressedB(BUTTON0)){
//            LED_on(LED0);
//            _delay_ms(1000);
//            LED_off(LED0);
//            
//            LED_on(LED1);
//            _delay_ms(1000);
//            LED_off(LED1);
//            
//            LED_on(LED2);
//            _delay_ms(1000);
//            LED_off(LED2);
//        }
//        
//        if (isPressedD(BUTTON1)){
//            BUZEER_on();
//            _delay_ms(1000);
//            BUZEER_off();
//        }
//        
//        if (isPressedD(BUTTON2)){
//            RELAY_on();
//            _delay_ms(1000);
//            RELAY_off();
//        }
//            
//        else
//            LED_off(LED0);
//        
//        if (isPressedD(BUTTON1))
//            LED_on(LED1);
//        else
//            LED_off(LED1);
//        
//        if (isPressedD(BUTTON2))
//            LED_on(LED2);
//        else
//            LED_off(LED2);
      }
    return 0;
}