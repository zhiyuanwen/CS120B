/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/drive/u/2/folders/1D3pmPrcXOmY3EycnAqFaOzG9Pg4MQ5AG
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

enum led_States { led_s0, led_s1, led_s2 } led_State;

unsigned char threeLEDs = 0x01;

void ThreeLEDsSM() {
    switch(led_State) {
        case led_s0:
            led_State = led_s1;
            break;
        case led_s1:
            led_State = led_s2;
            break;
        case led_s2:
            led_State = led_s0;
            break;
        default:
            led_State = led_s0;
            break;
    }
    
    switch(led_State) {
        case led_s0:
            threeLEDs = 0x01;
            break;
        case led_s1:
            threeLEDs = 0x02;
            break;
        case led_s2:
            threeLEDs = 0x04;
            break;
        default:
            threeLEDs = 0x01;
            break;
    }
}

enum bli_States { bli_s1, bli_s0 } bli_State;

unsigned char blinkingLED = 0x08;

void BlinkingLEDSM() {
    switch(bli_State) {
        case bli_s1:
            bli_State = bli_s0;
            break;
        case bli_s0:
            bli_State = bli_s1;
            break;
        default:
            bli_State = bli_s1;
            break;
    }
    
    switch(bli_State) {
        case bli_s1:
            blinkingLED = 0x08;
            break;
        case bli_s0:
            blinkingLED = 0;
            break;
        default:
            blinkingLED = 0x08;
            break;
    }
}

void CombineLEDsSM() {
    PORTB = blinkingLED | threeLEDs;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    while(1) {
        ThreeLEDsSM();
        BlinkingLEDSM();
        CombineLEDsSM();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}