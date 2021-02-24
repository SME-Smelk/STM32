/**
  ******************************************************************************
  * @Project        : FreeRTOS_05-Task-Notify
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS task programming a led and button via TaskNotify.
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
  * 		1-vled_task_Handler: Read de notify of task2 and put and trigger that
  * 		  in the led.
  * 		2-vbutton_task_Handler: This task create a notify that is receive
  * 		  for task1 vled_task_Handler for value of led.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

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
void vled_task_Handler(void *params);
void vbutton_task_Handler(void *params);
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
	SystemClock_Config();

	/* Configure the peripherals */
	GPIO_Init();
	USART2_UART_Init();

	/* Configure the system clock */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	/* Create a taskk */
	//Tarea 1
	xTaskCreate(vled_task_Handler,"LED",500,NULL,2,&xTaskHandler1);
	//tarea 2
	xTaskCreate(vbutton_task_Handler,"BUTTON",500,NULL,2,&xTaskHandler2);

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
void vled_task_Handler(void *params){

	uint32_t current_notification_value=0; //Para eIncrement

	while(1){

		//if(xTaskNotifyWait(0,0,NULL,portMAX_DELAY) == pdTRUE) //Para eNoAction
		if(xTaskNotifyWait(0,0,&current_notification_value,portMAX_DELAY) == pdTRUE)	//Para eIncrement
		{
			HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
			//UART2_write("Notificacion Recibida\r\n");	//Para eNoAction
			sprintf(msg,"Notificacion Recibida: Boton presionado: %ld \r\n",current_notification_value); //Para eIncrement
			UART2_write(msg); //Para eIncrement
		}

	}

}

/**
  * @brief  Function of task2.
  * @retval None
  */
void vbutton_task_Handler(void *params){

	while(1){

		if(HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN))
		{
			rtos_delay(100);
			//xTaskNotify(xTaskHandler1,0x0,eNoAction);	//Para eNoAction
			xTaskNotify(xTaskHandler1,0x0,eIncrement);	//Para eIncrement
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
