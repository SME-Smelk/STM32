/**
  ******************************************************************************
  * @Project        : 20-DMA_UART2-RX_SRAM
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-17-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Read the UART2 RX via DMA to SRAM with 10 bytes buffer.
  * 				  See data via window "Memory Browser"
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
  * 	-UART2 RX transfer via DMA
  * 	-Used stream 5, Channel 4. See reference RM0090 "Table 42. DMA1 request mapping"
  * 	-We can use PD12 (LED_GREEN).
  * 	-Config DMA:
  * 		-UART2 RX belongs to DMA1 thats is the reason for used it.
  * 		-We used PeriphDataAlignment and MemDataAlignment like a Half word
  * 		-We need to increment the memory to write to SRAM for each character
  * 		-P2M, Peripheral to Memory.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma1;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void USART2_UART_Init(void);
static void DMA_Init(void);

/* Private user code ---------------------------------------------------------*/
#define OFFSET 0x800
#define DEST_ADDRESS (SRAM1_BASE + OFFSET)

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
	DMA_Init();
	USART2_UART_Init();


	/* Start Code */

	while (1)
	{
		HAL_UART_Receive_DMA(&huart2, (uint8_t*)DEST_ADDRESS,10);
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

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  /* Prevent unused argument(s) compilation warning */
	  UNUSED(huart);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
