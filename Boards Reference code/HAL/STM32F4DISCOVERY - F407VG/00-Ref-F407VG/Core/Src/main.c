/**
  ******************************************************************************
  * @Project        : 00-BASE-Basic
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-20-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Base programming guide
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSI/HSE/PLL(HSI)/PLL(HSE) 	(Ej: HSI)
  * 	SYSCLK: 		XMHz						(EJ: 8MHz)
  * 	RTCSource:		None/LSI/LSE/HSE-DIVx 			(Ej: LSI)
  * 	RTCCLK: 		None/XMHz						(EJ: 32kHz)
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PA5      	------> LD2
  * 		PC13     	<------ B1(Button)
  * 	*ADC
  * 		PA0    	 	------> ADC1,INO
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void GPIO_Init(void);
void DMA_Init(void);
/* Private user code ---------------------------------------------------------*/


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* Configure the system clock */

	/* Configure the peripherals */
	GPIO_Init();
	DMA_Init();

	/* Start Code */


	while (1){

	}
}

void GPIO_Init(void){

}

void DMA_Init(void){

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
