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
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* The slave address is 0x28
 * then the OWN_ADDRESS will be 0x50
 * Because shift the address.
 * 0x50 = 01010000 >> 1:
 * 0x28 = 00101000
*/
#define OWN_ADDRESS 			0x50
#define SLAVE_ADDRESS 			OWN_ADDRESS >> 1

/* Slave id and version*/
#define SLAVE_ID				0xE0
#define SLAVE_VER				"1.0"
#define NUMBER_VAR_DATA			3

/* Mode Master/Slave - Read/Write */
#define CHECK_STATUS_CMD       		0xC1
#define READ_ID_VER_CMD        		0XC2
#define READ_DATA_CMD        		0XC3
#define WRITE_TO_PARAMETER_CMD 		0XC4
#define RESET_SLAVE_CMD        		0XC5

/*Data lenght*/
#define LEN_CHECK_STATUS_CMD   		1
#define LEN_READ_ID_VER_CMD   		4
#define LEN_READ_DATA_CMD   		4 * NUMBER_VAR_DATA
#define LEN_WRITE_TO_PARAMETER_CMD  5
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#define USER_BUTTON_GPIO_Port GPIOC
#define USER_BUTTON_Pin GPIO_PIN_13


/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

