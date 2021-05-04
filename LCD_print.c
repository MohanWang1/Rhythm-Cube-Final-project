/*
 * File:   LCD_print.c
 * Author: Mohan Wang
 *
 * Created on March 24, 2021, 4:27 PM
 */


#include "xc.h"
#include"LCD_print.h"
#include "string.h"
#include "setup.h"

void lcd_cmd(char command){
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}

void lcd_init(){
    I2C2BRG = 0x9D; // should be for 100KHz
    IFS3bits.MI2C2IF = 0; //clear Interrupt Flag
    I2C2CONbits.I2CEN = 1;//Enable I2C2
    
    delay_ms(40);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // internal osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100);    
    
    delay_ms(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    
    
    delay_ms(5);
}

void lcd_printChar(char myChar){
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}




void lcd_setCursor(char x, char y){
    char cursor =0x80| (0x40*y + x);
    lcd_cmd(cursor);
}

void lcd_printStr(const char str[]){
    int len = (unsigned)strlen(str)-1;
    int count = 0;
    
    I2C2CONbits.SEN = 1;//send control bit
    while(I2C2CONbits.SEN == 1);//wait control bit 
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;//send slaver address
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    for(count = 0; count < len; count ++){//send every data in string to lcd chip
        I2C2TRN = 0b11000000;
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        
        I2C2TRN = str[count];
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
    }
    I2C2TRN = 0b01000000;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = str[len];//send final data
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;//send stop bit
    while(I2C2CONbits.PEN == 1);
}

void shiftLeft(){//string shift to left on LCD
    lcd_cmd(0b00011000);
}

void shiftRight(){//string shift to right on LCD
    lcd_cmd(0b00011100);
}