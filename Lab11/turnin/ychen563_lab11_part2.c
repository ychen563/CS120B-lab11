/*	Author: ychen563
 *	Lab Section:024
 *	Assignment: Lab #11  Exercise #2
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

unsigned char message[] = " CS120B is Legend... wait for it DARY!     ";


enum LCD_display_states {LCD_display};

int LCDSMTick(int state){
	
    unsigned char display[16] ="";
    unsigned short i;
    
    switch(state){
        case LCD_display: state = LCD_display; break;
        default: state = LCD_display; break; 
    }
    switch(state){
        case LCD_display:
            for (i = 0; i < ((sizeof(message)/sizeof(message[0]))-2); i++){
                message[i] = message[i+1];
            }
            message[(sizeof(message)/sizeof(message[0]))-2] = message[0];
            
            for (i = 0; i < ((sizeof(display)/sizeof(display[0]))-1); i++){
                display[i] = message[i];
            }
            display[(sizeof(display)/sizeof(display[0]))-1] = '\0';
            break;
        default: break;
    }
    LCD_DisplayString(1, display);
    return state;
}

int main() {
    /* Insert DDR and PORT initializations */
    DDRC = 0xF0; PORTC = 0x0F;
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    static task task2;
    task *tasks[] = {&task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    task2.state = start;
    task2.period = 300;
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
