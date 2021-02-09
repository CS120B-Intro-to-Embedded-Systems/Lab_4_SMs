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

#define button_inc (PINA & 0x01) //button_inc = PINA & 0x01 (PA0);
#define button_dec (PINA & 0x02) //button_dec = PINA & 0x02 (PA1);
#define button_reset (PINA & 0x03) //button_reset = PINA & 0x03 (PA0 & PA1)

enum States {SM_START, SM_INIT, SM_INC, SM_DEC, SM_RESET, SM_STOP} SM_State;

unsigned char tmpC = 0x00;//for PORTC

void tick(){
    //transitions
    switch(SM_State){
        case SM_START:
            SM_State = SM_INIT;
            break;
        case SM_INIT: //can go to any state
            if(button_inc){
                SM_State = SM_INC;
            }else if (button_dec){
                SM_State = SM_DEC;
            }else if (button_reset){
                SM_State = SM_RESET;
            }
            break;
        case SM_INC: //can increase, decrease, or reset. if C = 9 then go to STOP
            if(button_inc){
                SM_State = SM_INC;
            }else if (button_dec){
                SM_State = SM_DEC;
            }else if (button_reset){
                SM_State = SM_RESET;
            }
            if(tmpC == 0x09){
                SM_State = SM_STOP;
            }
            break;
        case SM_DEC: //can increase, decrease, or reset. if C = 0 then break.
            if(button_inc){
                SM_State = SM_INC;
            }else if (button_dec){
                SM_State = SM_DEC;
            }else if (button_reset){
                SM_State = SM_RESET;
            }
            if(tmpC == 0x00){
                break;
            }
            break;
        case SM_RESET: //resets to 0 so no decrementing
            if(button_inc){
                SM_State = SM_INC;
            }else if(button_reset){
                SM_State = SM_RESET;
            }
            break;
        case SM_STOP: //reached max (C = 0x09) so no incrementing
            if(button_dec){
                SM_State = SM_DEC;
            }else if(button_reset){
                SM_State = SM_RESET;
            }
            break;
    }
    //state actions
    switch(SM_State){
        case SM_START:
            break;
        case SM_INIT:
            tmpC = 0x07; //start with C = 0x07
            PORTC = tmpC;
            break;
        case SM_INC:
            tmpC = tmpC + 0x01;
            PORTC = tmpC;
            break;
        case SM_DEC:
            if(tmpC == 0x00){
                break;
            }
            tmpC = tmpC - 0x01;
            PORTC = tmpC;
            break;
        case SM_RESET:
            tmpC = 0x00; //reset C to 0x00
            PORTC = tmpC;
            break;
        case SM_STOP:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    /* Insert your solution below */
    SM_State = SM_START;
    while (1) {
        tick();
    }
    return 1;
}
