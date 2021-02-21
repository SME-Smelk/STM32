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

#define SLAVE_ADDRESS 0x68

/* Mode Master/Slave - Read/Write */
#define MASTER_WRITE_CMD       0xC1
#define MASTER_READ_CMD        0XC2

/*slave*/
#define SLAVE_OWN_ADDRESS      (uint8_t)0x53;
#define SLAVE_ADDRESS_READ    (uint8_t) 0xA7
#define SLAVE_ADDRESS_WRITE    (uint8_t) 0xA6

/*bufer lenght*/
#define READ_LEN    5
#define WRITE_LEN   5

/* Exported macro ------------------------------------------------------------*/
#define USE_FULL_ASSERT

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define LED_GREEN_pin 				GPIO_PIN_12
#define LED_GREEN_port 				GPIOD

#endif /* __MAIN_H */
