/**
  ******************************************************************************
  * @Project        : 25-LowPower_modes
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-19-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example the use the LowPower modes:
  * 					-Sleep Mode
  * 					-Stop Mode
  * 					-Standby Mode
  * 				  The change of mode is via user button.
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
  * 	-Systick is desactivate CTRL = 0, because can wake up as it is an active
  * 	 interrupt it can wake up the system.
  * 	-In WFI is more selective, its wake up depend of Actual priority and PRIMASK
  * 	-Run mode, default mode. Is a non low power mode after a system or a power-on reset.
  * 	-Low Power Modes:
  * 		-Sleep mode: CPU CLK is turned OFF and there is no effect on other clocks
  * 			or analog clock sources. The current consumption is HIGHEST in this mode,
  * 			compared to other Low Power Modes.
  * 		-Stop Mode: In Stop mode, all clocks in the 1.2 V domain are stopped, the
  * 			PLLs, the HSI and the HSE RC oscillators are disabled. Internal SRAM and
  * 			register contents are preserved. Have may different categories.
  * 		-Standby Mode: It is based on the Cortex®-M4 with FPU deepsleep mode, with the
  * 			voltage regulator disabled. The 1.2 V domain is consequently powered off.
  * 			The PLLs, the HSI oscillator and the HSE oscillator are also switched off.
  * 	-Use the button to change thru low power modes. Once time you press the button
  * 	 it just wait 2 seconds to start the next mode (to avoid debounce):
  * 	 	-1. Run mode (Default): 8mA
  * 	 	-2. Sleep mode: 3.52mA
  * 	 	-3. Stop mode: 1.97mA
  * 	 	-4. Standby mode (Reset to main): 0.56mA
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
		i=0;
		char *str1 = "In Run mode: \n";
		HAL_UART_Transmit(&huart2, (uint8_t *) str1, strlen (str1), HAL_MAX_DELAY);
		flag_user_waiting = 1;
		while(flag_user_waiting);
		i++;
		/******************* SLEEP MODE *****************/
		char *str2 = "Sleep Mode: In 2 seg... ";
		HAL_UART_Transmit(&huart2, (uint8_t *) str2, strlen (str2), HAL_MAX_DELAY);

		  /** Blink the LED **/
		  for (int i=0; i<4; i++)
		  {
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			  HAL_Delay(500);
		  }

		char *str3 = "In Sleep Mode.\n";
		HAL_UART_Transmit(&huart2, (uint8_t *) str3, strlen (str3), HAL_MAX_DELAY);

		/***     Go to Sleep Mode    ****/
		HAL_SuspendTick();
		__WFI();

		/*** wake up from sleep mode ****/
		HAL_ResumeTick();
		i++;
		/******************* STOP MODE ******************/
		char *str4 = "Stop Mode: In 2 seg... ";
		HAL_UART_Transmit(&huart2, (uint8_t *) str4, strlen (str4), HAL_MAX_DELAY);

		  /** Blink the LED **/
		  for (int i=0; i<4; i++)
		  {
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			  HAL_Delay(500);
		  }

		char *str5 = "In Stop Mode.\n";
		HAL_UART_Transmit(&huart2, (uint8_t *) str5, strlen (str5), HAL_MAX_DELAY);

		/*** enable sleep on exit for interrupt only operations ****/
		HAL_SuspendTick();
		HAL_PWR_EnableSleepOnExit();

		/*** Go to Stop Mode ****/
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

		/*** wake up from stop mode ****/
		i++;
		/******************* STANDBY MODE ******************/

		char *str6 = "Standby Mode: In 2 seg... ";
		HAL_UART_Transmit(&huart2, (uint8_t *) str6, strlen (str6), HAL_MAX_DELAY);

		  /** Blink the LED **/
		  for (int i=0; i<4; i++)
		  {
			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			  HAL_Delay(500);
		  }

		char *str7 = "In Standby Mode.\n";
		HAL_UART_Transmit(&huart2, (uint8_t *) str7, strlen (str7), HAL_MAX_DELAY);

	    //Enable the wakeup pin 1 in pwr_csr register
	    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	    //Enable backup voltage reg.
	    HAL_PWREx_EnableBkUpReg();

	    HAL_PWR_EnterSTANDBYMode();


		/*** wake up from sleep mode ****/
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
