/**
  ******************************************************************************
  * @Project        : RTC_02-DateTime_Standby
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-19-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : RTC Date and time each second, no lost by Standby.
  * 				  PROGRAM NOT TESTED: We need a LSE X3 External clock witch
  * 				  is not provided for DISCOVERY-DISC1. LSE is a backup Domain.
  * 				  HSE or LSI are off in LP Standby because they are not in
  * 				  backup domain.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		LSE
  * 	RTCCLK: 		32.768kHz
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PD12      	------> USER_LED
  * 		PA0      	------> USER_BUTTON
  * 	*RTC			------> Date Time
  * @note
  * 	-Without LSE (Clock BackUp Domain) the time and date will be lost.
  * 	-DISCOVERY-DISC1 STM32F407VG do not provide the provide X3 LSE it will
  * 	 provide for user.
  * 	-We use LSE 32.768kHz (Need be provided for user)
  * 	-Table of prescaler RTC:
  * 	RTCCLK			PREDIV_A 	PREDIV_S	ck_spre
  * 	LSE 32.768kHz	127			255			1Hz
  * 	LSI 32Khz		127			249			1Hz****
  * 	LSI 37Khz		124			295			1Hz
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void GPIO_Init(void);
void Error_handler(void);
static void UART2_Init(void);
static void RTC_Init(void);

/* Private user code ---------------------------------------------------------*/
RTC_TimeTypeDef RTC_TimeRead;
RTC_DateTypeDef RTC_DateRead;


void printmsg(char *format,...)
 {

	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);

 }

char* getDayofweek(uint8_t number)
{
	char *weekday[] = { "Monday", "TuesDay", "Wednesday","Thursday","Friday","Saturday","Sunday"};

	return weekday[number-1];
}

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

	GPIO_Init();
	UART2_Init();
	RTC_Init();

	/* User Code */
/*
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
*/
	/* Start Code */
	printmsg("This is RTC calendar Test program\r\n");

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_BKPSRAM_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		printmsg("Woke up from STANDBY\r\n");
		HAL_GPIO_EXTI_Callback(0);
	}



	//Enable the wakeup pin 1 in pwr_csr register
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	printmsg("Went to STANDBY mode\r\n");
	HAL_PWR_EnterSTANDBYMode();

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

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

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void RTC_Init(void)
{

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv =  127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x23;
  sTime.Minutes = 0x59;
  sTime.Seconds = 0x50;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_FEBRUARY;
  sDate.Date = 0x2;
  sDate.Year = 0x12;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable Calibration
  */
  if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ) != HAL_OK)
  {
    Error_Handler();
  }
}

static void GPIO_Init(void)
{

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef ledgpio = {0};
	GPIO_InitTypeDef buttongpio = {0};

	ledgpio.Pin = LED_GREEN_Pin;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LED_GREEN_Port,&ledgpio);

	buttongpio.Pin = BUTTON_Pin;
	buttongpio.Mode = GPIO_MODE_IT_FALLING;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_Port,&buttongpio);

	HAL_NVIC_SetPriority(EXTI0_IRQn,15,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 RTC_TimeTypeDef RTC_TimeRead;
	 RTC_DateTypeDef RTC_DateRead;

	 HAL_RTC_GetTime(&hrtc,&RTC_TimeRead,RTC_FORMAT_BIN);

	 HAL_RTC_GetDate(&hrtc,&RTC_DateRead,RTC_FORMAT_BIN);

	 printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
			 RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);
	 printmsg("Current Date is : %02d-%2d-%2d  <%s> \r\n",RTC_DateRead.Month,RTC_DateRead.Date,\
			 RTC_DateRead.Year,getDayofweek(RTC_DateRead.WeekDay));
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
