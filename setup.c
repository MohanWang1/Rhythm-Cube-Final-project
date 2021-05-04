/*
 * File:   setup.c
 * Author: Mohan Wang
 *
 * Created on March 30, 2021, 5:24 PM
 */


#include "xc.h"
#include "setup.h"


void setup(){
    CLKDIVbits.RCDIV= 0;
    AD1PCFG = 0x9fff;
    //TRISB = 0x0003;
    //TRISA = 0b1111111111111110;
    //LATA = 0x0000;
    TRISBbits.TRISB6 = 0;
    
    
    //T1 setup
    T1CONbits.TON = 0;
    T1CON = 0;
    T1CONbits.TCKPS = 0b01;
    TMR1 = 0;
    PR1 = 0;
    T1CONbits.TON = 1;
    
    //T2 setup
    T2CONbits.TON = 0;
    T2CON = 0;
    
}

void delay_ms(unsigned long int ms){
     while(ms>0){
        asm("repeat #15998");
        asm("nop");
        ms--;
    }
}
