/*	Author: Mayur Ryali
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Locked, Wait, Release, Unlock} state;

unsigned short tempC; //store state numbers
unsigned char tempB;
unsigned char buttonX;
unsigned char buttonY;
unsigned char buttonPound;
unsigned char lockButton;

void Tick() {
	switch(state) {
		case Start:
			state = Locked;
			break;
		case Locked:
			if (lockButton) { //go to lock regardless if lock button is pressed
				state = Locked;
			}
			if (!buttonX && !buttonY && !lockButton && buttonPound) { //wait for button release if # is pressed
				state = Wait;
			}
			else { //stay locked otherwise
				state = Locked;
			}
			break;
		case Wait:
			if (!buttonX && !buttonY && !lockButton && !buttonPound) { //release if all buttons are not presses
				state = Release;
			}
			else if (!buttonX && !buttonY && !lockButton && buttonPound) { //wait for # button to be released
				state = Wait;
			}
			else { //lock if any other combination is pressed
				state = Locked;
			}
			break;
		case Release:
			if (buttonX && !buttonY && !lockButton && !buttonPound) { //unlock only if X-Y-X is pressed
				if (!buttonX && buttonY && !lockButton && !buttonPound) {
					if (buttonX && !buttonY && !lockButton && !buttonPound) {
						state = Unlock;
					}
				}
			}
			else if (!buttonX && !buttonY && !lockButton && !buttonPound) { //stay on release and wait for a button press
				state = Release;
			}
			else { //any other combo results in locked
				state = Locked;
			}
			break;
		case Unlock:
			if (!buttonX && !buttonY && lockButton && !buttonPound) { //lock button results in locked state
				state = Locked;
			}
			else if (!buttonX && !buttonY && !lockButton && buttonPound) { //new wait to allow for earlier code to lock
				state = Wait;
			}
			else { //stay unlocked
				state = Unlock;
			}
			break;
		/*
		case OtherWait:
			if (!buttonX && !buttonY && lockButton && !buttonPound) {//lock
				state = Locked;
			}
			else if (!buttonX && !buttonY && !lockButton && !buttonPound) {//consider earlier code
				state = OtherRelease;
			}
			else if (!buttonX && !buttonY && !lockButton && buttonPound) {
				state = OtherWait;
			}
			else {
				state = Unlock;
			}
			break;
		case OtherRelease:
			if (!buttonX && !buttonY && !lockButton && !buttonPound) {//wait for action
				state = OtherRelease;
			}
			else if ((!buttonX && buttonY && !lockButton && !buttonPound) || (!buttonX && !buttonY && lockButton && !buttonPound)) { //considers past input
				state = Locked;
			}
			else {
				state = Unlock;
			}	
			break;
		*/
		default:
			state = Start;
			break;
	}

	switch(state) { //state Start and default do not count
		case Start:
			break;
		case Locked:
			tempB = 0x00;
			tempC = 0x0000;
			break;
		case Wait:
			tempC = 0x0001;
			break;
		case Release:
			tempC = 0x0010;
			break;
		case Unlock:
			tempB = 0x01;
			tempC = 0x0011;
			break;
		/*
		case OtherWait:
			tempC = 0x0100;
			break;
		case OtherRelease:
			tempC = 0x0101;
			break;
		*/	
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output
	DDRC = 0xFF; PORTC = 0x00; //output

	state = Start;

    	while (1) {
		buttonX = PINA & 0x01;
		buttonY = PINA & 0x02;
		buttonPound = PINA & 0x04;
		lockButton = PINA & 0x80;
		Tick();
		PORTB = tempB;
		PORTC = tempC; //keep track of the state for debugging
    	}
    	return 1;
}
