/*
 * API_delay.c
 *
 *  Created on: 18 mar. 2023
 *      Author: Nicolas
 */
#include "API_delay.h"


void delayInit( delay_t * delay, tick_t duration ) {
	if(delay == NULL) return;
	delay->duration = duration;
	delay->running = false;
}

bool_t delayRead( delay_t * delay ) {
	if(delay == NULL) return false;
	if(!delay->running) {
		delay->startTime = HAL_GetTick();
		delay->running = true;
	}
	else if(HAL_GetTick() - delay->startTime >= delay->duration) {
		delay->running = false;
		return true;
	}
	return false;
}

void delayWrite( delay_t * delay, tick_t duration ) {
	if(delay == NULL) return;
	delay->duration = duration;
}
