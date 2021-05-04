/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
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
#include "stm32f1xx_sme_conf.h"
/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/


/********************* Ground Fault Parameters *******************************/
#define RESISTOR_GF 1
#define DIV_RESISTOR_PARAMETER 1

/************************** TIMER For ADC ************************************/
#define TIMER2_PRESCALER 0
#define TIMER2_PERIOD 1279

/*************************** ADC AD7892 **************************************/
#define NUMBER_ADC_CH 1
#define NUMBER_OF_DATA 100

/*Manual Calibrate values*/
#define OFFSET_P_ERR 		-110.78
#define GAIN_P_ERR 			205.95
#define OFFSET_N_ERR 		125.25
#define GAIN_N_ERR 			205.59

/* ADC AD7892_1 */
#define AD7892_1_CONVST_Pin 		GPIO_PIN_1
#define AD7892_1_CONVST_GPIO_Port 	GPIOB
#define AD7892_1_RFS_Pin 			GPIO_PIN_2
#define AD7892_1_RFS_GPIO_Port 		GPIOB
#define AD7892_1_EOC_Pin 			GPIO_PIN_11
#define AD7892_1_EOC_GPIO_Port 		GPIOB

/************************** Others pins **************************************/

#define USART_TX_Pin 		GPIO_PIN_2
#define USART_TX_GPIO_Port 	GPIOA
#define USART_RX_Pin 		GPIO_PIN_3
#define USART_RX_GPIO_Port 	GPIOA
#define LD2_Pin 			GPIO_PIN_5
#define LD2_GPIO_Port 		GPIOA


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
