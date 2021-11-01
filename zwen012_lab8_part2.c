/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise #2
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
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    ADC_init();
    unsigned short x = ADC;
    unsigned short ADC_max = 0;
    unsigned short ADC_min = 0xFFFF;
    while(1) {
        x = ADC;
        PORTB = (char)x;
        PORTD = ((char)(x >> 8)) & 0x03;
        if(x > ADC_max) {
            ADC_max = x;
        }
        if(x < ADC_min) {
            ADC_min = x;
        }
    }
    return 0;
}

//I use code from part 1 to determine what min and max are since they are outputted on ports B and D