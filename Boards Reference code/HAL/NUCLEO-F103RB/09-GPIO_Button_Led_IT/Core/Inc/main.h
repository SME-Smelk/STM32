/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define B1_Pin 						GPIO_PIN_13
#define B1_GPIO_Port 				GPIOC
#define B1_EXTI_IRQn 				EXTI15_10_IRQn
#define LD2_Pin 					GPIO_PIN_5
#define LD2_GPIO_Port 				GPIOA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
