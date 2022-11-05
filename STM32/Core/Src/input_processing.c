/*
 * input_processing.c
 *
 *  Created on: Nov 5, 2022
 *      Author: 84915
 */

#include "input_processing.h"

const unsigned int NO_OF_SEC_TIMERS = 10;
const unsigned int NO_OF_MIL_TIMERS = 10;

#define NO_OF_BUTTONS   3

const uint16_t button_port[3] = {BUTTON_RESET_PORT, BUTTON_INC_PORT, BUTTON_DEC_PORT};

const uint16_t seg7_port[7] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6};
uint8_t buffer_led7[1] = {1};
uint8_t isButtonPressed[NO_OF_BUTTONS] = {0, 0, 0};
uint8_t isButtonPressed3s[NO_OF_BUTTONS] = {0, 0, 0};

uint8_t isButtonReleased[NO_OF_BUTTONS] = {0, 0, 0};

typedef enum {
	NORMAL,
	AUTO_INC,
	AUTO_DEC,
	DEC2ZERO

} MainState;
MainState mainstate = DEC2ZERO;

typedef enum {
	STATE_RELEASED,
	STATE_PRESSED,
} ButtonState;
ButtonState buttonState[3] = {STATE_RELEASED, STATE_RELEASED, STATE_RELEASED};

void init_fsm_input_processing(TIM_HandleTypeDef* htim) {
	init_timer(htim, NO_OF_SEC_TIMERS, NO_OF_MIL_TIMERS);
	init_led7(seg7_port, buffer_led7);
	init_button_reading(button_port);
	setSecTimer(4, 1);
	setMilTimer(0, 100);
}

void led_processing(){
	if(getMilFlag(0) == 1){
		setMilTimer(0, 500);
		display_7SEG(0);
	}

}

void blinking(void){
	if(getSecFlag(4) == 1){
		setSecTimer(4, 1);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
}

void fsm_for_pressed_pressed3s(void) {
	for(int i = 0; i < NO_OF_BUTTONS; i++) {
		switch(buttonState[i]) {
		case STATE_RELEASED:
			if(is_button_pressed(i)) {
				buttonState[i] = STATE_PRESSED;
				isButtonPressed[i] = 1;
				setSecTimer(i, 3);
			}
			break;
		case STATE_PRESSED:
			if(!is_button_pressed(i)) {
				isButtonReleased[i] = 1;
				buttonState[i] = STATE_RELEASED;
			}
			else if(getSecFlag(i)){
				isButtonPressed3s[i] = 1;

				resetSecFlag(i);
			}
			break;
		}
	}
}

void main_fsm(void){
	switch(mainstate){
	case NORMAL:

		if(isButtonPressed3s[1] == 1){
			isButtonPressed3s[1] = 0;
			buffer_led7[0] = (buffer_led7[0] == 9) ? 0 : buffer_led7[0] + 1;
			setSecTimer(3, 1);
			mainstate = AUTO_INC;
		}
		else if(isButtonPressed3s[2] == 1){
			isButtonPressed3s[2] = 0;
			buffer_led7[0] = (buffer_led7[0] == 0) ? 9 : buffer_led7[0] - 1;
			setSecTimer(3, 1);
			mainstate = AUTO_DEC;
		}
		else if(isButtonPressed[0] == 1){
			isButtonPressed[0] = 0;
			buffer_led7[0] = 0;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(isButtonPressed[1] == 1){
			isButtonPressed[1] = 0;
			buffer_led7[0] = (buffer_led7[0] == 9) ? 0 : buffer_led7[0] + 1;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(isButtonPressed[2] == 1){
			isButtonPressed[2] = 0;
			buffer_led7[0] = (buffer_led7[0] == 0) ? 9 : buffer_led7[0] - 1;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(getSecFlag(3) == 1){
			resetSecFlag(3);
			setSecTimer(3, 1);
			mainstate = DEC2ZERO;
		}
		break;
	case AUTO_INC:
		if (isButtonReleased[1] == 1){
			isButtonReleased[1] = 0;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(getSecFlag(3) == 1){
			setSecTimer(3, 1);
			buffer_led7[0] = (buffer_led7[0] == 9) ? 0 : buffer_led7[0] + 1;
		}
		break;
	case AUTO_DEC:
		if (isButtonReleased[2] == 1){
			isButtonReleased[2] = 0;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(getSecFlag(3) == 1){
			setSecTimer(3, 1);
			buffer_led7[0] = (buffer_led7[0] == 0) ? 9 : buffer_led7[0] - 1;
		}
		break;
	case DEC2ZERO:
		if(isButtonPressed[0] == 1){
			isButtonPressed[0] = 0;
			buffer_led7[0] = 0;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(isButtonPressed[1] == 1){
			isButtonPressed[1] = 0;
			buffer_led7[0] = (buffer_led7[0] == 9) ? 0 : buffer_led7[0] + 1;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(isButtonPressed[2] == 1){
			isButtonPressed[2] = 0;
			buffer_led7[0] = (buffer_led7[0] == 0) ? 9 : buffer_led7[0] - 1;
			setSecTimer(3, 10);
			mainstate = NORMAL;
		}
		else if(getSecFlag(3) == 1){
			setSecTimer(3, 1);
			buffer_led7[0] = (buffer_led7[0] == 0) ? buffer_led7[0] : buffer_led7[0] - 1;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
			mainstate = DEC2ZERO;
		}
		else{
			mainstate = DEC2ZERO;
		}
		break;
	default:
		mainstate = DEC2ZERO;
		break;
	}
}
