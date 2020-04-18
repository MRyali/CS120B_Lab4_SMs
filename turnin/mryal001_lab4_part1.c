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

enum States {Start, Led2, Wait, Led1} state;

unsigned char tempB;
unsigned char button;

void Tick() {
	switch(state) {
		case Start:
			state = Led2;
			break;
		case Led2:
			if (button == 1) {
				state = Led2;
			}
			else {
				state = Wait;
			}
			break;
		case Wait:
			if (button == 1) {
				state = Led1;
			}
			else {
				state = Wait;
			}
			break;
		case Led1:
			if (button == 1) {
				state = Led1;
			}
			else {
				state = Led2; 
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			tempB = 0x01;
			break;
		case Led2:
			tempB = 0x02;
			break;
		case Wait:
			tempB = 0x02;
			break;
		case Led1:
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
		button = PINA & 0x01;
		Tick();
		PORTB = tempB;
    	}
    	return 1;
}
