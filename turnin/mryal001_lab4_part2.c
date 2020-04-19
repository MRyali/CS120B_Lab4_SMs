/*	Author: Mayur Ryali
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Init, Inc, Dec, Zero, Wait} state;

unsigned char tempC;
unsigned char button0;
unsigned char button1;

void Tick() {
	switch(state) {
		case Start:
			state = Init;
			break;
		case Init:
			if (button0 && !button1) {
				state = Inc;
			}
			else if (!button0 && button1) {
				state = Dec;
			}
			else if (button0 && button1) {
				state = Zero;
			}
			else {
				state = Init;
			}
			break;
		case Inc:
			if (button0 && button1) {
				state = Zero;
			}
			else {
				state = Wait;
			}
			break;
		case Dec:
			if (button0 && button1) {
				state = Zero;
			}
			else {
				state = Wait;
			}
			break;
		case Zero: 
			if (button0 && !button1) {
				state = Inc;
			}		   
			else if (!button0 && button1) {
				state = Dec;
			}
			else if (!button0 && !button1) {
				state = Init;
			}
			else {
				state = Zero;
			}
			break;
		case Wait:
			if (!button0 && !button1) {
				state = Init;
			}
			else if (button0 && button1) {
				state = Zero;
			}
			else {
				state = Wait;
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			break;
		case Init:
			break;
		case Inc:
			if (tempC < 9) {
				tempC = tempC + 1;
			}
			break;
		case Dec:
			if (tempC > 0) {
				tempC = tempC - 1;
			}
			break;
		case Zero:
			tempC = 0;
			break;
		case Wait:
			break;
		default:
			break;
	}
}



int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRC = 0xFF; PORTC = 0x00; //output

	state = Start;
	tempC = 0x07;

    	while (1) {
		button0 = PINA & 0x01;
		button1 = PINA & 0x02;
		Tick();
		PORTC = tempC;
    	}
    	return 1;
}
