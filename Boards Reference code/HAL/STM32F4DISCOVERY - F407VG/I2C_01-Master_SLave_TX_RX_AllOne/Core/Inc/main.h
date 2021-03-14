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
#include "string.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define MASTER_DEVICE
#define MASTER_WRITE

#define OWN_ADDRESS 			0x50
#define SLAVE_ADDRESS 			OWN_ADDRESS >> 1

/* Mode Master/Slave - Read/Write */
#define MASTER_WRITE_CMD       0xC1
#define MASTER_READ_CMD        0XC2

/*bufer lenght*/
#define READ_LEN    5
#define WRITE_LEN   5

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define USER_BUTTON_Pin GPIO_PIN_0
#define USER_BUTTON_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_13
#define LED_RED_GPIO_Port GPIOD

#endif /* __MAIN_H */
