/*
 * API_debounce.h
 *
 *  Created on: 27 mar. 2023
 *      Author: Nicolas
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;


#define DEBOUNCE_TIME 40

void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas
void buttonPressed();			// debe invertir el estado del LED1
void buttonReleased();		// debe invertir el estado del LED3


#endif /* API_INC_API_DEBOUNCE_H_ */
