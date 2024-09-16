#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h> //for the itoa

#include "DIO.h"
#include "mLCD.h"

//the next 2 functions are for the number to string conversion used later in
//lcd_data_num functoin
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
 
void initLCD(){
    //dio() //output from the up but input to the lcd!!
    setPORTC_dir(OUT);
    setPIND_dir(RS, OUT);
    setPIND_dir(RW, OUT);
    setPIND_dir(EN, OUT);
    
    //LCD setup()
    //write mode
    setPIND(RW, LOW); 
    
    //return home
    LCD_cmd(HOME);
    
    //set mode
    LCD_cmd(MODE_8BITS);
    
    //set cursor and display mode
    LCD_cmd(CURSOR_OFF_DISPLAY_ON);
    
    //clear
    LCD_CLEAR();
    LCD_cmd(ENTRY_MODE);
}
///////////////////////////////////////////////////////////////////////////////
void LCD_cmd(char cmd){
    //set Register Select to ZERO to select command register
    LCD_CONTROL &= ~(1<<RS);
    LCD_DATA = cmd;
    LCD_en();
    _delay_ms(20);
}
///////////////////////////////////////////////////////////////////////////////
void LCD_data(char data){
    //set Register Select to ONE to select data register
    LCD_CONTROL |= (1<<RS);
    LCD_DATA = data;
    LCD_en();
    _delay_ms(20);
}

void LCD_data_str (char* str){ 
    for (int i=0; str[i] != '\0'; i++){
        LCD_data(str[i]); //prints a string not char on top of each other
    }
}

void LCD_data_num(int num){
    char str[11];
    itoa(num, str); //changes the number to a string and then sends it to the print string function
    LCD_data_str(str);
}
///////////////////////////////////////////////////////////////////////////////
void LCD_en(){
    //enables the LCD by setting waiting then resetting as it is falling edge
    LCD_CONTROL |= (1<<EN);
    _delay_ms(5);
    LCD_CONTROL &= ~(1<<EN);
}

void LCD_CLEAR(){
    //LCD_cmd(CLEAR);
    LCD_CONTROL &= ~(1<<RS); //register on command mode (0)
    LCD_DATA = CLEAR; 
    LCD_en();
    _delay_ms(5); //we did it in a function for the delay
}

