/* 
 * File:   mUART.h
 * Author: User
 *
 * Created on September 6, 2024, 6:09 PM
 */

#ifndef MUART_H
#define	MUART_H
#include <avr/io.h>

#define F_CPU             16000000UL
#define UART_BAUD_9600    9600

#define RECEIVER      0
#define TRANSMITTER   1
#define BOTH          2

#define SYNCH_MASTER        0
#define SYNCH_SLAVE         1
#define ASYNCH              2
#define ASYNCH_X2           3

#define XCK         PB0 //clock generation pin only active in synch mode

#define SELECT_UCSRC   (UCSRC | (1<<URSEL))//beacuse its address is multiplexed with UBRRH

/*
 REGISTERS:
 * UCSRA            ? USART Control and Status Register A
 * UCSRB            ? USART Control and Status Register B
 * UCSRC            ? USART Control and Status Register C
 * UDR              ? USART I/O Data Register (for TX and RX)
 * UBRRL and UBRRH  ? USART Baud Rate Registers
 
 */

void initUART(int baud, int data_mode, int clk_mode);
void UART_send (char data);
void UART_sen_str (char* data);
char UART_receive();
void UART_receive_no_pooling();
void UART_flush();

//enable interrupts
void UART_TXINT_EN();
void UART_RXINT_EN();
void UART_DATAINT_EN();

#endif	/* MUART_H */

