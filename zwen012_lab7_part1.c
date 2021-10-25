/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
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

enum inc_States { inc_SMStart, inc_s0, inc_Up, inc_Down, inc_Reset } inc_State;
unsigned char count = 10;
unsigned char tmpC = 0;
void increment_Button() {
    switch(inc_State) {
        case inc_SMStart:
            inc_State = inc_s0; //Go to state0
            break;
        case inc_s0:
            if((PINA & 0x01) && (PINA & 0x02)) { //Reset the count
                inc_State = inc_Reset;
            }
            else if(PINA & 0x01) { //Go up 1
                inc_State = inc_Up;
                count = 10;
            }
            else if(PINA & 0x02) { //Go down 1
                inc_State = inc_Down;
                count = 10;
            }
            break;
        case inc_Up:
            if((PINA & 0x01) && (PINA & 0x02)) { //Reset if A1 pushed
                inc_State = inc_Reset;
            }
            else if(!(PINA & 0x01)) { //Continue until release
                inc_State = inc_s0;
            }
            else if(PINA & 0x02) { //Go straight to down 1
                inc_State = inc_Down;
                count = 10;
            }
            break;
        case inc_Down:
            if((PINA & 0x01) && (PINA & 0x02)) { //Reset if A0 pushed
                inc_State = inc_Reset;
            }
            else if(PINA & 0x01) { //Straight to up 1
                inc_State = inc_Up;
                count = 10;
            }
            else if(!(PINA & 0x02)) { // Continue until release
                inc_State = inc_s0;
            }
            break;
        case inc_Reset:
            if(!(PINA & 0x01) && !(PINA & 0x02)) {
                inc_State = inc_s0; //Go back to start
            }
            break;
        default:
            inc_State = inc_SMStart;
            break;
    }
    
    switch(inc_State) {
        case inc_SMStart:
            PORTC = 0x07;
            break;
        case inc_s0:
            break;
        case inc_Up:
            if(PINA & 0x01 && count < 10) {
                count++;
            }
            if(count >= 10 && tmpC < 9) {
                tmpC += 1;
                count = 0;
            }
            PORTC = tmpC;
            break;
        case inc_Down:
            if(PINA & 0x02 && count < 10) {
                count++;
            }
            if(count >= 10 && tmpC > 0) {
                tmpC -= 1;
                count = 0;
            }
            PORTC = tmpC;
            break;
        case inc_Reset:
            tmpC = 0;
            PORTC = tmpC;
            break;
        default:
            PORTC = 0x07;
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    TimerSet(100);
    TimerOn();
    LCD_init();
    inc_State = inc_SMStart;
    while(1) {
        increment_Button();
        while(!TimerFlag);
        TimerFlag = 0;
        LCD_ClearScreen();
        LCD_Cursor(1);
        LCD_WriteData(tmpC + '0');
    }
    return 0;
}