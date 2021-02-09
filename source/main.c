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

unsigned char tmpA = 0x00, tmpB = 0x00;//for PORTA and PORTB
unsigned char count = 0, passkey[4];

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
            }
            break;
        case SM_PND:
            if(count < 4){
                SM_State = SM_PND;
            }else{
                if(passkey[0] == 0x04 && passkey[1] == 0x01 && passkey[2] == 0x02 && passkey[3] == 0x01){
                    SM_State = SM_Y;
                }else{
                    SM_State = SM_INIT;
                }
            }
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
            passkey[count] = tmpA;
            count++;
            break;
        case SM_Y:
            if(tmpB == 0x00){
                tmpB = 0x01; //if locked before unlock
            }else if(tmpB == 0x01){
                tmpB = 0x00; //if unlocked before lock
            }
            PORTB = tmpB;
            for(count = 0; count < 4; count++){
                passkey[count] = 0;
            }
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
    SM_State = SM_START;
    while (1) {
        tick();
    }
    return 1;
}
