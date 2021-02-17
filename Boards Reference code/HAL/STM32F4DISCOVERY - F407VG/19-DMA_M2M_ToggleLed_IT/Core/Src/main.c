/**
  ******************************************************************************
  * @Project        : 19-DMA_M2M_ToggleLed_IT
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-17-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Blinking LED_GREEN PD12 with Interrupt. Simple DMA transfer
  * 				  Memory to Memory from SRAM (data) to register ODR of LED.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSE
  * 	SYSCLK: 		84MHz
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PD12      	------> LED_GREEN
  * @note
  * 	-Interrupt non-blocking Poll transfer with DMA
  * 	-Whenever stream can be used. Use those that are not linked to others
  * 	 peripherals.
  * 	-We can see in PD12 (LED_GREEN) blinking
  * 	-Config DMA:
  * 		-We use DMA2, because bus matrix. DMA1 don't have to access to the
  * 		 memory by bus peripheral (DMA_PI)
  * 		-We used PeriphDataAlignment and MemDataAlignment like a Half word,
  * 		 because the led ODR register is a 16 bits register and write the
  * 	 	 value 0x1000 set the PD12, bit 12.
  * 		-We dont need increment, because is only one data, same too burst.
  * 		-We need a FIFOMode to use M2M, because
  * 		-M2M, Transfer Memory to Memory.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma2;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void USART2_UART_Init(void);
static void DMA_Init(void);

void my_Dma_TC_cb( DMA_HandleTypeDef *pHandle);
/* Private user code ---------------------------------------------------------*/
uint16_t led_data[2] = { 0x0000, 0x1000 };

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
	USART2_UART_Init();
	DMA_Init();

	/* Start Code */
	HAL_DMA_RegisterCallback(&hdma2,HAL_DMA_XFER_CPLT_CB_ID,&my_Dma_TC_cb);

	while (1)
	{

		HAL_DMA_Start_IT(&hdma2,(uint32_t)&led_data[1],(uint32_t) &GPIOD->ODR, 1);
		HAL_DMA_PollForTransfer(&hdma2, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		HAL_Delay(1000);
		HAL_DMA_Start_IT(&hdma2,(uint32_t)&led_data[0],(uint32_t) &GPIOD->ODR, 1);
		HAL_DMA_PollForTransfer(&hdma2, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		HAL_Delay(1000);

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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void USART2_UART_Init(void)
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GREEN_Port, &GPIO_InitStruct);
}


/**
* @brief DMA
*/
static void DMA_Init(void)
{
	/* Peripheral clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	hdma2.Instance = DMA2_Stream0;
	hdma2.Init.Channel = DMA_CHANNEL_0;
	hdma2.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma2.Init.MemInc = DMA_MINC_DISABLE;
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma2.Init.Mode = DMA_NORMAL;
	hdma2.Init.Priority = DMA_PRIORITY_LOW;
	hdma2.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma2.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma2.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma2) != HAL_OK)
	{
		Error_Handler();
	}

	/* DMA interrupt init */
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/* User IT DMA */
void my_Dma_TC_cb( DMA_HandleTypeDef *pHandle)
{


}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
