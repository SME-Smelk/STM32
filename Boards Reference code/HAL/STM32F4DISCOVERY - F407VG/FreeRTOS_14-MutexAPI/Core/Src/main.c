/**
  ******************************************************************************
  * @Project        : FreeRTOS_14-MutexAPI
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS Mutex API programming
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
  * 		prvPrintTask1:
  *
  * 		prvPrintTask2:
  *
  * 		prvNewPrintString:
  *
  * @Notes:
  * 	-We have two tasks with different priority and both write via UART. Task2
  * 	 have major priority than task1. Task2 we use the uart forever, agains that
  * 	 we used mutex to sync the functionality.
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
char usr_msg[250]={0};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Private user code ---------------------------------------------------------*/

/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandler1=NULL;
TaskHandle_t xTaskHandler2=NULL;

/* Dimensions the buffer into which messages destined for stdout are placed. */
#define mainMAX_MSG_LEN	( 80 )

//prototipes
/* The task to be created.  Two instances of this task are created. */
static void prvPrintTask1( void *pvParameters );
static void prvPrintTask2( void *pvParameters );
/* The function that uses a mutex to control access to standard out. */
static void prvNewPrintString( const portCHAR *pcString );

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
mutex type semaphore that is used to ensure mutual exclusive access to UART. */
xSemaphoreHandle xMutex;

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
	//SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start();

#ifdef USE_MUTEX
    /* Before a semaphore is used it must be explicitly created.  In this example
	a mutex type semaphore is created. */
    xMutex = xSemaphoreCreateMutex();
#endif

	/* The tasks are going to use a pseudo random delay, seed the random number
	generator. */
	srand( 567 );


#ifdef USE_MUTEX
	/* Only create the tasks if the semaphore was created successfully. */
	if( xMutex != NULL )
	{
#endif


		/* Create two instances of the tasks that attempt to write stdout.  The
		string they attempt to write is passed in as the task parameter.  The tasks
		are created at different priorities so some pre-emption will occur. */
		xTaskCreate( prvPrintTask1, "Print1", 240, "Task 1 ******************************************\r\n", 1, NULL );
		xTaskCreate( prvPrintTask2, "Print2", 240, "Task 2 ------------------------------------------\r\n", 2, NULL );

		/* Start message */
		UART2_write("Demo of mutual exclusion using Mutex APIs\r\n");

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
#ifdef USE_MUTEX
	}
#endif
    /* If all is well we will never reach here as the scheduler will now be
    running the tasks.  If we do reach here then it is likely that there was
    insufficient heap memory available for a resource to be created. */
	while(1);
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


static void prvPrintTask1( void *pvParameters )
{
char *pcStringToPrint;

	/* Two instances of this task are created so the string the task will send
	to prvNewPrintString() is passed in the task parameter.  Cast this to the
	required type. */
	pcStringToPrint = ( char * ) pvParameters;

	while(1)
	{
		/* Print out the string using the newly defined function. */
		prvNewPrintString( pcStringToPrint );

		/* Wait a pseudo random time.  Note that rand() is not necessarily
		re-entrant, but in this case it does not really matter as the code does
		not care what value is returned.  In a more secure application a version
		of rand() that is known to be re-entrant should be used - or calls to
		rand() should be protected using a critical section. */
	}
}

static void prvPrintTask2( void *pvParameters )
{
char *pcStringToPrint;

	/* Two instances of this task are created so the string the task will send
	to prvNewPrintString() is passed in the task parameter.  Cast this to the
	required type. */
	pcStringToPrint = ( char * ) pvParameters;

	while(1)
	{
		/* Print out the string using the newly defined function. */
		prvNewPrintString( pcStringToPrint );

		/* Wait a pseudo random time.  Note that rand() is not necessarily
		re-entrant, but in this case it does not really matter as the code does
		not care what value is returned.  In a more secure application a version
		of rand() that is known to be re-entrant should be used - or calls to
		rand() should be protected using a critical section. */
		//vTaskDelay( rand() & 0XF );
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

static void prvNewPrintString( const portCHAR *pcString )
{
static char cBuffer[ mainMAX_MSG_LEN ];

#ifdef USE_MUTEX
	/* The semaphore is created before the scheduler is started so already
	exists by the time this task executes.

	Attempt to take the semaphore, blocking indefinitely if the mutex is not
	available immediately.  The call to xSemaphoreTake() will only return when
	the semaphore has been successfully obtained so there is no need to check the
	return value.  If any other delay period was used then the code must check
	that xSemaphoreTake() returns pdTRUE before accessing the resource (in this
	case standard out. */
	xSemaphoreTake( xMutex, portMAX_DELAY );
	{
#endif
		/* The following line will only execute once the semaphore has been
		successfully obtained - so standard out can be accessed freely. */
		sprintf( cBuffer, "%s", pcString );
		UART2_write(cBuffer);
#ifdef USE_MUTEX
	}
	xSemaphoreGive( xMutex );
#endif
}
/*-----------------------------------------------------------*/


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
