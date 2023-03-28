/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_delay.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;



/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_TIME 40

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t ActualState;
delay_t DelayDebounce;

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();	// debe leer las entradas, resolver la lógica de transición de estados y actualizar las salidas
void buttonPressed();			// debe invertir el estado del LED1
void buttonReleased();		// debe invertir el estado del LED3

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();


  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);



  delayInit( &DelayDebounce, DEBOUNCE_TIME );
  debounceFSM_init();

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);


  while (1)
  {
	  debounceFSM_update();
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
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
			delayInit(&DelayDebounce,DEBOUNCE_TIME);
		}
		break;
	case BUTTON_FALLING:
		//tengo que chequear si se cumplio el tiempo antirebote
		if(delayRead(&DelayDebounce)){
			if(BSP_PB_GetState(BUTTON_USER)){ActualState = BUTTON_DOWN;	}//si se cumplio el tiempo vuelvo a analizar el estado para saber si realmente se produjo ese cambio o fue un rebote
			else{ActualState = BUTTON_UP;}//sino, vuelvo al estado anterior.
		}
		break;
	case BUTTON_DOWN:
		if(!BSP_PB_GetState(BUTTON_USER)){ //si se suelta y esta en bajo.... igual que el BUTTON UP pero alrevez
			ActualState = BUTTON_RAISING;
			delayInit(&DelayDebounce,DEBOUNCE_TIME);
		}
		break;
	case BUTTON_RAISING:
		if(delayRead(&DelayDebounce)){
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

void buttonPressed(){
	BSP_LED_Toggle(LED1);
}
void buttonReleased(){
	BSP_LED_Toggle(LED3);
}









#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
