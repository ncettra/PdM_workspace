/*
 * API_uart.c
 *
 *  Created on: 4 abr. 2023
 *      Author: Nicolas
 */

#include "API_uart.h"
#include <stdbool.h>
#include <stdint.h>

UART_HandleTypeDef UartHandle;

bool uartInit(){
  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
	return false;
  }
  return true;
};


//envia el string completo
void uartSendString(uint8_t * pstring){
	//sizeof o strlen ? ? cual me marca el \0??.... o en caso de memoria dinamica es lo mismo?....
	HAL_UART_Transmit(&UartHandle,pstring,sizeof(pstring),USART_TIMEOUT);
	/*while(*pstring){
		HAL_UART_Transmit(&UartHandle,pstring++,1,TIMEOUT);
	}*/
}
//envia n caracteres
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Transmit(&UartHandle,pstring,size,USART_TIMEOUT);
}
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Receive(&UartHandle,pstring,size,USART_TIMEOUT);
}
