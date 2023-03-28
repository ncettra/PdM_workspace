/*
 * API_debounce.c
 *
 *  Created on: 27 mar. 2023
 *      Author: Nicolas
 */


#include "API_debounce.h"
#include "API_delay.h"
#include <stdint.h>
#include <stdbool.h>

static delay_t DebounceTimer; //al declararla como static impide el uso externo
static uint8_t ActualState;

static bool_t key;

bool_t readKey(void){

	if(key){
		key=false; //reinicio estado
		return true;
	}
	else{
		return false;
	}
}

void debounceFSM_init(){
	ActualState = BUTTON_UP;
}

void debounceFSM_update(){
	switch(ActualState){
	case BUTTON_UP:
		if(BSP_PB_GetState(BUTTON_USER)){ //si se presiona y esta en alto (UP)... --> paso a estado falling e inicializo el contador para volver a chequear en falling
			ActualState = BUTTON_FALLING;
			delayInit(&DebounceTimer,DEBOUNCE_TIME);
		}
		break;
	case BUTTON_FALLING:
		//tengo que chequear si se cumplio el tiempo antirebote
		if(delayRead(&DebounceTimer)){
			if(BSP_PB_GetState(BUTTON_USER)){ActualState = BUTTON_DOWN;	}//si se cumplio el tiempo vuelvo a analizar el estado para saber si realmente se produjo ese cambio o fue un rebote
			else{ActualState = BUTTON_UP;}//sino, vuelvo al estado anterior.
		}
		break;
	case BUTTON_DOWN:
		if(!BSP_PB_GetState(BUTTON_USER)){ //si se suelta y esta en bajo.... igual que el BUTTON UP pero alrevez
			ActualState = BUTTON_RAISING;
			delayInit(&DebounceTimer,DEBOUNCE_TIME);
		}
		break;
	case BUTTON_RAISING:
		if(delayRead(&DebounceTimer)){
			if(!BSP_PB_GetState(BUTTON_USER)){
				ActualState = BUTTON_UP;
			}
			else{
				ActualState = BUTTON_DOWN;
			}
		}
		break;
	}
}

void buttonPressed(){	key = true;}
void buttonReleased(){	key = false;}

