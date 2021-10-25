/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise #2
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

enum li_States { li_SMStart, li_s0, li_s1Up, li_s2, li_s1Down, li_Victory} li_State;
unsigned char checkV = 5;
void light_Show() {
    switch(li_State) {
        case li_SMStart:
            li_State = li_s0;
            break;
        case li_s0:
            if(checkV == 9) {
                li_State = li_Victory;
            }
            else {
                li_State = li_s1Up;
            }
            break;
        case li_s1Up:
            if(checkV == 9) {
                li_State = li_Victory;
            }
            else {
                li_State = li_s2;
            }
            break;
        case li_s2:
            if(checkV == 9) {
                li_State = li_Victory;
            }
            else {
                li_State = li_s1Down;
            }
            break;
        case li_s1Down:
            if(checkV == 9) {
                li_State = li_Victory;
            }
            else {
                li_State = li_s0;
            }
            break;
        case li_Victory:
            break;
        default:
            li_State = li_SMStart;
            break;
    }
    
    switch(li_State) {
        case li_SMStart:
            PORTB = 0x01;
            checkV = 5;
            break;
        case li_s0:
            if(PINA & 0x01) {
                checkV += 1;
            }
            LCD_ClearScreen();
            LCD_Cursor(1);
            LCD_WriteData(checkV + '0');
            PORTB = 0x01;
            break;
        case li_s1Up:
            if(PINA & 0x01) {
                checkV -= 1;
            }
            LCD_ClearScreen();
            LCD_Cursor(1);
            LCD_WriteData(checkV + '0');
            PORTB = 0x02;
            break;
        case li_s2:
            if(PINA & 0x01) {
                checkV += 1;
            }
            LCD_ClearScreen();
            LCD_Cursor(1);
            LCD_WriteData(checkV + '0');
            PORTB = 0x04;
            break;
        case li_s1Down:
            if(PINA & 0x01) {
                checkV -= 1;
            }
            LCD_ClearScreen();
            LCD_Cursor(1);
            LCD_WriteData(checkV + '0');
            PORTB = 0x02;
            break;
        case li_Victory:
            LCD_DisplayString(1, "Victory!");
            break;
        default:
            PORTB = 0x01;
            checkV = 5;
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x05;
    DDRD = 0xFF; PORTD = 0x00;
    TimerSet(300);
    TimerOn();
    LCD_init();
    li_State = li_SMStart;
    PORTB = 0x01;
    while(1) {
        light_Show();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}