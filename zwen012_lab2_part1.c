/*	Your Name & E-mail: Zhiyuan Wen (zwen012@ucr.edu)
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: 
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include “simAVRHeader.h”
#endif	

int main(void) {
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    unsigned char tmpA = 0x00;
    while(1) {
        tmpA = PINA & 0x03;
        if(tmpA == 0x01) {
            PORTB = 0x01;
        }
        else {
            PORTB = 0x00;
        }
    }
    return 0;
}
