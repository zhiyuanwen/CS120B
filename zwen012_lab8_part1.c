/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: x
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    ADC_init();
    unsigned short x = ADC;
    while(1) {
        x = ADC;
        PORTB = (char)x;
        PORTD = ((char)(x >> 8)) & 0x03;
    }
    return 0;
}