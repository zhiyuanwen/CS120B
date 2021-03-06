/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise #3
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
    unsigned short ADC_max = 0x02FF;
    unsigned short ADC_min = 0x00FF;
    while(1) {
        x = ADC;
        if(x >= ADC_max / 2) {
            PORTB = 0x01;
        }
        else {
            PORTB = 0x00;
        }
    }
    return 0;
}