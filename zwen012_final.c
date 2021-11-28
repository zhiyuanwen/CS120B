/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Final Project
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *  Demo Link #1: N/A
 *	Demo Link #2: 
 *  Final Project Demo: 
 */

#include <stdio.h>
#include <avr/io.h>
#include "timer.h"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//A0 is on/off, A1 is speed up, A2 is speed down
//A3 is rotate, A4 is up/down, A5 is left/right

char display[11][8] = {0};
unsigned char power = 0;
unsigned char score = 0;
unsigned char highScore = score;
unsigned char tmpOut = 0x00;

//Game On/Off SM
enum game_States { game_off, game_on };

int GameToggleSM(int state) {
    switch(state) {
        case game_off:
            if(PINA & 0x01) {
                state = game_on;
            }
            break;
        case game_on:
            if(PINA & 0x01) {
                state = game_off;
            }
            break;
        default:
            state = game_off;
            break;
    }
    
    switch(state) {
        case game_off:
            tmpOut |= 0x00;
            power = 0;
            break;
        case game_on:
            tmpOut |= 0x01;
            power = 1;
            break;
        default:
            tmpOut |= 0x00;
            power = 0;
            break;
    }
    return state;
}

//Joystick SM

void ADC_init() {
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN);
}
int ReadADC(int ADCchannel) {
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
    ADCSRA |= (1<<ADSC);
    while( ADCSRA & (1<<ADSC) );
    return ADC;
}

enum joystick_States { joystick };

unsigned short xAxis = 0;
unsigned short yAxis = 0;

int JoystickTiltSM(int state) {
    switch(state) {
        case joystick:
            break;
        default:
            state = joystick;
            break;
    }
    
    switch(state) {
        case joystick:
            xAxis = ReadADC(5);
            yAxis = ReadADC(4);
            if(xAxis >= 700 || xAxis <= 400) {
                tmpOut |= 0x20;
            }
            else {
                tmpOut &= 0xDF;
            }
            if(yAxis <= 400) {
                tmpOut |= 0x10;
            }
            else {
                tmpOut &= 0xEF;
            }
            break;
        default:
            break;
    }
    return state;
}

//Rotate SM
enum rotate_States { rotation };

int RotateButtonSM(int state) {
    switch(state) {
        case rotation:
            break;
        default:
            state = rotation;
            break;
    }
    
    switch(state) {
        case rotation:
            if(PINA & 0x08) {
                tmpOut |= 0x08;
            }
            else {
                tmpOut &= 0xF7;
            }
            break;
        default:
            break;
    }
    return state;
}

//Toggle speed SM
enum speed_States { speeder };

int SpeedToggleSM(int state) {
    switch(state) {
        case speeder:
            break;
        default:
            state = speeder;
            break;
    }
    
    switch(state) {
        case speeder:
            if(PINA & 0x02) {
                tmpOut |= 0x02;
                tmpOut &= 0xFB;
            }
            else if(PINA & 0x04) {
                tmpOut |= 0x04;
                tmpOut &= 0xFD;
            }
            else {
                tmpOut &= 0xF9;
            }
            break;
        default:
            break;
    }
    return state;
}

//Clear lines and check current is down

//Output 2d array (inputs working for now)
void OutputInputSM() {
    if(tmpOut & 0x01) {
        PORTB = tmpOut;
    }
    else {
        PORTB = 0x00;
    }
    //Nokia printer for board
    /*
    for(unsigned short i = 8; i > 0; --i) {
        print('|');
        for(unsigned short j = 0; j < 8; ++j) {
            if(display[i - 1][j] == 1) {
                print('0');
            }
            else {
                print(' ');
            }
        }
        print('|');
    }
    print("__________");
    */
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;
    
    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    const char start = -1;

    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GameToggleSM;

    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &JoystickTiltSM;

    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &RotateButtonSM;

    task4.state = start;
    task4.period = 100;
    task4.elapsedTime = task4.period;
    task4.TickFct = &SpeedToggleSM;

    unsigned short i;
    unsigned long GCD =  tasks[0] -> period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD, tasks[i] -> period);
    }
    ADC_init();
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
        OutputInputSM();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}