/*	Author: ychen563
 *	Lab Section:024
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "io.h"
#endif

unsigned char input = ' ';

enum keypad_States {keypad_display};

int keypadSMTick(int state) {
    
    unsigned char x = GetKeypadKey();
    switch (state) {
        case keypad_display: state = keypad_display; break;
        default: state = keypad_display; break;
    }
    switch (state) {
        case keypad_display:
            switch(x){
            case '1' :input = '1'; break;
            case '2' :input = '2'; break;
            case '3' :input = '3'; break;
            case '4' :input = '4'; break;
            case '5' :input = '5'; break;
            case '6' :input = '6'; break;
            case '7' :input = '7'; break;
            case '8' :input = '8'; break;
            case '9' :input = '9'; break;
            case 'A' :input = 'A'; break;
            case 'B' :input = 'B'; break;
            case 'C' :input = 'C'; break;
            case 'D' :input = 'D'; break;
            case '*' :input = '*'; break;
            case '0' :input = '0'; break;
            case '#' :input = '#'; break;
            }
        break;
       default: break;
    }
    return state;
}

enum LCD_display_states {LCD_display};

int LCDSMTick(int state){
    
    switch(state){
        case LCD_display: state = LCD_display; break;
        default: state = LCD_display; break; 
    }
    switch(state){
        case LCD_display:
            LCD_ClearScreen();
            LCD_WriteData(input);
            break;
        default: break;
    }
    return state;
}

int main() {
    /* Insert DDR and PORT initializations */
    DDRC = 0xF0; PORTC = 0x0F;
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime  = task1.period;
    task1.TickFct = &keypadSMTick;

    task2.state = start;
    task2.period = 100;
    task2.elapsedTime  = task2.period;
    task2.TickFct = &LCDSMTick;


    unsigned short i;

    unsigned long GCD = tasks[0]->period;
    for (i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();

    LCD_init();

   unsigned short j;
    while (1) {
        for (j = 0; j < numTasks; j++) {
            if (tasks[j]->elapsedTime == tasks[j]->period) {
                tasks[j]->state = tasks[j]->TickFct(tasks[j]->state);
                tasks[j] ->elapsedTime = 0;
            }
            tasks[j]->elapsedTime += GCD;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}
