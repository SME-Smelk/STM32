/**
  ******************************************************************************
  * @Project        : FreeRTOS_04-Led-ButtonIT
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS task programming with led and button via interrupt.
  * @Lib			: CMSIS, HAL. Third party: FreeRTOS and SEGGER SystemView
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * @RTOS
  * 	*task
  * 		1-task_Greenled_handler: Set the value to the led to blink.
  * 	*Exti NVIC Interrupt
  * 		Set de value of led.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t button_state = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIO_Init(void);
void asd(void);
/* Private user code ---------------------------------------------------------*/

EXTI_HandleTypeDef exti_button;

/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;

//prototipes
void task_Greenled_handler(void *params);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	//Habilito el contador de ciclo DWT
	DWT->CTRL |= (1<<0);

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the peripherals */
	GPIO_Init();
	USART2_UART_Init();


	/* Configure the system clock */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	/* Create a taskk */
	//Tarea 1
	xTaskCreate(task_Greenled_handler,"Led",configMINIMAL_STACK_SIZE,NULL,1,NULL);

	/* Start Message */
	//HAL_UART_Transmit(&huart2,(uint8_t*) "FreeRTOS Init\n", sizeof("FreeRTOS Init\n"), HAL_MAX_DELAY);
	UART2_write("FreeRTOS Init\n");

	/* Init scheduler */
	//osKernelInitialize();

	/* Start scheduler */
	//osKernelStart();
	vTaskStartScheduler();

	while (1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Function of gpio init.
  * @retval None
  */
void GPIO_Init(void){

GPIO_InitTypeDef GPIO_InitStruct = {0};

/* GPIO Ports Clock Enable */
__HAL_RCC_GPIOD_CLK_ENABLE();
__HAL_RCC_GPIOA_CLK_ENABLE();


/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);

/*Configure GPIO pins LED: LED_GREEN_PIN */
GPIO_InitStruct.Pin = LED_GREEN_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);

/*Configure GPIO pins LED: LED_GREEN_PIN */
/*Configure GPIO pins LED: LED_GREEN_PIN */
GPIO_InitStruct.Pin = BUTTON_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);



/* EXTI interrupt init*/
HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}


/**
  * @brief  Function of task1.
  * @retval None
  */
void task_Greenled_handler(void *params){
	while(1){


		if(button_state == BUTTON_PRESSED){
			//Turn on the led
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
			UART2_write("LEDGREEN: ON\r\n");
		}else{
			//Turn off the led
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
			UART2_write("LEDGREEN: OFF\r\n");
		}
		taskYIELD();
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	button_state^=1;

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}
