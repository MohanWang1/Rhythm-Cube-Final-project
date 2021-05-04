/*
 * File:   LED_display.c
 * Author: Mohan Wang
 *
 * Created on April 2, 2021, 4:58 PM
 */


#include "xc.h"
#include "LED_display.h"
#include "setup.h"
#include "WS2812B_write.h"
#include "stdio.h"

//int NUM_LEDS ;                  

void LED_writeColor(int r, int g, int b){
    int count=0;
    for(count = 8; count >0; count--){
        if(r&0x80){
            write_1();
        }
        else{
            write_0();
        }
        
        r = r << 1;
    }
    for(count = 8; count >0; count--){
        if(g&0x80){
            write_1();
        }
        else{
            write_0();
        }
        
        g = g << 1;
    }
    for(count = 8; count >0; count--){
        if(b&0x80){
            write_1();
        }
        else{
            write_0();
        }
        
        b = b << 1;
    }
    //wait_100us();
}

void LED_YELLOW(){
    LED_writeColor(204, 225, 33);
}


void LED_GREEN(){
    LED_writeColor(255, 0, 0);
}

void LED_RED(){
    LED_writeColor(0, 255, 0);
}

void LED_BLUE(){
    LED_writeColor(0, 0, 255);
}

void LED_WHITE(){//LED display write color
    LED_writeColor(251, 252, 252);
}

void start_Display(struct LED *stripe){
    int count;
    int i;
    //int c = 255/stripe->NUM_LEDS;
    for(count = 0; count<= stripe->NUM_LEDS; count ++){
        for(i = 0; i < count; i++){
            LED_GREEN();
        }
        reset();
        delay_ms(200);
    }
}

//
//
//
//void MtoD(int n){
//    if(n){
//        LED_WHITE();
//    }
//    else
//        LED_RED();
//}