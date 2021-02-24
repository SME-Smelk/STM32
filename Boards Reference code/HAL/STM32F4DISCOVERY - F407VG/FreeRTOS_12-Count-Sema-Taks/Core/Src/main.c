/**
  ******************************************************************************
  * @Project        : FreeRTOS_12-Count-Sema-Taks
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS Counting Semaphore test.
  * @Lib			: CMSIS, HAL. Third party: FreeRTOS and SEGGER SystemView
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*None
  * @RTOS
  * 	*task
  * 		vHandlerTask: Trigger the program and consume a semaphore.
  * 		vPeriodicTask: Pending the interrupt
  * 	*Interrupt
  * 		EXTI0_IRQ_CallBack: Put a 5 semaphores
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
char usr_msg[250]={0};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIO_Init(void);
void EXTI0_IRQ_CallBack(void);
/* Private user code ---------------------------------------------------------*/

/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;
TaskHandle_t xTaskHandler2=NULL;
//prototipes

/* The tasks to be created. */
static void vHandlerTask( void *pvParameters );
static void vPeriodicTask( void *pvParameters );


/*-----------------------------------------------------------*/

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
semaphore that is used to synchronize a task with an interrupt. */
xSemaphoreHandle xCountingSemaphore;

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

    /* Before a semaphore is used it must be explicitly created.  In this example
	a counting semaphore is created.  The semaphore is created to have a maximum
	count value of 10, and an initial count value of 0. */
    xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );

	/* Check the semaphore was created successfully. */
	if( xCountingSemaphore != NULL )
	{

		/* Start Message */
		UART2_write("Demo of usage of counting semaphore\r\n");

		/* Create the 'handler' task.  This is the task that will be synchronized
		with the interrupt.  The handler task is created with a high priority to
		ensure it runs immediately after the interrupt exits.  In this case a
		priority of 3 is chosen. */
		xTaskCreate( vHandlerTask, "Handler", 500, NULL, 1, NULL );

		/* Create the task that will periodically generate a software interrupt.
		This is created with a priority below the handler task to ensure it will
		get preempted each time the handler task exist the Blocked state. */
		xTaskCreate( vPeriodicTask, "Periodic", 500, NULL, 3, NULL );

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}

	UART2_write("Semaphore error r\n");

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
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);



/* EXTI interrupt init*/
HAL_NVIC_SetPriority(EXTI0_IRQn, 15, configMAX_SYSCALL_INTERRUPT_PRIORITY);
HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

static void vHandlerTask( void *pvParameters )
{
	/* As per most tasks, this task is implemented within an infinite loop. */
	while(1)
	{
		/* Use the semaphore to wait for the event.  The semaphore was created
		before the scheduler was started so before this task ran for the first
		time.  The task blocks indefinitely meaning this function call will only
		return once the semaphore has been successfully obtained - so there is no
		need to check the returned value. */
		xSemaphoreTake( xCountingSemaphore, portMAX_DELAY );

		/* To get here the event must have occurred.  Process the event (in this
		case we just print out a message). */

		UART2_write("Handler task - Processing event.\r\n");
	}
}

static void vPeriodicTask( void *pvParameters )
{
	/* As per most tasks, this task is implemented within an infinite loop. */
	while(1)
	{
		/* This task is just used to 'simulate' an interrupt.  This is done by
		periodically generating a software interrupt. */
		vTaskDelay( pdMS_TO_TICKS(500) );

		/* Generate the interrupt, printing a message both before hand and
		afterwards so the sequence of execution is evident from the output. */

    	UART2_write("Periodic task - Pending the interrupt.\r\n");

        //pend the interrupt
        NVIC_SetPendingIRQ(EXTI0_IRQn);

        UART2_write("Periodic task - Resuming.\r\n");

	}
}

void EXTI0_IRQ_CallBack(void)
{

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* 'Give' the semaphore multiple times.  The first will unblock the handler
	task, the following 'gives' are to demonstrate that the semaphore latches
	the events to allow the handler task to process them in turn without any
	events getting lost.  This simulates multiple interrupts being taken by the
	processor, even though in this case the events are simulated within a single
	interrupt occurrence.*/

	UART2_write("==>Button_Handler\r\n");

	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
	xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );

	/* Clear the software interrupt bit using the interrupt controllers  */


	/* Giving the semaphore may have unblocked a task - if it did and the
	unblocked task has a priority equal to or above the currently executing
	task then xHigherPriorityTaskWoken will have been set to pdTRUE and
	portEND_SWITCHING_ISR() will force a context switch to the newly unblocked
	higher priority task.

	NOTE: The syntax for forcing a context switch within an ISR varies between
	FreeRTOS ports.  The portEND_SWITCHING_ISR() macro is provided as part of
	the Cortex M3 port layer for this purpose.  taskYIELD() must never be called
	from an ISR! */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}
