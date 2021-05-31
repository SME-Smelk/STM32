/**
  ******************************************************************************
  * @file    stm32x_sme_conf.h
  * @author  Ismael Poblete V.
  * @brief   Header file of SME module.
  ******************************************************************************
  *	@attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32x_SME_CONF_H
#define __STM32x_SME_CONF_H

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the SME driver
  */

#define SME_GENERALMATH_MODULE_ENABLED

/**
  * @brief  SME Status structures definition
  */
typedef enum
{
  SME_OK       			  = 0x00U,
  SME_ERROR				  = 0x01U,
  SME_STACKOVERFLOW  	  = 0x02U,

  SME_BUSY    			  = 0x03U,
  SME_NEWDATA    		  = 0x04U,
} SME_StatusTypeDef;

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */

#ifdef SME_GENERALMATH_MODULE_ENABLED
  #include <stm32x_sme_generalmath.h>
#endif /* SME_GPS_MODULE_ENABLED */

#endif /* __STM32x_SME_CONF_H */
