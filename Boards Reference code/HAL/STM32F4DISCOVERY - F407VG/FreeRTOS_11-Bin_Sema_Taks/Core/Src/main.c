/**
  ******************************************************************************
  * @Project        : FreeRTOS_11-Bin_Sema_Taks
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-23-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : FreeRTOS test Binary Semaphore Tasks.
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
  * 		1-vManagerTask: Send a queue. Send a semaphore.
  * 		2-vEmployeeTask: Receive a queue. Take a semaphore.
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

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/
char usr_msg[250]={0};
/* FreeRTOS ------------------------------------------------------------------*/

/* Tasks ----------------------------------*/
TaskHandle_t xTaskHandleM=NULL;
TaskHandle_t xTaskHandleE=NULL;
/* The tasks to be created. */
static void vManagerTask( void *pvParameters );
static void vEmployeeTask( void *pvParameters );

/* Semaphore-------------------------------*/

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
semaphore that is used to synchronize both manager and employee task */
xSemaphoreHandle xWork;

/* this is the queue which manager uses to put the work ticket id */
xQueueHandle xWorkQueue;

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

    /* Before a semaphore is used it must be explicitly created.
     * In this example a binary semaphore is created . */
    vSemaphoreCreateBinary( xWork );

	/* The queue is created to hold a maximum of 1 Element. */
    xWorkQueue = xQueueCreate( 1, sizeof( unsigned int ) );

    /* Check the semaphore and queue was created successfully. */
    if( (xWork != NULL) && (xWorkQueue != NULL) )
    {

    	/* Start Message */
    	UART2_write("Demo of Binary semaphore usage between 2 Tasks \r\n");

		/* Create the 'Manager' task.  This is the task that will be synchronized with the Employee task.  The Manager task is created with a high priority  */
        xTaskCreate( vManagerTask, "Manager", 500, NULL, 3, NULL );

        /* Create a employee task with less priority than manager */
        xTaskCreate( vEmployeeTask, "Employee", 500, NULL, 1, NULL );

        /* Start the scheduler so the created tasks start executing. */
        vTaskStartScheduler();
    }

    UART2_write("Queue/Sema create failed.. \r\n");


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

void vManagerTask( void *pvParameters )
{

	 unsigned int xWorkTicketId;
	 portBASE_TYPE xStatus;
	 uint8_t cont = 0;
   /* The semaphore is created in the 'empty' state, meaning the semaphore must
	 first be given using the xSemaphoreGive() API function before it
	 can subsequently be taken (obtained) */
   xSemaphoreGive( xWork);
   for( ;; )
   {
       /* get a work ticket id(some random number) */
	   cont++;
	   xWorkTicketId = cont;
	   if(cont >= 255) cont=0;


		/* Sends work ticket id to the work queue */
		xStatus = xQueueSend( xWorkQueue, &xWorkTicketId , portMAX_DELAY ); //Post an item on back of the queue

		if( xStatus != pdPASS )
		{
		    UART2_write("Could not send to the queue.\r\n");
		}else
		{
			/* Manager notifying the employee by "Giving" semaphore */
			xSemaphoreGive( xWork);
			/* after assigning the work , just yield the processor because nothing to do */
			taskYIELD();
		}
   }
}
/*-----------------------------------------------------------*/

void EmployeeDoWork(unsigned char TicketId)
{
	/* implement the work according to TickedID */
	sprintf(usr_msg,"Employee task : Working on Ticked id : %d\r\n",TicketId);
	UART2_write(usr_msg);
	vTaskDelay(TicketId);
}

static void vEmployeeTask( void *pvParameters )
{

	unsigned char xWorkTicketId;
	portBASE_TYPE xStatus;
   /* As per most tasks, this task is implemented within an infinite loop. */
   for( ;; )
   {
		/* First Employee tries to take the semaphore, if it is available that means there is a task assigned by manager, otherwise employee task will be blocked */
		xSemaphoreTake( xWork, 0 );

		/* get the ticket id from the work queue */
		xStatus = xQueueReceive( xWorkQueue, &xWorkTicketId, 0 );

		if( xStatus == pdPASS )
		{
		  /* employee may decode the xWorkTicketId in this function to do the work*/
			EmployeeDoWork(xWorkTicketId);
		}
		else
		{
			/* We did not receive anything from the queue.  This must be an error as this task should only run when the manager assigns at least one work. */
			UART2_write("Employee task : Queue is empty , nothing to do.\r\n");

		}
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

