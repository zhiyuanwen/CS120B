/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #9  Exercise #2
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

void set_PWM(double frequency) {
    static double current_frequency;
    if(frequency != current_frequency) {
        if(!frequency) { TCCR3B &= 0x08; }
        else { TCCR3B |= 0x03; }
        if(frequency < 0.954) { OCR3A = 0xFFFF; }
        else if(frequency > 31250) { OCR3A = 0x0000; }
        else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum inc_States { inc_SMStart, inc_s0, inc_Up, inc_Down, inc_Reset } inc_State;
unsigned char noteNum = 7;
unsigned char offOn = 1;
void increment_Button() {
    switch(inc_State) {
        case inc_SMStart:
            inc_State = inc_s0; //Go to state0
            break;
        case inc_s0:
            if(PINA & 0x04) { //Sound off/on
                inc_State = inc_Reset;
            }
            else if(PINA & 0x01) { //Up 1 note
                inc_State = inc_Up;
            }
            else if(PINA & 0x02) { //Down 1 note
                inc_State = inc_Down;
            }
            break;
        case inc_Up:
            inc_State = inc_s0;
            break;
        case inc_Down:
            inc_State = inc_s0;
            break;
        case inc_Reset:
            inc_State = inc_s0;
            break;
        default:
            inc_State = inc_SMStart;
            break;
    }
    
    switch(inc_State) {
        case inc_SMStart:
            break;
        case inc_s0:
            break;
        case inc_Up:
            if(noteNum < 7) {
                noteNum += 1;
            }
            break;
        case inc_Down:
            if(noteNum > 0) {
                noteNum -= 1;
            }
            break;
        case inc_Reset:
            if(offOn == 0) {
                set_PWM(0);
                offOn = 1;
            }
            else {
                offOn = 0;
            }
            break;
        default:
            PORTB = 0x07;
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    PWM_on();
    while(1) {
        increment_Button();
        if(offOn == 0) {
            if(noteNum == 0) {
                set_PWM(261.63);
            }
            else if(noteNum == 1) {
                set_PWM(293.66);
            }
            else if(noteNum == 2) {
                set_PWM(329.63);
            }
            else if(noteNum == 3) {
                set_PWM(349.23);
            }
            else if(noteNum == 4) {
                set_PWM(392.00);
            }
            else if(noteNum == 5) {
                set_PWM(440.00);
            }
            else if(noteNum == 6) {
                set_PWM(493.88);
            }
            else if(noteNum == 7) {
                set_PWM(523.25);
            }
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    
    PWM_off();
    return 0;
}