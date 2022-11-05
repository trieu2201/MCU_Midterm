/*
 * input_processing.h
 *
 *  Created on: Nov 5, 2022
 *      Author: 84915
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"
#include <stdlib.h>

#include "timer.h"
#include "7seg-led.h"
#include "input_reading.h"

typedef enum {
	BUTTON_FUNC
} ButtonArrayAccess;

void init_fsm_input_processing(TIM_HandleTypeDef* htim);
void led_processing();
void blinking(void);
void fsm_for_pressed_pressed3s(void);
void main_fsm(void);

#endif /* INC_INPUT_PROCESSING_H_ */
