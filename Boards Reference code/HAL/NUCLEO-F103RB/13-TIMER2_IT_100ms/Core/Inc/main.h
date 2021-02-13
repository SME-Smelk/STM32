/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin 			GPIO_PIN_2
#define USART_TX_GPIO_Port 		GPIOA
#define USART_RX_Pin 			GPIO_PIN_3
#define USART_RX_GPIO_Port 		GPIOA
#define LD2_Pin 				GPIO_PIN_5
#define LD2_GPIO_Port 			GPIOA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
