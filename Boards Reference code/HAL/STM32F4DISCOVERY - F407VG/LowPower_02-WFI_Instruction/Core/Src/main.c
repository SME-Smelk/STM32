/**
  ******************************************************************************
  * @Project        : LowPower_02-WFI_Instruction
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-18-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example the use WFI instruction for trigger the sleep mode.
  * 				  WFI (Waiting for interrupt), it stays in sleep mode until
  * 				  an interruption occurs (User Button).
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PA0      	------> USER_BUTTON
  * @Note
  * 	-Systick is desactivate CTRL = 0, because can wake up as it is an active
  * 	 interrupt of HAL and it can wake up the system of WFI.
  * 	-In WFI is more selective, its wake up depend of Actual priority and PRIMASK
  * 	 -WFE depend of the Actual priority, PRIMASK and SEVONPEND.
  * 	-Use the macro SLEEPMODE_BY_WFI in main.h to activate or desactivate WFI
  * 	for electrical consumption tests.
  * 	 	-First descomment macro and test consumption.
  * 	 		Normal mode: 6.8ma
  * 	 	-Second comment and see the consumption of the sleep mode.
  * 	 		Sleep Mode: 3.72mA
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void UART2_Init(void);
static void GPIO_AnalogConfig(void);

/* Private user code ---------------------------------------------------------*/
char some_data1[] = "Go to sleep...\r\n";
char some_data2[] = "WakeUp to send this data\r\n";
char some_data3[] = "The interrupt finish. I am in the loop.\r\n";

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	GPIO_AnalogConfig();
	GPIO_Init();
	UART2_Init();

	/* Desactivate Systick */
	 //SysTick->CTRL = 0;

	/* User Code */
	char msg[100];

	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK   : %ldHz\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1  : %ldHz\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2  : %ldHz\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	/* Start Code */
	while (1){
		HAL_UART_Transmit(&huart2,(uint8_t*)some_data1,(uint16_t)strlen((char*)some_data1),HAL_MAX_DELAY);
	#ifdef SLEEPMODE_BY_WFI
		/* Systick is not required so disabled it before going to sleep*/
		HAL_SuspendTick();
		/* going to sleep here */
		__WFI();
	#endif
		/* Continues from here when wakes up */
		/* Enable the Systick */
		HAL_ResumeTick();
		HAL_UART_Transmit(&huart2,(uint8_t*)some_data3,(uint16_t)strlen((char*)some_data3),HAL_MAX_DELAY);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Config MCO, signal output of SYSCLK in PC9 pin */
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_1);
}

static void UART2_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* Change GPIO to analog for low power. */
static void GPIO_AnalogConfig(void)
{
/*
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
*/
    __HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef gpio = {0};

	gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_4 |\
			   GPIO_PIN_5 | GPIO_PIN_6 |GPIO_PIN_7 |\
			   GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10 |\
			   GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 |\
			   GPIO_PIN_14 | GPIO_PIN_15;

	gpio.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA,&gpio);
	HAL_GPIO_Init(GPIOB,&gpio);
	HAL_GPIO_Init(GPIOC,&gpio);
	HAL_GPIO_Init(GPIOD,&gpio);

}

static void GPIO_Init(void)
{

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* User button */
	GPIO_InitTypeDef gpio = {0};

	gpio.Pin = USER_BUTTON_Pin;
	gpio.Mode = GPIO_MODE_IT_RISING;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_BUTTON_Port,&gpio);

	HAL_NVIC_SetPriority(EXTI0_IRQn,15,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* Button Interrupt */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if ( HAL_UART_Transmit(&huart2,(uint8_t*)some_data2,(uint16_t)strlen((char*)some_data2),HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
