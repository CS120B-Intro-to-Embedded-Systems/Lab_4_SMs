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
#include <simAVRHeader.h>
#endif

#define button_x (PINA & 0x01) //button_x = PINA & 0x01 (PA0);
#define button_y (PINA & 0x02) //button_y = PINA & 0x02 (PA1);
#define button_pnd (PINA & 0x04) //button_pnd (#) = PINA & 0x04 (PA0 & PA1)
#define button_in (PINA & 0x80) //button_in (inside house) = PINA & 0x80 (PA7)

enum States {SM_START, SM_INIT, SM_Y, SM_PND} SM_State;

unsigned char tmpB = 0x00;//for PORTB

void tick(){
    //transitions
    switch(SM_State){
        case SM_START:
            SM_State = SM_INIT;
            break;
        case SM_INIT:
            if(button_pnd){ //if only '#' is pressed
                SM_State = SM_PND;
            }else{
                SM_State = SM_INIT;
            break;
        case SM_PND:
            if(button_y){ //if only 'Y' is pressed
                SM_State = SM_Y;
            }else{
                SM_State = SM_INIT; //if anything else, lock door (init)
            }
            break;
        case SM_Y:
            if(button_in){ //if inside button pressed lock the door (init)
                SM_State = SM_INIT;
            }
            break;
        default:
            break;
    }
    //state actions
    switch(SM_State){
        case SM_START:
            break;
        case SM_INIT:
            tmpB = 0x00; //door locked
            PORTB = tmpB;
            break;
        case SM_PND:
            break;
        case SM_Y:
            tmpB = 0x01; //door unlocked;
            PORTB = tmpB;
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
