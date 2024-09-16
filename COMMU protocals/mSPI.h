/* 
 * File:   mSPI.h
 * Author: User
 *
 * Created on September 13, 2024, 6:11 PM
 */

#ifndef MSPI_H
#define	MSPI_H

#include <avr/io.h>

#define MASTER      1
#define SLAVE       0

#define CLK_2       0
#define CLK_4       1
#define CLK_8       2
#define CLK_16      3
#define CLK_32      4
#define CLK_64_     5
#define CLK_128     6

#define MOSI        PB5
#define MISO        PB6
#define SCK         PB7
#define SS          PB4

#define MSB_FIRST   0
#define LSB_FIRST   1

/*
 REGISTERS:
 * SPCR ? SPI Control Register
 * SPSR ? SPI Status Register
 * SPDR ? SPI Data Register
 
 */

void initSPI(char mode);
void SPIclk(char clk);
void SPI_INT_EN();

void SPIsend(char data);
void SPIsend_str(char* data);
char SPIreceive();
char SPI_MASTER_receive();

void SPI_DATA_ORDER(char order);
void SPI_selectSlave(int slavePin);
void SPI_deselectSlave(int slavePin);
char SPI_check_collision();

#endif	/* MSPI_H */

