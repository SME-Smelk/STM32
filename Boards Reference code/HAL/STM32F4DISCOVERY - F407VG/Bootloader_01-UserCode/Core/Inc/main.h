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
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define BL_DEBUG_MSG_EN

#define FLASH_SECTOR2_BASE_ADDRESS 0x08008000U

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_12
#define LD4_GPIO_Port GPIOD
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOD

#endif /* __MAIN_H */
