/*
 * API_delay.h
 *
 *  Created on: 18 mar. 2023
 *      Author: Nicolas
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_


#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>

typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile?
typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile?

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );


#endif /* API_INC_API_DELAY_H_ */
