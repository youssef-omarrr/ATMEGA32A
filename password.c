#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "DIO.h"
#include "outdev.h"
#include "mLCD4.h"
#include "keypad.h"


int main(){
    initKEYPAD();
    _delay_ms(5);
    initLCD4(); 
    _delay_ms(5);
    initBUZZER();
    _delay_ms(5);
         
    int flag =0;
    char pass [16];
    int count =0;
    
    do{
        char temp1 [16];
        int temp2 =0;
    
        LCD4_data_str("Enter new pswrd:");
        LCD4_goto(1,0);
        
        for (int i=0; i<16; i++){
            char x = readkey();
            if (x == '='){
                LCD4_CLEAR();
                LCD4_data_str("Password Saved");
                _delay_ms(500);
                LCD4_CLEAR();
                flag = 1;
                strcpy(pass, temp1);
                count = temp2;
                break;
            }
            else if (x == 'c'){
                LCD4_CLEAR();
                break;

            }
            else{
                LCD4_data(x);
                temp1[i] = x;
                _delay_ms(350);
                temp2++;
            }
        }
    }while (flag == 0);
    
    int y = 0;
    
    while(1){
        while (y<3){
            LCD4_CLEAR();
            LCD4_data_str("Enter pswrd:");
            LCD4_goto(1,0);
            char pass2 [count];
            
            for (int i=0; i<16; i++){
                char z = readkey();
                _delay_ms(250);
                if (z == '='){
                    if (strncmp(pass2, pass, count)){
                        LCD4_CLEAR();
                        if (y<2){
                        LCD4_data_str("Try again!");
                        _delay_ms(1000);
                        LCD4_CLEAR();
                        y++;
                        break;
                        }
                        else{
                            LCD4_data_str("out of tries!!");
                            _delay_ms(250);
                            BUZZER_on();
                            y = 3;
                            break;
                        }
                 
                    }
                    else{
                        LCD4_CLEAR();
                        LCD4_data_str("Right password!");
                        _delay_ms(1000);
                        y = 5;
                        break;
                    }
                }

                else if (z == 'c'){
                    LCD4_CLEAR();
                    break;
                } 

                else{
                    LCD4_data(z);
                    pass2[i] = z;
                    _delay_ms(100);
                } 
            }
        
        }

      }
    return 0;
}