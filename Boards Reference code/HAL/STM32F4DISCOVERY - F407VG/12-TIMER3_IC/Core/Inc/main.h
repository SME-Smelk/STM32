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
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define USE_FULL_ASSERT

/*** TIMER2 PWM ***/
#define TIMER2_PRESCALER 599
#define TIMER2_PERIOD 13
/* Pulse % 0 - 1*/
#define TIMER2_DUTY 0.5
#define TIMER2_PULSE ((TIMER2_PERIOD+1)*TIMER2_DUTY)-1
/***--------***/

/*** TIMER3 PWM ***/
#define TIMER3_PRESCALER 0
#define TIMER3_PERIOD 65535
/***--------***/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin 				GPIO_PIN_2
#define USART_TX_GPIO_Port 			GPIOA
#define USART_RX_Pin 				GPIO_PIN_3
#define USART_RX_GPIO_Port 			GPIOAx1

#endif /* __MAIN_H */
