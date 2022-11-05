/*
 * 7seg-led.h
 *
 *  Created on: Nov 5, 2022
 *      Author: 84915
 */

#ifndef INC_7SEG_LED_H_
#define INC_7SEG_LED_H_

#include "main.h"
#include <stdlib.h>

void init_led7(const uint16_t* port, uint8_t* buffer);
void display_7SEG(unsigned int index);

#endif /* INC_7SEG_LED_H_ */
