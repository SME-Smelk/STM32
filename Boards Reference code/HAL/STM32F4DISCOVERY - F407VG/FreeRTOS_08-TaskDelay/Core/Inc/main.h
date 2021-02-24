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
/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define USE_FULL_ASSERT
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

#endif /* __MAIN_H */
