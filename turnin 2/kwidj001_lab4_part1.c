/*	Author: Karsten
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "header/simAVRHeader.h"
#endif

#define button (PINA & 0x01) //button = PINA & 0x01 (PA0);

enum LEDStates {SM_Start, SM_LED0, SM_WAIT0, SM_LED1, SM_WAIT1} SM_State;

void tick(){ 
    //transitions
    switch(SM_State){
        case SM_Start:
            SM_State = SM_LED0;
            break;
        case SM_LED0:
            SM_State = button ? SM_LED1 : SM_LED0; //if button pressed turn on led1 if not stay in state led 0 on
            break;
        case SM_LED1:
            SM_State = button ? SM_LED0 : SM_LED1; //if button pressed turn on led0 if not stay in led1
            break;
        default:
            SM_State = SM_Start;
            break;
    }
    //state actions
    switch(SM_State){
        case SM_Start:
            break;
        case SM_LED0:
            PORTB = 0x01;
            break;
        case SM_LED1:
            PORTB = 0x02;
            break;
        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    SM_State = SM_Start;
    while (1) {
        tick();
    }
    return 1;
}
