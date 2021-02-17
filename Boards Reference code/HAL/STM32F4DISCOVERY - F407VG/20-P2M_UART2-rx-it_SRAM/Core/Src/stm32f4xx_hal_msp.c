/**
  ******************************************************************************
  * File Name          : stm32f4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern DMA_HandleTypeDef hdma1;

/* External functions --------------------------------------------------------*/


/**
  * Initializes the Global MSP.
  */

void HAL_MspInit(void)
{

  /* System interrupt init*/
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(huart->Instance==USART2)
		{
		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		 GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


		/* USART2 DMA Init */
		/* USART2_RX Init */
		hdma1.Instance = DMA1_Stream5;
		hdma1.Init.Channel = DMA_CHANNEL_4;
		hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma1.Init.MemInc = DMA_MINC_ENABLE;
		hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma1.Init.Mode = DMA_NORMAL;
		hdma1.Init.Priority = DMA_PRIORITY_LOW;
		hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma1) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(huart,hdmarx,hdma1);
	}
}



