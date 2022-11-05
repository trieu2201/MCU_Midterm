/*
 * input_reading.h
 *
 *  Created on: Nov 5, 2022
 *      Author: 84915
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_


#include "main.h"
#include <stdlib.h>

void init_button_reading(const uint16_t* port);
void button_reading(void);
unsigned char is_button_pressed(uint8_t index);

#endif /* INC_INPUT_READING_H_ */
