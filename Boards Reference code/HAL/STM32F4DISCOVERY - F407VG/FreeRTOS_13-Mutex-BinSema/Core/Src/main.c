/**
  ******************************************************************************
  * @Project        : FreeRTOS_13-Mutex-BinSema
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS Binary Semaphore like a Mutual exclusion.
  * @Lib			: CMSIS, HAL. Third party: FreeRTOS and SEGGER SystemView
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*None
  * @RTOS
  * 	*tasks
  * 		task1: Take a semaphore, finish the operation in the critical region
  * 			   and Give a semaphore
  * 		task2: Take a semaphore, finish the operation in the critical region
  * 			   and Give a semaphore
  *
  * @note:
  * 	-This program use a Binary Semaphore like a Mutual exclusion. Each task
  * 	  only execute the critical region one time at once.
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
/* Private user code ---------------------------------------------------------*/

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;
TaskHandle_t xTaskHandler2=NULL;

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );


//binary semaphore handle
xSemaphoreHandle xBinarySemaphore;

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
	USART2_UART_Init();

	/* Configure the system clock */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	//Creating a binary semaphore
	vSemaphoreCreateBinary(xBinarySemaphore);

	 if(xBinarySemaphore != NULL)
	 {

			/* Start Message */
			UART2_write("Demo of Mutual exclusion using binary semaphore\r\n");

			/* Create one of the two tasks. */
			xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
							"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
							500,		/* Stack depth in words. */
							NULL,		/* We are not using the task parameter. */
							1,			/* This task will run at priority 1. */
							NULL );		/* We are not using the task handle. */

			/* Create the other task in exactly the same way. */
			xTaskCreate( vTask2, "Task 2", 500, NULL, 1, NULL );

			//makes sema available for the first time
			xSemaphoreGive(xBinarySemaphore);

			/* Start the scheduler so our tasks start executing. */
			vTaskStartScheduler();
	 }else
	 {
		 UART2_write("binary semaphore creation failed\r\n");

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

void vTask1( void *pvParameters )
{
	const char *pcTaskName = "Task 1 is running\r\n";

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		//before printing , lets own the semaphore or take the semaphore */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );

		/* Print out the name of this task. */
		sprintf( usr_msg,"%s",pcTaskName);
		UART2_write(usr_msg);

		//give the semaphore here. give operation increases the bin sema value back to 1
		xSemaphoreGive(xBinarySemaphore);

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( pdMS_TO_TICKS(500) );
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{

	const char *pcTaskName = "Task 2 is running\r\n";

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		//before printing , lets own the semaphore or take the semaphore */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );

		/* Print out the name of this task. */
		sprintf( usr_msg,"%s",pcTaskName);
		UART2_write(usr_msg);

		//give the semaphore here. give operation increases the bin sema value back to 1
		xSemaphoreGive(xBinarySemaphore);

		/*Now this task will be blocked for 500ticks */
		vTaskDelay( pdMS_TO_TICKS(500));
	}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}
