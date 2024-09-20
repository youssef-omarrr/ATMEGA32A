///////////////// UART MASTER ////////////////////////////////////////

#include <avr/io.h>
#include "mHEADERS.h"


int main() {
    
    initBUTTON();
    initUART(9600, TRANSMITTER, SYNCH_MASTER);
            
    initLCD4();
    LCD4_data_str("MASTER");
    
    while (1) {
        if (isPressed(BUTTON2))
            {
            _delay_ms(100);
            char* data = "Hello World";
            LCD4_goto(1,0);
            for (int i=0; data[i] != '\0'; i++){
                UART_send(data[i]);
                _delay_us(100);
                LCD4_data_num(i);   
            }
        }
        
        if (isPressed(BUTTON1)){
            _delay_ms(100);
            UART_sen_str("Hello World");
            _delay_us(100);
            
        }
       
    }

    return 0;
}

///////////////// UART SLAVE ////////////////////////////////////////
#include <avr/io.h>
#include "mHEADERS.h"

ISR(USART_RXC_vect) {
        char received_data;
    
        // Read the received data from UDR (clears RXC flag)
        received_data = UDR;
        LCD4_data(received_data);
    }

int main() {
    
    initLCD4();
    initBUTTON();
    initUART(9600, RECEIVER, SYNCH_SLAVE);
    LCD4_data_str("SLAVE");
    sei();
    
    while (1) {
        if (isPressed(BUTTON2)){
            LCD4_CLEAR();
            for (int i=0; i<16; i++){
                char x = UART_receive();
                LCD4_data(x);   
//            UART_flush();
            }
        }
        
        if (isPressed(BUTTON1)){
            LCD4_CLEAR();
            UART_RXINT_EN();
        }  
    }
    return 0;
}
