/**
  ******************************************************************************
  * @Project        : 31-RTC_Demo_APIs
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-20-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : RTC Colection of APIs
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		LSI
  * 	RTCCLK: 		32khz
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PD12      	------> USER_LED
  * 		PD13      	------> USER_LED
  * 		PD14      	------> USER_LED
  * 		PD15      	------> USER_LED
  * 		PA0			------> USER_BUTTON
  * 	*RTC
  * @note
  * 	-We use LSI 32Khz
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
void Error_Handler(void);
static void UART2_Init(void);
static void RTC_Init(void);
void RTC_Init(void);
void RTC_ConfigureTimeDate(void);
uint8_t PrintUserMenu(void);
uint8_t is_valid_time_set(void);
uint8_t Calendar_Demo(uint8_t arg);
uint8_t Alarm_Demo(uint8_t arg);
uint8_t LowPowerDemo(uint8_t arg);
uint8_t  TimeStamp_Demo(uint8_t arg);
uint8_t CalendarDemoMenuPrint(void);
uint8_t LowPowerDemoMenuPrint(void);
uint8_t Alarm_DemoPrint(void);
uint16_t getYear(uint8_t *year);
void RTC_configureUserGivenTime(uint8_t seconds,uint8_t minutes,uint8_t hour,uint8_t AMPM, uint8_t format);
void RTC_configureUserGivenDate(uint16_t year,uint8_t month,uint8_t date);
void RTC_DisplayCurrentTime(void);
void RTC_DisplayCurrentDate(void);

/* Private user code ---------------------------------------------------------*/

uint8_t user_input[30];
uint8_t data_user = '\0';
UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

/* prints formatted string to console over UART */
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

	GPIO_InitTypeDef ledgpio = {0};

	ledgpio.Pin = LED_GREEN_Pin;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LED_GREEN_Port,&ledgpio);

}

