
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h> //for the itoa

#include "DIO.h"
#include "mLCD4.h"

//the next 2 functions are for the number to string conversion used later in
//lcd4_data_num functoin
///////////////////////////////////////////////////////////////////////////////
 static void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}
/* itoa:  convert n to characters in s */
 static void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}
///////////////////////////////////////////////////////////////////////////////
 
void initLCD4(){
    //dio() //output from the up but input to the LCD!!
    setPORTC_dir_val(0xF0, OUT);
    setPIND_dir(RS, OUT);
    setPIND_dir(RW, OUT);
    setPIND_dir(EN, OUT);
    
    //LCD setup()
    //write mode
    setPIND(RW, LOW); 
    
    //return home
    LCD4_cmd(HOME);
    
    //set mode
    LCD4_cmd(MODE_4BITS);
    
    //set cursor and display mode
    LCD4_cmd(CURSOR_OFF_DISPLAY_ON);
    
    //clear
    LCD4_CLEAR();
    LCD4_cmd(ENTRY_MODE);
}
///////////////////////////////////////////////////////////////////////////////
void LCD4_cmd(char cmd){ //we will need to multiplex the data
    //set Register Select to ZERO to select command register
    LCD_CONTROL &= ~(1<<RS);
    //to multiplex the data we will make sure that the most sig 4 bits of the LCD port are ZERO
    //and the least sig 4 bits of the command are ZERO
    //and then we OR them, thus we get the first half of our command
    LCD_DATA = (LCD_DATA & 0x0F) | (cmd & 0xF0);
    LCD4_en();
    //now we send the other 4 bits by shifting them
    LCD_DATA = (LCD_DATA & 0x0F) | (cmd << 4);
    LCD4_en();
    //notice we only use half the LCD data pins at a time as it is in 4 bits mode
    _delay_ms(20);
}
///////////////////////////////////////////////////////////////////////////////

void LCD4_data(char data){
    //set Register Select to ONE to select data register
    //the same multiplexing technique but with data instead of commands
    LCD_CONTROL |= (1<<RS);
    LCD_DATA = (LCD_DATA & 0x0F) | (data & 0xF0);
    LCD4_en();
    //now we send the other 4 bits by shifting them
    LCD_DATA = (LCD_DATA & 0x0F) | (data << 4);
    LCD4_en();
    _delay_ms(20);
}

void LCD4_data_str (char* str){  //same as 8 bits
    for (int i=0; str[i] != '\0'; i++){
        LCD4_data(str[i]); //prints a string not char on top of each other
    }
}

void LCD4_data_num(int num){ //same as 8 bits
    char str[11];
    itoa(num, str); //changes the number to a string and then sends it to the print string function
    LCD4_data_str(str);
}
///////////////////////////////////////////////////////////////////////////////
void LCD4_move_right(int x){
    for(int i=0; i<x; i++){
        LCD4_cmd(MOVE_RIGHT);
    }
}
void LCD4_move_left(int x){
    for(int i=0; i<x; i++){
        LCD4_cmd(MOVE_LEFT);
    }
}
void LCD4_goto(int row, int col){ //row is ZERO or ONE
    LCD4_cmd(LINE1 | col | (row<<6)); //according to the row it will be first or second line
}                                     //this is determined in the 7th bit (bit number 6)
///////////////////////////////////////////////////////////////////////////////
void LCD4_en(){ //same as 8 bits
    //enables the LCD by setting waiting then resetting as it is falling edge
    LCD_CONTROL |= (1<<EN);
    _delay_ms(5);
    LCD_CONTROL &= ~(1<<EN);
}

void LCD4_CLEAR(){ //we need to clear the data 4 bits at a time
    //LCD_cmd(CLEAR);
    LCD_CONTROL &= ~(1<<RS); //register on command mode (0)
    LCD_DATA = (LCD_DATA & 0x0F) | (CLEAR & 0xF0);
    LCD4_en();
    LCD_DATA = (LCD_DATA & 0x0F) | (CLEAR << 4);
    LCD4_en();

    _delay_ms(5); //we did it in a function for the delay
}

