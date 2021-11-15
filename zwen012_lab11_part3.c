/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #3
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

unsigned char displayL = 0x00;

int toggleKeypadSMState(int state) {
    switch(state) {
        case '0':
            PORTB = 0x00;
            break;
        case '1':
            PORTB = 0x01;
            break;
        case '2':
            PORTB = 0x02;
            break;
        case '3':
            PORTB = 0x03;
            break;
        case '4':
            PORTB = 0x04;
            break;
        case '5':
            PORTB = 0x05;
            break;
        case '6':
            PORTB = 0x06;
            break;
        case '7':
            PORTB = 0x07;
            break;
        case '8':
            PORTB = 0x08;
            break;
        case '9':
            PORTB = 0x09;
            break;
        case 'A':
            PORTB = 0x0A;
            break;
        case 'B':
            PORTB = 0x0B;
            break;
        case 'C':
            PORTB = 0x0C;
            break;
        case 'D':
            PORTB = 0x0D;
            break;
        case '*':
            PORTB = 0x0E;
            break;
        case '#':
            PORTB = 0x0F;
            break;
        case '\0':
            break;
        default:
            PORTB = 0x1B;
            break;
    }
    return state;
}

enum display_States { display_display };
int displaySMTick(int state) {
    unsigned char output;
    switch(state) {
        case display_display:
            state = display_display;
            break;
        default:
            state = display_display;
            break;
    }
    switch(state) {
        case display_display:
            output = displayL;
            break;
        default:
            break;
    }
    PORTB = output;
    LCD_ClearScreen();
    LCD_Cursor(1);
    LCD_WriteData(output);
    return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = GetKeypadKey();
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &toggleKeypadSMState;

    task2.state = display_display;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &displaySMTick;

    unsigned short i;
    unsigned long GCD =  tasks[0] -> period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD, tasks[i] -> period);
    }
    TimerSet(GCD);
    TimerOn();
    LCD_init();
    while(1) {
        for(i = 0; i < numTasks; ++i) {
            if(tasks[i] -> elapsedTime == tasks[i] -> period) {
                tasks[i] -> state = tasks[i] -> TickFct(tasks[i] -> state);
                tasks[i] -> elapsedTime = 0;
            }
            tasks[i] -> elapsedTime += GCD;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}