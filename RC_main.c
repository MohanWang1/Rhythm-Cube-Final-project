/*
 * File:   RC_main.c
 * Author: Mohan Wang
 *
 * Created on March 30, 2021, 6:26 AM
 */


#include "xc.h"
#include"setup.h"
#include "LCD_print.h"
#include "LED_display.h"
#include "button.h"
#include "WS2812B_write.h"
#include"stdio.h"
#include "buzzer.h"
//#include "assert.h"

#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


#define BUFFER_SIZE 2   //Buffer size of IC
#define LED_LEN 19      //LENGTH of LED stripe
#define REFRESHRATE 4   //LED REFRESHRATE
#define SONG_SIZE 118   //song array size for LED
volatile unsigned long int buffer[BUFFER_SIZE] = {0}; 
volatile unsigned int readInx = 0, writeInx = 0, numElemsInBuff = 0; //index for buffer
volatile int status_button = 0; // the flag of button
volatile int T2_overflow = 0;      //overflow for timer2
volatile int T4_overflow = 0;      //overflow for timer4
//extern int NUM_LEDS;
//volatile int LED_Song[100];
volatile int LED_STA;   //LED status
volatile int SCORE = 0;  //SCORE of player
volatile int playFlag = 0;  

//song information array of Super Mario for LED display
volatile int LED_Song[SONG_SIZE] = 
{
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
    
  1, 1, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 0, 1, 0, 0, 0,

  1, 0, 0, 1, 0, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 1, 0,

  1, 1, 1, 1, 0, 1, 1,
  0, 1, 0, 1, 1, 1, 0, 0,

  1, 0, 0, 1, 0, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 1, 0,

  1, 1, 1, 1, 0, 1, 1,
  0, 1, 0, 1, 1, 1, 0, 0,

  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0};

//T1 interrupt for LED display
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0;
}

//T2 interrupt for button check
void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    T2_overflow++;
    _T2IF = 0;
}

//play music
//void __attribute__((interrupt, auto_psv)) _T4Interrupt(void)
//{
//    _T4IF = 0;
//    voice_set(0);
//    tone_play(melody[T4_overflow]);
//    T4_overflow ++;
//}

void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void) //Input Capture interrupt
{
    _IC1IF = 0;    
//    if(playFlag == 0){
    unsigned long int current;
    unsigned long int tempDuty;
    
    current = (unsigned long int)IC1BUF;//get value from IC buffer
    tempDuty = current + (unsigned long int)((62500)*T2_overflow); //calculate the real time 
    
    if(tempDuty > 250) //de-bounce
    {  
        if(numElemsInBuff < BUFFER_SIZE)
        {
            buffer[writeInx++] = tempDuty; 
            writeInx %= BUFFER_SIZE; //helps with the loop wrapping using Modulus
            ++numElemsInBuff;
        }

        TMR2 = 0;
        T2_overflow = 0;//reset 
        if(status_button==1) SCORE++; //calculate score
    }

}


//convert the information of music to LED stripe output information
//output PF 
int music_Display(volatile int* music, struct LED *stripe, int PF){
    int count = 0;
    if(PF>(SONG_SIZE - stripe->NUM_LEDS)) 
        return -1;//check if the play is already finish, return -1 to end 
    for(count = 0; count<stripe->NUM_LEDS; count++ ){
        if(music[PF+count]){
            LED_RED();
        }   
        else
            LED_WHITE();
    }
    status_button = music[PF+1];
    return PF + 1; 
}

int main(void) {
    setup();
    buzzer_init(); 
    lcd_init();
    button_init();
    lcd_setCursor(0, 0);

    struct LED led;
    struct LED *stripe = &led;
    
    (*stripe).NUM_LEDS = LED_LEN;
    (*stripe).REFRESH_RATE = REFRESHRATE;
    PR1 = 1/stripe->REFRESH_RATE * 1000000/62.5/8 - 1; //set refresh rate
    //PR1 = 399;
    start_Display(stripe);
    char str[10];
    
    
    lcd_printStr("Welcome! Double click to start game");
    while(!double_check(T2_overflow, numElemsInBuff,readInx, buffer, BUFFER_SIZE)){ // double click to start
        shiftLeft();
        delay_ms(400);
        //T4CONbits.TON = 1;//enable music
    }
    while(1){
        
        if(_T1IF==0){ // following by refresh rate
            playFlag = music_Display(LED_Song, stripe, playFlag);
            if(status_button==0){
                voice_set(0);
            
            }
            else{
                voice_set(400);
            }
            
        }
        if(status_button==0){
            voice_set(0);
            
        }
        else{
            voice_set(400);
        }
        if(playFlag == -1){
            voice_set(0);
            break;
        }
        
        
        lcd_init();
        lcd_setCursor(0,0);
        sprintf(str,"%d", playFlag);
        lcd_printStr(str);
        shiftLeft();
        //delay_ms(400);
               

    }
    
    //show final score
    lcd_init();
    lcd_setCursor(0,0);
    sprintf(str,"SCORE: %d", SCORE); 
    lcd_printStr(str);
    
    while(1){
        shiftLeft();
        delay_ms(400);
    }
    
    return 0;
}
