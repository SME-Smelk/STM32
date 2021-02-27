/**
  ******************************************************************************
  * @Project        : 30-RTC_Alarm_Standby_WakeUp
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-20-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : RTC used like a wakeup in StandBy Mode.
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
  * 	*RTC
  * 		Alarm A		------> ALARM_MINUTES 0
  * 							ALARM_SECONDS 3
  * 							00:03
  * @note
  * 	-STM32 RTC emdebs two alarms alarm A and alarm B. An alarm can be generated
  * 	 at a given time or date programmed by the user.
  * 	-Without LSE (Clock BackUp Domain) the time and date will be lost.
  * 	-DISCOVERY-DISC1 STM32F407VG do not provide the provide X3 LSE it will
  * 	 provide for user.
  * 	-Use macros ALARM_MINUTES and ALARM_SECONDS to represent the alarm at 00:10,
  * 	 its means, the button should be pressed before that time. The start time is
  * 	 59:50.
  * 	-We use LSI 32Khz
  * 	-Table of prescaler RTC:
  * 		RTCCLK			PREDIV_A 	PREDIV_S	ck_spre
  * 		LSE 32.768kHz	127			255			1Hz
  * 		LSI 32Khz		127			249			1Hz****
  * 		LSI 37Khz		124			295			1Hz
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
void RTC_CalendarConfig(void);
void RTC_AlarmConfig(void);

/* Private user code ---------------------------------------------------------*/
RTC_TimeTypeDef RTC_TimeRead;
RTC_DateTypeDef RTC_DateRead;

uint8_t flag_alarm = 0;

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


	//Enable clock for PWR Controller block
	__HAL_RCC_PWR_CLK_ENABLE();

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);

		printmsg("Woke up from standby mode\r\n");

		 RTC_TimeTypeDef  RTC_TimeRead;
		 RTC_DateTypeDef RTC_DateRead;

		HAL_RTC_GetTime(&hrtc,&RTC_TimeRead,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&RTC_DateRead,RTC_FORMAT_BIN);

		printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
		RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);

		HAL_GPIO_WritePin(LED_GREEN_Port,LED_GREEN_Pin,GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(LED_GREEN_Port,LED_GREEN_Pin,GPIO_PIN_RESET);

	}

	printmsg("This is RTC Alarm Test program\r\n");


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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
  hrtc.Init.SynchPrediv = 249;
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

	ledgpio.Pin = LED_GREEN_Pin;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LED_GREEN_Port,&ledgpio);

	ledgpio.Pin = BUTTON_Pin;
	ledgpio.Mode = GPIO_MODE_IT_FALLING;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_Port,&ledgpio);

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

	 //RTC_CalendarConfig();

	 HAL_RTC_GetTime(&hrtc,&RTC_TimeRead,RTC_FORMAT_BIN);

	 HAL_RTC_GetDate(&hrtc,&RTC_DateRead,RTC_FORMAT_BIN);

	 printmsg("-----------------\r\n");
	 printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
			 RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);
	 printmsg("Current Date is : %02d-%2d-%2d  <%s> \r\n",RTC_DateRead.Month,RTC_DateRead.Date,\
			 RTC_DateRead.Year,getDayofweek(RTC_DateRead.WeekDay));

	 //make sure that WUF and RTC alarm A flag are cleared
	 __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
	 __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	 RTC_AlarmConfig();

	 printmsg("Went to STANDBY mode\r\n");

	 //Go to standby mode
	 HAL_PWR_EnterSTANDBYMode();

}


void  RTC_AlarmConfig(void)
{

	 RTC_AlarmTypeDef AlarmA_Set;

	 memset(&AlarmA_Set,0,sizeof(AlarmA_Set));

	 HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_A);

	 /* Alarm for hour o second*/

	 AlarmA_Set.Alarm = RTC_ALARM_A;
	 AlarmA_Set.AlarmTime.Minutes = ALARM_MINUTES;
	 AlarmA_Set.AlarmTime.Seconds = ALARM_SECONDS;
	 AlarmA_Set.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY ;
	 AlarmA_Set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	 /* Alarm for week days*/
/*
	 AlarmA_Set.Alarm = RTC_ALARM_A;
	 AlarmA_Set.AlarmTime.Hours = 00;
	 AlarmA_Set.AlarmTime.Minutes = 00;
	 AlarmA_Set.AlarmTime.Seconds = 05;
	 AlarmA_Set.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	 AlarmA_Set.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	 AlarmA_Set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
*/
	 /* Alarm every specific day
	  * alarm sound at the day "day" at 1:00:00.
	  * */
/*
	 AlarmA_Set.Alarm = RTC_ALARM_A;
	 AlarmA_Set.AlarmTime.Hours = 1;
	 AlarmA_Set.AlarmTime.Minutes = 0;
	 AlarmA_Set.AlarmTime.Seconds = 5;
	 AlarmA_Set.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	 AlarmA_Set.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	 AlarmA_Set.AlarmDateWeekDay = RTC_WEEKDAY_THURSDAY;
	 AlarmA_Set.AlarmMask = RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS;
	 AlarmA_Set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
*/
	 if ( HAL_RTC_SetAlarm_IT(&hrtc, &AlarmA_Set, RTC_FORMAT_BIN) != HAL_OK)
	 {
		 Error_Handler();
	 }

	 printmsg("Alarm Set Successful\r\n");
	 printmsg("-----------------\r\n");

}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{

	printmsg("-----------------\r\n");
	printmsg("Alarm Triggered \r\n");

	RTC_TimeTypeDef RTC_TimeRead;

	HAL_RTC_GetTime(hrtc,&RTC_TimeRead,RTC_FORMAT_BIN);

	printmsg("Current Time is : %02d:%02d:%02d\r\n",RTC_TimeRead.Hours,\
	RTC_TimeRead.Minutes,RTC_TimeRead.Seconds);
	printmsg("-----------------\r\n");
	flag_alarm = 1;

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
