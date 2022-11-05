/*
 * 7seg-led.c
 *
 *  Created on: Nov 5, 2022
 *      Author: 84915
 */

#include "7seg-led.h"

// control Anode of LED7
#define ENABLE_LED  	GPIO_PIN_RESET
#define DISABLE_LED  	GPIO_PIN_SET
// control single segment
#define SET_SEG  		GPIO_PIN_RESET
#define RESET_SEG  		GPIO_PIN_SET

// Hex code for displaying a single led7
static uint8_t seg7Hex[] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04, 0xff}; // last one is for clear



// Array of GPIO port for each segment
static const uint16_t* seg7Port;
// Array of GPIO port for each Anode

// pointer to the user buffer
static uint8_t* seg7Val;
// represent for all segment pin
static uint16_t allseg;

void init_led7(const uint16_t* port, uint8_t* buffer) {

	seg7Port = port;
	seg7Val = buffer;
	allseg = seg7Port[0] | seg7Port[1] | seg7Port[2] | seg7Port[3] | seg7Port[4] | seg7Port[5] | seg7Port[6];
}


void display_7SEG(unsigned int index) {
	HAL_GPIO_WritePin(SEG_PORT, allseg, RESET_SEG);

	uint8_t hexcode = seg7Hex[seg7Val[index]];
	for(int i = 0; i < 7; i++) {
		if((hexcode & (0x40 >> i)) == 0)
			HAL_GPIO_WritePin(SEG_PORT, seg7Port[i], SET_SEG);
	}
}
