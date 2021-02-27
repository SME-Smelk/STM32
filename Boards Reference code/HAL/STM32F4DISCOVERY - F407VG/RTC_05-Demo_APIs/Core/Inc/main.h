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
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_Port GPIOD
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_Port GPIOA

#define LED1_PORT GPIOD
#define LED2_PORT GPIOD
#define LED3_PORT GPIOD
#define LED4_PORT GPIOD

#define LED1_PIN_NO GPIO_PIN_12
#define LED2_PIN_NO GPIO_PIN_13
#define LED3_PIN_NO GPIO_PIN_14
#define LED4_PIN_NO GPIO_PIN_15

//#define RTC_CLOCK_SOURCE_LSE
#define RTC_CLOCK_SOURCE_LSI
#endif /* __MAIN_H */
