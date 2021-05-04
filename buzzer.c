/*
 * File:   botton.c
 * Author: Mohan Wang
 *
 * Created on March 30, 2021, 5:43 PM
 */


#include "xc.h"
#include "buzzer.h"

int melody[] = {
        E5, E5, 0, E5, 0, C5, E5, 0,
        G5, 0, 0, 0, G4, 0, 0, 0,

        C5, 0, 0, G4, 0, 0, E4, 0,
        0, A4, 0, B4, 0, AS4, A4, 0,

        G4, E5, G5, A5, 0, F5, G5,
        0, E5, 0, C5, D5, B4, 0, 0,

        C5, 0, 0, G4, 0, 0, E4, 0,
        0, A4, 0, B4, 0, AS4, A4, 0,

        G4, E5, G5, A5, 0, F5, G5,
        0, E5, 0, C5, D5, B4, 0, 0
    };

//buzzer initialize
void buzzer_init(){
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    TRISBbits.TRISB6 = 0;        //set RB6 to output mode
    
    __builtin_write_OSCCONL(OSCCON & 0xBF);//bond OC to RP6
    RPOR3bits.RP6R = 18;
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    OC1CON = 0;
    OC1CONbits.OCTSEL = 0b1;
    OC1CONbits.OCM = 0b110;
    
    //T3 setup to output PWM
    //PWM could control the tone and Open/Close of buzzer
    T3CON = 0;
    T3CONbits.TCKPS = 0b00; //set pre scalar to 1
    TMR3 = 0;                
    PR3 = 400; //set PR3
    
    T3CONbits.TON = 1; // enable Timer3
    OC1RS = 0 ; //initial OC1RS
    
    //T4 setup to play music
    T4CON = 0;
    T4CONbits.TCKPS = 0b00;
    TMR4 = 0;
    PR4 = 0; // just a setup 
    //T4CONbits.TON = 1;//enable
}

void playRate_set(int refreshRate){
    PR4 = 1/refreshRate * 1000000/62.5 - 1;
}

//set voice 
//val is the value of PRx which means the duty cycle is 50%
//if val = 0
//   the buzzer stop working 
void voice_set(int val){
    OC1RS = val/2;
}

//f is the frequence of the tone
//it should be include in the song information array
void tone_play(int f){
    int PR_Val = (int)PRX(f); 
    PR3 = PR_Val;
    voice_set(PR_Val);
}

