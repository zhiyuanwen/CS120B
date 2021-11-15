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
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

const char* str1 = "                CS120B is Legend...wait for it DARY!                ";
char* str2 = "";
unsigned char i = 0;

enum display_Text { display_p1 };
int displaySMText(int state) {
    switch(state) {
        case display_p1:
            state = display_p1;
            break;
        default:
            state = display_p1;
            break;
    }
    switch(state) {
        case display_p1:
            str2 = str1[i];
            for(unsigned char j = i + 1; j < i + 16; j++) {
                str2 = str2 + str1[j];
            }
            LCD_DisplayString(1, str2);
            i = (i + 1) % 68;
            break;
        default:
            break;
    }
    return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0x00; PORTC = 0xFF;
    
    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = -1;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &displaySMText;

    unsigned short i;
    unsigned long GCD = tasks[0] -> period;
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