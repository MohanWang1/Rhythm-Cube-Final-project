/* 
 * File:   buzzer.h
 * Author: MohanWANG
 *
 * Created on May 3, 2021, 4:37 AM
 */

#ifndef BUZZER_H
#define	BUZZER_H

#ifdef	__cplusplus
extern "C" {
#endif

    //preset the tone of music
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
    
    
#define PRX(f) (1/(f)*1000000/62.5 - 1)    //the calculation of the pre scalar
    
    //music info array of Super Mario
    extern int melody[];
    void buzzer_init();             //buzzer
    void playRate_set(int refreshRate);
    void voice_set(int val);
    void tone_play(int f);

#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

