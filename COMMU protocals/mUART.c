#include <avr/io.h>
#include "mUART.h"
#include "DIO.h"
#include <string.h>
#include <util/delay.h>
/**
 * 
 * @param baud              UART_BAUD_9600
 * 
 * @param data_mode         RECEIVER      
                            TRANSMITTER  
                            BOTH
 * 
 * @param clk_mode          ASYNCH
                            ASYNCH_X2
                            SYNCH_MASTER
 *                          SYNCH_SLAVE
 *                                    
 */
void initUART (int baud,  int data_mode, int clk_mode){
    
    //at anytime we want to access UCSRC we have to put '1' in URSEL
    //because its address is multiplexed with UBRRH
    //URSEL = 1 -> UCSRC 
    //URSEL = 0 -> UBRRH 
    
    unsigned short ubrr = 0;
    
    switch (clk_mode){
        //UMSEL = 1 -> synch
        //UMSEL = 0 -> Asynch
        case(ASYNCH):
            ubrr = (F_CPU/16.0/baud)-1;
            //select mode (and register at the same clock cycle because the address is multiplexed)
            UCSRC = SELECT_UCSRC & (~(1<<UMSEL));
            UCSRA &= ~(1<<U2X);//must reset x2 bit
            break;
        case(ASYNCH_X2):
            ubrr = (F_CPU/8.0/baud)-1;
            //select mode
            UCSRC  = SELECT_UCSRC & (~(1<<UMSEL));
            UCSRA |= 1<<U2X;  //set X2 mode
            break;
        case(SYNCH_MASTER):
            ubrr = (F_CPU/2.0/baud)-1;
            //select mode
            UCSRC  = SELECT_UCSRC | (1<<UMSEL);
            //set clock direction
            setPINB_dir(XCK, OUT);
            break;
        case(SYNCH_SLAVE):
            ubrr = (F_CPU/2.0/baud)-1;
            //select mode
            UCSRC  = SELECT_UCSRC | (1<<UMSEL);
            //set clock direction
            setPINB_dir(XCK, IN);
            break;
           
    }
    
    //set baud rate
    UBRRH = (unsigned int) ubrr>>8;
    UBRRL = (unsigned int) ubrr;
    
    //8 bit data and 2 stop bits mode (USBS for stop bits)(UCSZ0 for 8bits mode)
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

    //enable receiver and transmitter
    switch (data_mode){
        //reset them first 
        UCSRB &= ~(1<<RXEN)|(1<<TXEN);
       
        case(RECEIVER):
            UCSRB |= (1<<RXEN);
            break;
        case(TRANSMITTER):
            UCSRB |= (1<<TXEN);
            break;
        case(BOTH):
            UCSRB |= (1<<RXEN)|(1<<TXEN);
            break; 
    }
    
    
}

void UART_send (char data){
    while (!(UCSRA & (1<<UDRE))); /*wait until USART Data Register is Empty to send data
                                  we used while inside the function instead of 'if' to prevent data losses*/
    //Put data into buffer, sends the data
    UDR = data;                   
}

void UART_sen_str (char* data){
    for (int i=0; i < strlen(data); i++){
        while (!(UCSRA & (1<<UDRE))); /*wait until USART Data Register is Empty to send data
                                      we used while inside the function instead of 'if' to prevent data losses*/
        //Put data into buffer, sends the data
        UDR = data[i];  
        _delay_us(100);
    }
}

char UART_receive(){
    //reset RX interrupt first
    UCSRB &= ~(1<<RXCIE);
    
    //RXC flag bit is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
    //wait until all data has been received (RXC=0 until data is read (from TX) then it's 1)
    while (!(UCSRA & (1<<RXC))); 
    
    // Error checking for frame error, data overrun, and parity error
    if (UCSRA & ( (1<<FE) | (1<<DOR) | (1<<UPE)) ) {
        return -1;
    }
    
    //once RXC = 1 (means new data available in the UART receive buffer.) return data in UDR
    return UDR;           
    //or instead of pooling we can receive this data in the interrupt of RXC
}

void UART_receive_no_pooling(){
    
      // ISR to handle received data
    
//    ISR(USART_RXC_vect) {
//        char received_data;
    
//        // Read the received data from UDR (clears RXC flag)
//        received_data = UDR;
//    }
    
}

void UART_flush(){
    /*The receiver buffer FIFO will be flushed when the Receiver is disabled, that is, 
     * the buffer will be emptied of its contents.
     * Unread data will be lost
     */
    
    if (UCSRB & (1<<RXEN)) {  // Check if the receiver is enabled
        unsigned char dummy;
        //every unread data is stored in dummy
        while (UCSRA & (1<<RXC)) dummy = UDR;
    }
}

//interrupts functions
void UART_TXINT_EN(){
    UCSRB |= (1<<TXCIE); //TX complete interrupt enable
}
void UART_RXINT_EN(){
    UCSRB |= (1<<RXCIE); //RX complete interrupt enable
}
void UART_DATAINT_EN(){
    UCSRB |= (1<<UDRIE); //UART Data Register complete interrupt enable
}