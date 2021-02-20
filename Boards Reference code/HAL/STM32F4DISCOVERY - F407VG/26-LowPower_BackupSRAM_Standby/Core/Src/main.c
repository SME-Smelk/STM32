/**
  ******************************************************************************
  * @Project        : 26-LowPower_Backup_SRAM_Standby
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-19-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example the use BackUp SRAM recovery in Standby Mode Low power.
  * 				  The change of mode is via user button.
  * 				  We write "hello" in SRAM Backup.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PA0      	------> USER_BUTTON
  * @Note
  * 	-In Standby Mode: SRAM and register contents are lost except for registers in the
  * 	 backup domain. The backup domain consist in RTC registers, RTC backup register and
  * 	 backup SRAM.
  * 	-BackUp Memory SRAM not depend of clocks, agains that depend on LP Voltage regulator
  * 	 from backup domain
  * 	-The STM32F4 have a Back up memory 4Kbyte of SRAM. It can be use to save data when
  * 	 a loss power is produced, like the effect of the Standby Mode which produce a loss
  * 	 data inside. The normal SRAM for effect of the Standby mode is deleted.
  * 	-Systick is desactivate CTRL = 0, because can wake up as it is an active
  * 	 interrupt it can wake up the system.
  * 	-Low Power Modes:
  * 		-Standby Mode: It is based on the Cortex®-M4 with FPU DeepSleep mode, with the
  * 			voltage regulator disabled. The 1.2 V domain is consequently powered off.
  * 			The PLLs, the HSI oscillator and the HSE oscillator are also switched off.
  * 	-Use the button to exit of the standby mode by "wake up pin" A0 (USER_BUTTON).
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

uint8_t flag_user_waiting = 0;
uint8_t i = 0;

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

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	uint32_t * pBackupSRAMbase=0;

	char write_buf[] = "Hello";

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	GPIO_AnalogConfig();
	GPIO_Init();
	UART2_Init();

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

	//1. Turn on the clock in RCC register for backup sram
	__HAL_RCC_BKPSRAM_CLK_ENABLE();

	//2. Enable Write access to the backup domain
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	pBackupSRAMbase = (uint32_t*)BKPSRAM_BASE;

	//Enable clock for PWR Controller block
	__HAL_RCC_PWR_CLK_ENABLE();

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		printmsg("woke up from the standby mode\r\n");
		uint8_t data = (uint8_t)*pBackupSRAMbase;
		if(data != 'H')
		{
			printmsg("Backup SRAM data is lost\r\n");
		}
		else
		{
			printmsg("Backup SRAM data is safe \r\n");
		}

	}else
	{
		for(uint32_t i =0 ; i < strlen(write_buf)+1 ; i++)
		{
			*(pBackupSRAMbase+i) = write_buf[i];
		}
	}


	printmsg("Press the user button to enter standby mode\r\n");
	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) != GPIO_PIN_RESET);

	//when user pushes the user button, it comes here
	printmsg("Going to Standby mode\r\n");

	//Enable the wakeup pin 1 in pwr_csr register
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	//Enable backup voltage reg.
	HAL_PWREx_EnableBkUpReg();

	HAL_PWR_EnterSTANDBYMode();



	while(1);

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

	if(flag_user_waiting){
		flag_user_waiting = 0;
	}
	if(i==2){
		SystemClock_Config();
		HAL_ResumeTick();
		HAL_PWR_DisableSleepOnExit();
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
