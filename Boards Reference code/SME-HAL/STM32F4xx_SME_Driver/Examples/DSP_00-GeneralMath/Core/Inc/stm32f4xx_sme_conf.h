/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.h
  * @author  Ismael Poblete V.
  * @brief   Header file of CORTEX HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef __STM32F4xx_SME_CONF_H
#define __STM32F4xx_SME_CONF_H

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the SME driver
  */

//#define SME_GPS_MODULE_ENABLED
#define SME_GENERALMATH_MODULE_ENABLED


/**
  * @brief  SME GPS Status structures definition
  */
typedef enum
{
  SME_OK       			  = 0x00U,
  SME_ERROR				  = 0x01U,

  SME_BUSY    			  = 0x02U,
  SME_NEWDATA    		  = 0x03U,
} SME_StatusTypeDef;

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */
#ifdef SME_GPS_MODULE_ENABLED
  #include "stm32f4xx_sme_gps.h"
#endif /* SME_GPS_MODULE_ENABLED */

#ifdef SME_GENERALMATH_MODULE_ENABLED
  #include "stm32f4xx_sme_generalmath.h"
#endif /* SME_GENERALMATH_MODULE_ENABLED */

#endif /* __STM32F4xx_SME_CONF_H */
