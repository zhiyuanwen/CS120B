/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    while(1) {
        PORTB = 0x00;
        while(!TimerFlag);
        TimerFlag = 0;

        PORTB = 0x01;
        while(!TimerFlag);
        TimerFlag = 0;

        PORTB = 0x02;
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}