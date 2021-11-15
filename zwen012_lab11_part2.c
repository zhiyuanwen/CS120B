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

enum display_Text { display_p1, display_p2, display_p3 };
int displaySMText(int state) {
    switch(state) {
        case display_p1:
            state = display_p2;
            break;
        case display_p2:
            state = display_p3;
            break;
        case display_p3:
            state = display_p1;
            break;
        default:
            state = display_p1;
            break;
    }
    switch(state) {
        case display_p1:
            LCD_DisplayString(1, "CS120B is Legend")
            break;
        case display_p2:
            LCD_DisplayString(1, "wait for it...")
            break;
        case display_p3:
            LCD_DisplayString(1, "DARY!")
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
    DDRD = 0xFF; PORD = 0x00;
    
    static task task1;
    task *tasks[] = { &task1 };

    task1.state = -1;
    task1.period = 5000;
    task1.elapsedTime = task1.period;
    task1.TickFct = &displaySMText;

    TimerSet(5000);
    TimerOn();
    LCD_init();
    LCD_ClearScreen();
    while(1) {
        for(i = 0; i < numTasks; ++i) {
            if(tasks[i] -> elapsedTime == tasks[i] -> period) {
                tasks[i] -> state = tasks[i] -> TickFct(tasks[i] -> state);
                tasks[i] -> elapsedTime = 0;
            }
            tasks[i] -> elapsedTime += 5000;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}