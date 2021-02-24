/**
  ******************************************************************************
  * @Project        : FreeRTOS_10-Queue-Command
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS test a queue Command.
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
  * 		1-vTask1_menu_display: Send via UART2 the menu program and the queue.
  * 		2-vTask2_cmd_handling: handling the command receive.
  * 		3-vTask3_cmd_processing: Process the command receive.
  * 		4-vTask4_cmd_uart_write: Read the Queue and send the data via UART2.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
char msg[100];
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void RTC_Init(void);

uint8_t getCommandCode(uint8_t *buffer);
void getArguments(uint8_t *buffer);
void make_led_on(void);
void make_led_off(void);
void led_toggle(TimerHandle_t xTimer);
void led_toggle_start(uint32_t duration);
void led_toggle_stop(void);
void read_led_status(char *task_msg);
void read_rtc_info(char *task_msg);
void print_error_message(char *task_msg);
void exit_app(void);
/* Private user code ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;
TaskHandle_t xTaskHandler2=NULL;
TaskHandle_t xTaskHandler3=NULL;
TaskHandle_t xTaskHandler4=NULL;
//prototipes
void vTask1_menu_display(void *params);
void vTask2_cmd_handling(void *params);
void vTask3_cmd_processing(void *params);
void vTask4_cmd_uart_write(void *params);

/*Queue  */
//Escructura de comandos

//command queue
typedef struct APP_CMD{
	uint8_t COMMAND_NUM;
	uint8_t COMMAND_ARGS[10];
}APP_CMD_t;

QueueHandle_t command_queue=NULL;
QueueHandle_t uart_write_queue=NULL;

uint8_t command_buffer[20];
uint8_t command_len =0;
uint8_t data_byte;

//El menu
char menu[]={"\
\r\nLED_ON				----> 1 \
\r\nLED_OFF				----> 2 \
\r\nLED_TOGGLE			----> 3 \
\r\nLED_TOGGLE_OFF			----> 4 \
\r\nLED_READ_STATUS			----> 5 \
\r\nRTC_PRINT_DATETIME		----> 6 \
\r\nEXIT_APP			----> 0 \
\r\nType your option here : "};

//software timer handler
TimerHandle_t led_timer_handle = NULL;


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
	RTC_Init();
	USART2_UART_Init();


	/* Configure the system clock */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	/**/
	HAL_UART_Receive_IT(&huart2,&data_byte,sizeof(data_byte));

	/* Create Queues */
	//Queue command
	command_queue=xQueueCreate(10,sizeof(APP_CMD_t*));
	//Queue Write Uart
	uart_write_queue=xQueueCreate(10,sizeof(char*));

	if((command_queue != NULL) && (uart_write_queue != NULL))
	{
		UART2_write("Queue's Creation Success!\n");

		/* Create a taskk */
		//Tarea 1
		xTaskCreate(vTask1_menu_display,"task1-menu",500,NULL,1,&xTaskHandler1);
		//tarea 2
		xTaskCreate(vTask2_cmd_handling,"task2-cmd-handling",500,NULL,2,&xTaskHandler2);
		//tarea 3
		xTaskCreate(vTask3_cmd_processing,"task3-cmd-process",500,NULL,2,&xTaskHandler3);
		//tarea 4
		xTaskCreate(vTask4_cmd_uart_write,"task4-uart-write",500,NULL,2,&xTaskHandler4);

		/* Start Message */
		//HAL_UART_Transmit(&huart2,(uint8_t*) "FreeRTOS Init\n", sizeof("FreeRTOS Init\n"), HAL_MAX_DELAY);
		UART2_write("FreeRTOS Init: Queue Command Processing Demo\n");

		/* Init scheduler */
		//osKernelInitialize();

		/* Start scheduler */
		//osKernelStart();
		vTaskStartScheduler();

	}else
	{
		UART2_write("Queue Creation Failed\n");
	}

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
HAL_GPIO_WritePin(BUTTON_PORT, BUTTON_PIN, GPIO_PIN_RESET);


/*Configure GPIO pins LED: LED_GREEN_PIN */
GPIO_InitStruct.Pin = LED_GREEN_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);

/*Configure GPIO pins BUTTON */
GPIO_InitStruct.Pin = BUTTON_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

}

static void RTC_Init(void)
{
/*
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  // Initialize RTC Only

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  //Initialize RTC and set the Time and Date

  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
*/
}

void vTask1_menu_display(void *params)
{
	//Apunto puntero al string menu
	char *pData = menu;

	while(1)
	{
		xQueueSend(uart_write_queue,&pData,portMAX_DELAY);

		//Esperamos hasta que exista una notifiacion.
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

	}
}

void vTask2_cmd_handling(void *params)
{

	uint8_t command_code=0;
	APP_CMD_t *new_cmd;

	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//1. send command to queue
		new_cmd = (APP_CMD_t*) pvPortMalloc(sizeof(APP_CMD_t));

		taskENTER_CRITICAL();
		command_code = getCommandCode(command_buffer);
		new_cmd->COMMAND_NUM = command_code;
		getArguments(new_cmd->COMMAND_ARGS);
		taskEXIT_CRITICAL();

		//send the command to the command queue
		xQueueSend(command_queue,&new_cmd,portMAX_DELAY);
	}

}

