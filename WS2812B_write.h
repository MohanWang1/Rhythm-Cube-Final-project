/* 
 * File:   WS2812B_write.h
 * Author: Mohan Wang
 *
 * Created on April 1, 2021, 11:00 PM
 */

#ifndef WS2812B_WRITE_H
#define	WS2812B_WRITE_H

#ifdef	__cplusplus
extern "C" {
#endif
void wait_100us(void);
void wait_1ms(void);
void reset(void);
void write_0(void);
void write_1(void);



#ifdef	__cplusplus
}
#endif

#endif	/* WS2812B_write */

