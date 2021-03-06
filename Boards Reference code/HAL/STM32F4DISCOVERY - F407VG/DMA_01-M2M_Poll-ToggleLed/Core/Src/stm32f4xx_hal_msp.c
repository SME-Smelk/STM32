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
extern DMA_HandleTypeDef hdma2;
extern ADC_HandleTypeDef hadc1;
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
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  }
}


/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void DMA_Init(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC1)
  {
	/* Peripheral clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**ADC1 GPIO Configuration
	PA4     ------> ADC1_IN4
	PA5     ------> ADC1_IN5
	PA6     ------> ADC1_IN6
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    /* ADC2 DMA Init */
    /* ADC2 Init */
    hdma2.Instance = DMA2_Stream0;
    hdma2.Init.Channel = DMA_CHANNEL_0;
    hdma2.Init.Direction = DMA_MEMORY_TO_MEMORY;
    hdma2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma2.Init.MemInc = DMA_MINC_ENABLE;
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

  }
}