void vTask3_cmd_processing(void *params)
{
	APP_CMD_t *new_cmd;
	char task_msg[50];

	uint32_t toggle_duration = pdMS_TO_TICKS(500);

	while(1)
	{
		xQueueReceive(command_queue,(void*)&new_cmd,portMAX_DELAY);

		if(new_cmd->COMMAND_NUM == LED_ON_COMMAND)
		{
			make_led_on();
		}
		else if(new_cmd->COMMAND_NUM == LED_OFF_COMMAND)
		{
			make_led_off();
		}
		else if(new_cmd->COMMAND_NUM == LED_TOGGLE_COMMAND)
		{
			led_toggle_start(toggle_duration);
		}
		else if(new_cmd->COMMAND_NUM == LED_TOGGLE_STOP_COMMAND)
		{
			led_toggle_stop();
		}
		else if(new_cmd->COMMAND_NUM == LED_READ_STATUS_COMMAND)
		{
			read_led_status(task_msg);
		}
		else if(new_cmd->COMMAND_NUM == RTC_READ_DATE_TIME_COMMAND )
		{
			read_rtc_info(task_msg);
		}
		else if(new_cmd->COMMAND_NUM == EXIT_APP )
		{
			exit_app();
		}else
		{
			print_error_message(task_msg);
		}

		//lets free the allocated memory for the new command
		vPortFree(new_cmd);

	}
}

void vTask4_cmd_uart_write(void *params)
{
	char *pData = NULL;

	while(1)
	{
		//Leemos la cola
		xQueueReceive(uart_write_queue, &pData, portMAX_DELAY);
		UART2_write(pData);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(HAL_UART_Receive_IT(&huart2,&data_byte,sizeof(data_byte)) == HAL_OK){

		command_buffer[command_len++] = data_byte;// & 0xFF;
		if(data_byte == '\r')
		{
			//Reinicio command lenght
			command_len=0;
			//El usuario termino de entregar el dato
			//Notificamos que el comando se ha recibido
			xTaskNotifyFromISR(xTaskHandler2,0,eNoAction,&xHigherPriorityTaskWoken);
			xTaskNotifyFromISR(xTaskHandler1,0,eNoAction,&xHigherPriorityTaskWoken);
		}
	}

	//Liberamos la CPU a la siguiente tarea.
	if(xHigherPriorityTaskWoken){
		taskYIELD();
	}
}

uint8_t getCommandCode(uint8_t *buffer)
{

	return buffer[0]-48;
}


void getArguments(uint8_t *buffer)
{


}

void print_error_message(char *task_msg)
{
	sprintf( task_msg,"\r\nInvalid command received\r\n");
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}

void make_led_on(void)
{
	HAL_GPIO_WritePin(LED_GREEN_PORT,LED_GREEN_PIN,SET);
}


void make_led_off(void)
{
	HAL_GPIO_WritePin(LED_GREEN_PORT,LED_GREEN_PIN,RESET);
}

void read_led_status(char *task_msg)
{
	sprintf(task_msg , "\r\nLED status is : %d\r\n", HAL_GPIO_ReadPin(LED_GREEN_PORT,LED_GREEN_PIN));
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}

void led_toggle_start(uint32_t duration)
{
	if(led_timer_handle==NULL){

		//1-Creamos timer por software
		led_timer_handle = xTimerCreate("LED_TIMER", duration, pdTRUE, NULL, led_toggle);
	}

	//2-Start
	xTimerStart(led_timer_handle,portMAX_DELAY);
}

void led_toggle(TimerHandle_t xTimer){
	HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
}

void led_toggle_stop(void)
{
	xTimerStop(led_timer_handle,portMAX_DELAY);
}

void read_rtc_info(char *task_msg)
{
	/*
	RTC_TimeTypeDef sTime1;
	RTC_DateTypeDef sDate1;

	//read time and date from RTC peripheral of the microcontroller
	HAL_RTC_GetTime(&hrtc, &sTime1, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate1, RTC_FORMAT_BCD);

	sprintf(task_msg,"\r\nTime: %02d:%02d:%02d \r\n Date : %02d-%2d-%2d \r\n",
			sTime1.Hours,sTime1.Minutes,sTime1.Hours,sDate1.Date,sDate1.Month,sDate1.Year);

	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
	*/

	sprintf(task_msg,"\r\nTime: 00:00:00 \r\n Date : 01-01-2020 \r\n");
	xQueueSend(uart_write_queue,&task_msg,portMAX_DELAY);
}

void exit_app(void)
{

taskENTER_CRITICAL();
UART2_write("Se eliminan tareas, se deshabilita IRQ UART y se activa Sleep Mode...");

//Borro las tareas
vTaskDelete(xTaskHandler1);
vTaskDelete(xTaskHandler2);
vTaskDelete(xTaskHandler3);
vTaskDelete(xTaskHandler4);

//Desactivo las interrupcion UART
/* USART2 interrupt Init */
HAL_NVIC_DisableIRQ(USART2_IRQn);
taskEXIT_CRITICAL();

}


void vApplicationIdleHook(void)
{
	//Envio a normal sleep mode
	__WFI();

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}
