/* 
 * File:   SPI_SLAVE_TEST.h
 * Author: User
 *
 * Created on September 16, 2024, 10:38 PM
 */

#ifndef SPI_SLAVE_TEST_H
#define	SPI_SLAVE_TEST_H

/*to send data to a device like memory card
1) send instruction (send data, go home, ...)
2) send address
3) send data
 
---- to receive data we send a dummy data like (0x00) just to push
     the shift registers to send the data in the device back to the master */

#define EE_READ     0x03       //Read data from memory array beginning at selected address
#define EE_WRITE    0x02       //Write data to memory array beginning at selected address
#define EE_WREN     0x06       //Set the write enable latch (enable write operations)
#define EE_RDSR     0x05       //Read STATUS register
#define EE_WRSR     0x01       //Write STATUS register

/* page address begins with XXXX 0000 
 * and ends with XXXX 1111.
 */

void EE_SEND_DATA (char address, char data);
void EE_SEND_DATA_STR (char address, char* data);
void EE_SEND_DATA_ARR (char address, char* data, char size); //array you want to write from your code to EEPROM
char EE_READ_DATA (char address);
void EE_READ_DATA_ARR (char address, char* data, char size); //array you want to write to from EEPROM to code
char check_error(char size);


#endif	/* SPI_SLAVE_TEST_H */

