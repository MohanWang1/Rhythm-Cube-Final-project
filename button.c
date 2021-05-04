/*
 * File:   botton.c
 * Author: Mohan Wang
 *
 * Created on March 30, 2021, 5:43 PM
 */


#include "xc.h"
#include "button.h"
#include "setup.h"



void button_init(){

    
    TRISBbits.TRISB8 = 1;
    CNPU2bits.CN22PUE = 1;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    _IC1R = 8;      //RPINR7bits.IC1R = 8; is also an option
    __builtin_write_OSCCONL(OSCCON & 0x40);
    
//set up timer 2
    T2CON = 0;
    T2CONbits.TCKPS = 0b11; //256 size
    PR2 = 62499;  
    TMR2 = 0;
    
    _T2IE = 1;
    _T2IF = 0;
    
    IC1CON = 0;
    IC1CONbits.ICTMR = 1;
    IC1CONbits.ICM = 0b010; 

    _IC1IF = 0;
    _IC1IE = 1;
    IC1BUF = 0; //reset the buffer
    
    T2CONbits.TON = 1; 
    
}

//read the buffer
unsigned long int readFromBuffer(int *buffer, int buffer_size, volatile unsigned int readInx, volatile unsigned int numElemsInBuff)
{
    unsigned long int x;
    
    //assert(numElemsInBuff>0);
    x = buffer[readInx++];
    readInx %= buffer_size;     //same modulus concept
    --numElemsInBuff;
    
    return x;
}

int double_check(volatile int overflow, volatile int numElemsInBuff, volatile unsigned int readInx, volatile unsigned long int *buffer, int buffer_size){
    unsigned long int x = 0;
    while(TMR2 + ((unsigned long int)overflow * (unsigned long int)(62500)) < 125000)   //checks if 2 seconds have passed w/o press
        {                                                                               //stays in while loop if less than 2 sec
            if(numElemsInBuff == 0)
            {
                continue;
            }

            x = readFromBuffer((int*)buffer, buffer_size, readInx, numElemsInBuff); //will return a cycle unit value for  clock time

            if(x!=0 && (buffer[1]!=0))  
            {
                if(x < 15625) //this cycle value is for double click it is less .25ms
                {
                    return 1; //center position
                }
            }
        }
    return 0;
}
