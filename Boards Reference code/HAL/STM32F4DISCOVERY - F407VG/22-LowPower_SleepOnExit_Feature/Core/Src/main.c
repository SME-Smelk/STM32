/**
  ******************************************************************************
  * @Project        : 22-LowPower_SleepOnExit_Feature
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 18-02-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Use the feature SleepOnExit. Sleep all the time that we are
  * 				  not in an ISR. WakeUp in a ISR TIMER6 10ms, at the exit it
  * 				  fall sleep.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*TIMER6 IT 10ms
  * 	*GPIO
  * 		PD12      	------> LED_GREEN
  * @note
  * -To fall down sleep mode with feature SleepOnExit try HAL_PWR_EnableSleepOnExit();
  * -Probe the current consumption with JP1 ldd. It allows the consumption to be
  *  measured by removing the jumper and connecting an ammeter.
  * -To probe the current consumption of the system try:
  *
  * 	-comment the macro SLEEP_ON_EXIT_SLEEPMODE  API to Normal mode and current
  * 	 without sleep mode. Probe the current consumption. Ej: 7.78mA
  *
  * 	-Descomment the macro SLEEP_ON_EXIT_SLEEPMODE in main.h to enter in sleep mode
  * 	 all the time that we are not in an ISR.Probe the current consumption. Ej: 5.22mA
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer6;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void GPIO_AnalogConfig(void);
static void UART2_Init(void);
static void TIMER6_Init(void);

/* Private user code ---------------------------------------------------------*/
char some_data[] = "We are testing SLEEPONEXIT feature\r\n";

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */	HAL_Init();
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	GPIO_AnalogConfig();
	GPIO_Init();
	UART2_Init();
	TIMER6_Init();

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

	/* System Control Register (SCR)
	 * bit 1: SLEEPDEEP
	 * Manual: SCB->SCR |= ( 1 << 1);
	 * */

	/*Enable SleepOnExit: sleep all the time that we are not in an ISR
	 * WakeUp in a ISR, in the exit sleep.
	 * */
#ifdef SLEEP_ON_EXIT_SLEEPMODE
	HAL_PWR_EnableSleepOnExit();
#endif
	/* lets start with fresh Status register of Timer to avoid any spurious interrupts */
    TIM6->SR = 0;

	//Lets start the timer in interrupt mode
	HAL_TIM_Base_Start_IT(&htimer6);

	while (1);
	return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
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

    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* User button */
	GPIO_InitTypeDef gpio = {0};

	gpio.Pin = GPIO_PIN_12;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD,&gpio);

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


static void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 1999;
	htimer6.Init.Period = 79;
	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
		Error_Handler();
	}

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if ( HAL_UART_Transmit(&huart2,(uint8_t*)some_data,(uint16_t)strlen((char*)some_data),HAL_MAX_DELAY) != HAL_OK)
	 {
		 Error_Handler();
	 }
	 HAL_GPIO_WritePin(LED_GREEN_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
