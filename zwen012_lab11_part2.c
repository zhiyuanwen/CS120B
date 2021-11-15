/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #2
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

char str1[] = "CS120B is Legend... wait for it DARY!";
unsigned char i = 0;
unsigned str2[] = "";

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
            LCD_ClearScreen();
            if(i >= sizeof(str1)) {
                str2 += str1[sizeof(str1) - i];
                i--;
            }
            else {
                str2 += str1[i];
                i++;
            }
            LCD_DisplayString(str2);
            break;
        default:
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

    task1.state = display_display;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &displaySMTick;

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
            tasks[i] -> elapsedTime += GCD;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}