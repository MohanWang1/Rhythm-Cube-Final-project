/* 
 * File:   LCD_print.h
 * Author: Mohan Wang
 *
 * Created on March 24, 2021, 4:26 PM
 */

#ifndef LCD_PRINT_H
#define	LCD_PRINT_H

#ifdef	__cplusplus
extern "C" {
#endif

    void lcd_cmd(char command);
    void lcd_init();
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char str[]);
    void shiftLeft();
    void shiftRight();


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_PRINT_H */

