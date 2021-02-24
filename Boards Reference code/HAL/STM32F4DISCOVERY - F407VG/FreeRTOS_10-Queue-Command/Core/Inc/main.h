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
#include "usart2.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "queue.h"
#include "timers.h"
/* Private includes ----------------------------------------------------------*/
UART_HandleTypeDef huart2;
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0
#define BUTTON_PRESSED TRUE
#define BUTTON_NOT_PRESS FALSE

//Green LED
#define LED_GREEN_PIN GPIO_PIN_14
#define LED_GREEN_PORT GPIOD
//Button
#define BUTTON_PIN GPIO_PIN_0
#define BUTTON_PORT GPIOA

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/

#define LED_ON_COMMAND 					1
#define LED_OFF_COMMAND 				2
#define LED_TOGGLE_COMMAND 				3
#define LED_TOGGLE_STOP_COMMAND 		4
#define LED_READ_STATUS_COMMAND 		5
#define RTC_READ_DATE_TIME_COMMAND		6
#define EXIT_APP						0

#endif /* __MAIN_H */
