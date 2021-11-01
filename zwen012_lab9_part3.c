/*	Author: Zhiyuan Wen
 *	Lab Section: 021
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description:
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/drive/u/2/folders/1-35d06_1YBgX1yZ-u_84sli5ApimIx7z
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    unsigned char button = 1;
    unsigned char playT = 0;
    unsigned char i = 0;
    PWM_on();
    double notes[] = {261.63, 329.63, 392.00, 329.63, 261.63};
    while(1) {
        if((PINA & 0x01 && button) || playT) {
            if(i < 5) {
                set_PWM(notes[i]);
                i += 1;
                playT = 1;
            }
            else {
                set_PWM(0);
                playT = 0;
            }
            button = 0;
        }
        if(!(PINA & 0x01)) {
            button = 1;
        }
    }
    
    PWM_off();
    return 0;
}