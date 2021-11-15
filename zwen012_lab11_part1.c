/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: 
 */

#include <avr/io.h>
#include "timer.h"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



enum keypad_States { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, *, #, \0};
int toggleKeypadSMState(int state) {
    
    switch(state) {
        case 0:
            PORTB = 0x00;
            break;
        case 1:
            PORTB = 0x01;
            break;
        case 2:
            PORTB = 0x02;
            break;
        case 3:
            PORTB = 0x03;
            break;
        case 4:
            PORTB = 0x04;
            break;
        case 5:
            PORTB = 0x05;
            break;
        case 6:
            PORTB = 0x06;
            break;
        case 7:
            PORTB = 0x07;
            break;
        case 8:
            PORTB = 0x08;
            break;
        case 9:
            PORTB = 0x09;
            break;
        case A:
            PORTB = 0x0A;
            break;
        case B:
            PORTB = 0x0B;
            break;
        case C:
            PORTB = 0x0C;
            break;
        case D:
            PORTB = 0x0D;
            break;
        case *:
            PORTB = 0x0E;
            break;
        case #:
            PORTB = 0x0F;
            break;
        case \0:
            PORTB = 0x1F;
            break;
        default:
            PORTB = 0x1B;
            break;
    }
    return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    static _task task1;
    _task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = GetKeypadKey();
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &toggleKeypadSMState;

    unsigned short i;
    unsigned long GCD =  tasks[0] -> period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD, tasks[i] -> period);
    }
    TimerSet(GCD);
    TimerOn();
    while(1) {
        for(i = 0; i < numTasks; ++i) {
            if(tasks[i] -> elapsedTime == tasks[i] -> period) {
                tasks[i] -> state = tasks[i] -> TickFct(tasks[i] -> state);
                tasks[i] -> elapsedTime = 0;
            }
            tasks[i] -> elapsedTime += 10;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}