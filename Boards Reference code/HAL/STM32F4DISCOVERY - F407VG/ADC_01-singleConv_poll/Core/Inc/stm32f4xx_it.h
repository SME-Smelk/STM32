/**
  ******************************************************************************
  * @file    stm32f4xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
 void NMI_Handler(void);
 void HardFault_Handler(void);
 void MemManage_Handler(void);
 void BusFault_Handler(void);
 void UsageFault_Handler(void);
 void SysTick_Handler(void);


#endif /* __STM32F4xx_IT_H */
