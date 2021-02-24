/**
  ******************************************************************************
  * @Project        : FreeRTOS_06-TaskDelete
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS task delete programming with led and button via.
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
  * 		1-vTask1_Handler: Toggle led green.
  * 		2-vTask2_Handler: Toggle led green, if button is pressed the toggle led
  * 		 of task2 is off, the task is deleted.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Private user code ---------------------------------------------------------*/
char msg[100];
/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;
TaskHandle_t xTaskHandler2=NULL;
//prototipes
void vTask1_Handler(void *params);
void vTask2_Handler(void *params);
void rtos_delay(uint32_t delay_in_ms);
void GPIO_Init(void);

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
	//SystemClock_Config();

	/* Configure the peripherals */
	GPIO_Init();
	USART2_UART_Init();

	/* Configure the system clock */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	/* Create a taskk */
	//Tarea 1
	xTaskCreate(vTask1_Handler,"task1",configMINIMAL_STACK_SIZE,NULL,1,&xTaskHandler1);
	//tarea 2
	xTaskCreate(vTask2_Handler,"task2",configMINIMAL_STACK_SIZE,NULL,2,&xTaskHandler2);

	/* Start Message */
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
GPIO_InitStruct.Pin = BUTTON_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

}
/**
  * @brief  Function of task1.
  * @retval None
  */
void vTask1_Handler(void *params)
{

	sprintf(msg,"Task-1 is running\r\n");
	UART2_write(msg);

	while(1)
	{
		rtos_delay(200);
		//vTaskDelay(200);
		HAL_GPIO_TogglePin(LED_GREEN_PORT,LED_GREEN_PIN);
	}
}

/**
  * @brief  Function of task2.
  * @retval None
  */
void vTask2_Handler(void *params)
{

	sprintf(msg,"Task-2 is running\r\n");
	UART2_write(msg);

	while(1)
	{
		if( HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN))
		{
			//button is pressed on the nucleo board
			//Task2 deletes itself
			sprintf(msg,"Task2 is getting deleted\r\n");
			UART2_write(msg);
			vTaskDelete(NULL);
		}else
		{
			//button is not pressed on the nucleo board
			//lets toggle the led for every 1 sec
             rtos_delay(1000);
             HAL_GPIO_TogglePin(LED_GREEN_PORT,LED_GREEN_PIN);
		}

	}
}

void rtos_delay(uint32_t delay_in_ms){

	//Current tick
	uint32_t current_tick_count = xTaskGetTickCount();

	//MiliSeconds to Ticks
	uint32_t delay_in_ticks= (delay_in_ms * configTICK_RATE_HZ) / 1000;

	//delay
	while(xTaskGetTickCount()< (current_tick_count + delay_in_ticks) );

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}
