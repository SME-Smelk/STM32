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

#define SME_GPS_MODULE_ENABLED


/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */
#ifdef SME_GPS_MODULE_ENABLED
  #include "stm32f4xx_sme_gps.h"
#endif /* SME_GPS_MODULE_ENABLED */

#endif /* __STM32F4xx_SME_CONF_H */
