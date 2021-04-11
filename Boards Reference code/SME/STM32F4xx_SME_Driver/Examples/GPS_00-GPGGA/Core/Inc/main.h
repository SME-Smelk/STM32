/*
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx_hal.h>
#include <stm32f4xx_sme_conf.h>

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define USART1_TX_Pin 				GPIO_PIN_6
#define USART1_TX_GPIO_Port 		GPIOB
#define USART1_RX_Pin 				GPIO_PIN_7
#define USART1_RX_GPIO_Port 		GPIOB

#define USART2_TX_Pin 				GPIO_PIN_2
#define USART2_TX_GPIO_Port 		GPIOA
#define USART2_RX_Pin 				GPIO_PIN_3
#define USART2_RX_GPIO_Port 		GPIOA
#endif /* __MAIN_H */
