/**
  ******************************************************************************
  * @file    stm32l4xx_sme_generalmath.h
  * @author  Ismael Poblete V.
  * @brief   Header file of SME GeneralMath module.
  ******************************************************************************

  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4xx_SME_GENERALMATH_H
#define __STM32L4xx_SME_GENERALMATH_H

/* Includes ------------------------------------------------------------------*/
#include <stm32l4xx_hal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stm32l4xx_sme_conf.h>

/* Exported constants --------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/
float SME_GeneralMath_rms_float32(uint32_t size_block, float *input_buff_voltage);
float SME_GeneralMath_average_float32(uint32_t size_block, float *input_buff_voltage);

#endif /* __STM32L4xx_SME_GENERALMATH_H */