void RTC_ConfigureTimeDate(void)
{
	  RTC_DateTypeDef  sdatestructure;
	  RTC_TimeTypeDef  stimestructure;

	  /*##-1- Configure the Date #################################################*/
	  /* Set Date: Tuesday February 18th 2014 */
	  sdatestructure.Year = 0x18;
	  sdatestructure.Month = RTC_MONTH_AUGUST;
	  sdatestructure.Date = 0x23;
	  sdatestructure.WeekDay = RTC_WEEKDAY_THURSDAY;

	  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the Time #################################################*/
	  /* Set Time: 02:20:00 */
	  stimestructure.Hours = 0x02;
	  stimestructure.Minutes = 0x20;
	  stimestructure.Seconds = 0x00;
	  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
	  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	  if(HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

}

uint8_t PrintUserMenu(void)
{
	uint32_t cnt=0;
   printmsg("RTC Demo Application\r\n");
   printmsg("Calendar Demo--> 1\r\n");
   printmsg("Alarm Demo--> 2\r\n");
   printmsg("Time-Stamp Demo-->3\r\n");
   printmsg("Low-Power Modes Demo-->4\r\n");
   printmsg("Exit this app-->0\r\n");
   printmsg("Type your option here :");


   while(data_user != '\r')
   {
 	  HAL_UART_Receive(&huart2, (uint8_t*)&data_user, 1, HAL_MAX_DELAY);
 	  user_input[cnt]=data_user;
 	  cnt++;
   }
   printmsg("\r\n received inputs %d %d \r\n",user_input[0],user_input[1]);


   switch(user_input[0])
   {
   uint8_t arg,ret;
   case '1':
	   arg = CalendarDemoMenuPrint();
       ret = Calendar_Demo(arg);
       while(ret)
       {
    	   arg = CalendarDemoMenuPrint();
    	   ret = Calendar_Demo(arg);
       }
       break;
   case '2':
	   arg = Alarm_DemoPrint();
       ret = Alarm_Demo(arg);
       while(ret)
       {
    	   ret = Alarm_Demo(arg);
       }
	   break;
   case '3':
	   printmsg("This is time stamp Demo\r\n");

	   if(is_valid_time_set())
	   {
		   printmsg("press the user button to know the time stamp\r\n");
		   TimeStamp_Demo(0);
	   }
	   else
	   {
		   printmsg("Valid time is not set: first set time and try this demo\r\n");
	   }
	   break;
   case '4':

	   arg = LowPowerDemoMenuPrint();
       ret = LowPowerDemo(arg);
       while(ret)
       {
    	   ret = LowPowerDemo(arg);
       }
	   break;
   case '0' :
	   printmsg("Exiting this app\r\n");
	   return 1;
	   break;
   default:
	   printmsg("\r\nInvalid option.. try again\r\r");

   }

   return 0;

}


uint8_t is_valid_time_set(void)
{
	return 1;
}

uint8_t Calendar_Demo(uint8_t arg)
{
uint8_t time_format,seconds,minutes,hour,date,month,year,ampm;
uint8_t user_input[6];
uint32_t cnt=0;
	switch(arg)
	{
	case '1':
		printmsg("Which time format do you want ?\r\n");
		printmsg("24h-->0\r\n");
		printmsg("12h-->1\r\n");
		printmsg("Type your option here :");

		  while(data_user != '\r')
		   {
		 	  HAL_UART_Receive(&huart2, (uint8_t*)&data_user, 1, HAL_MAX_DELAY);
		 	  user_input[cnt]=data_user;
		 	  cnt++;
		   }
		printmsg("\r\n received inputs %d %d \r\n",user_input[0],user_input[1]);
		time_format = (user_input[0]-48);
		printmsg("Enter Hour value here(1-12) or (1-24) :");
		  while(data_user != '\r')
		   {
		 	  HAL_UART_Receive(&huart2, (uint8_t*)&data_user, 1, HAL_MAX_DELAY);
		 	  user_input[cnt]=data_user;
		 	  cnt++;
		   }
	    printmsg("\r\n received inputs %d %d \r",user_input[0],user_input[1]);
	    hour = user_input[0];
		printmsg("Enter minutes value here( 0 to 59 :");
		HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
		printmsg("\r\n received inputs %d %d \r",user_input[0],user_input[1]);
		minutes = user_input[0];
		printmsg("Enter seconds value here( 0 to 59 :");
		HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
		printmsg("\r\n received inputs %d %d \r",user_input[0],user_input[1]);
		seconds = user_input[0];
		printmsg("is it AM(1)/PM(0)?:");
		HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
		ampm = user_input[0];
		RTC_Init();
		RTC_configureUserGivenTime(seconds,minutes,hour,ampm,time_format);
		printmsg("\r\nTime set Successful !\r\n");
		RTC_DisplayCurrentTime();
		break;
	case '2':
		printmsg("Enter Date(1 to 31) value here :");
		HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
	    printmsg("\r\n received inputs %d %d \r\n",user_input[0],user_input[1]);
	    date = user_input[0];
		printmsg("Enter month value here( 1-12) :");
		HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
		printmsg("\r\n received inputs %d %d \r\n",user_input[0],user_input[1]);
		month = user_input[0];
		printmsg("Enter year value here :");
		uint8_t year[5];
		HAL_UART_Receive(&huart2,year,5,HAL_MAX_DELAY);
		uint16_t year_16t = getYear(year);
		RTC_configureUserGivenDate(year_16t,month,date);
		printmsg("Date is set !\r\n");
		RTC_DisplayCurrentDate();
		break;
	case '3':
		RTC_DisplayCurrentTime();
		RTC_DisplayCurrentDate();
		break;
	case '0':
		return 0;

	default :
		printmsg("Enter valid option\r\n");

	}
	return 1;

}

uint8_t Alarm_Demo(uint8_t arg)
{

	return 0;
}
uint8_t LowPowerDemo(uint8_t arg)
{

	return 0;
}

uint8_t  TimeStamp_Demo(uint8_t arg)
{

	return 0;
}

uint8_t CalendarDemoMenuPrint(void)
{
	uint32_t cnt=0;
	printmsg("This is calendar Demo\r\n");
	printmsg("Set time-->1\r\n");
	printmsg("Set date-->2\r\n");
	printmsg("Display current time and date-->3\r\n");
	printmsg("Go back to main menu -->0\r\n");
	printmsg("Type your option here :");
	uint8_t user_input[2];
	  while(data_user != '\r')
	   {
	 	  HAL_UART_Receive(&huart2, (uint8_t*)&data_user, 1, HAL_MAX_DELAY);
	 	  user_input[cnt]=data_user;
	 	  cnt++;
	   }

	printmsg("\r\n received inputs %d %d \r\n",user_input[0],user_input[1]);

	return user_input[0];
}

uint8_t LowPowerDemoMenuPrint(void)
{
	printmsg("This is LOW power demo\r\n");
	printmsg("STOP mode demo+ RTC alarm\r\n");
	printmsg("STANDBY mode demo + RTC wakeup timer\r\n");
	printmsg("Type your option here :");
	uint8_t user_input[2];
	HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
	printmsg("\r\n received inputs %d %d \r",user_input[0],user_input[1]);

	return user_input[0];
}
uint8_t Alarm_DemoPrint(void)
{
	printmsg("This is alarm Demo\r\n");
	printmsg("Display already set alarm\r\n");
	printmsg("Delete an alarm\r\n");
	printmsg("Set new Alarm\r\r");
	printmsg("Type your option here :");
	uint8_t user_input[2];
	HAL_UART_Receive(&huart2,user_input,2,HAL_MAX_DELAY);
	printmsg("\r\n received inputs %d %d \r",user_input[0],user_input[1]);

	return user_input[0];
}

void RTC_configureUserGivenTime(uint8_t seconds,uint8_t minutes,uint8_t hour,uint8_t AMPM, uint8_t format)
{
	  RTC_DateTypeDef  sdatestructure;
	  RTC_TimeTypeDef  stimestructure;



	  /*##-2- Configure the Time #################################################*/
	  /* Set Time: 02:20:00 */
	  stimestructure.Hours = hour;
	  stimestructure.Minutes = minutes;
	  stimestructure.Seconds = seconds;
	  if(format)
	  {
		  if(AMPM)
			  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
		  else
			  stimestructure.TimeFormat = RTC_HOURFORMAT12_PM;
	  }
	  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	  if(HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

}
void RTC_configureUserGivenDate(uint16_t year,uint8_t month,uint8_t date)
{
	RTC_DateTypeDef  sdatestructure;


	year = year % 100;

	  /*##-1- Configure the Date #################################################*/
	  /* Set Date: Tuesday February 18th 2014 */
	  sdatestructure.Year = year;
	  sdatestructure.Month = month;
	  sdatestructure.Date = date;
	  //sdatestructure.WeekDay = RTC_WEEKDAY_THURSDAY;

	  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }
}
void RTC_DisplayCurrentTime(void)
{
	char showtime[50];

	RTC_TimeTypeDef stimestructureget;

	memset(&stimestructureget,0,sizeof(stimestructureget));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);

	/* Display time Format : hh:mm:ss */
	sprintf((char*)showtime,"Current Time is : %02d:%02d:%02d\r\n",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	HAL_UART_Transmit(&huart2,(uint8_t*)showtime,strlen(showtime),HAL_MAX_DELAY);

}
void RTC_DisplayCurrentDate(void)
{
	char showtime[50];

	RTC_DateTypeDef sdatestructureget;

	memset(&sdatestructureget,0,sizeof(sdatestructureget));

	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

	memset(showtime,0,sizeof(showtime));
	sprintf((char*)showtime,"Current Date is : %02d-%2d-%2d\r\n",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);

	HAL_UART_Transmit(&huart2,(uint8_t*)showtime,strlen(showtime),HAL_MAX_DELAY);
}


uint16_t getYear(uint8_t *year)
{
   uint32_t i =0;
   uint8_t value=0;
   uint16_t sum=0;
   for( i = 0 ; i < 4 ; i++)
   {
	   value =  year[i] - 48;
	   if(value  >= 0 && value <=9 )
	   {
           sum  = sum + value * (1000 /(10 ^i));
	   }
	   else
	   {
		   return 0;
	   }
   }

   return sum;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
