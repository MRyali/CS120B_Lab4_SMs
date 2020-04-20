/*	Author: Mayur Ryali
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Init, Led2, Wait, Led1} state;

unsigned char tempB;
unsigned char button;

void Tick() {
	switch(state) {
		case Start:
			state = Init;
			break;
		case Init:
			if (button == 1) { //check if button has been pressed in the initial state
				state = Led2;
			}
			else {
				state = Init;
			}
			break;
		case Led2:
			if (button == 1) { //check if button is pushed
				state = Led2;
			}
			else { //otherwise wait for action
				state = Wait;
			}
			break;
		case Wait:
			if (button == 1) { //wait for another button press to transition to new Led
				state = Led1;
			}
			else { //continue waiting for action
				state = Wait;
			}
			break;
		case Led1:
			if (button == 1) { //check if button is pushed
				state = Led1;
			}
			else { //go back to start to reset SM
				state = Init; 
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start: //default starts with PB0 set to 1
			tempB = 0x01;
			break;
		case Init:
			tempB = 0x01;
			break;
		case Led2: //PB1 set to 1
			tempB = 0x02;
			break;
		case Wait:
			tempB = 0x02;
			break;
		case Led1: //PB0 set to 1
			tempB = 0x01;
			break;
		default:
			tempB = 0x01;
			break;
	}
}



int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output

	state = Start;
	
    	while (1) {
		button = PINA & 0x01; //start with PB0 Led on
		Tick();
		PORTB = tempB;
    	}
    	return 1;
}
