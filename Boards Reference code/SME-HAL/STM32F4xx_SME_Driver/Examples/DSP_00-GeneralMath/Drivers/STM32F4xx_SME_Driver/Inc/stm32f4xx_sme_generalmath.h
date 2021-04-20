/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.h
  * @author  Ismael Poblete V.
  * @brief   Header file of SME GPS module.
  ******************************************************************************

  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_SME_GENERALMATH_H
#define __STM32F4xx_SME_GENERALMATH_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx_hal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stm32f4xx_sme_conf.h>

/**
  * @brief  NMEA GPGGA handle Structure definition
  */

typedef struct {

	uint32_t cont_databuff;
	uint8_t flag_buffdata_ready;				/*!< Counter like a motor to fill data to the buffer array.*/
	uint16_t *adc_buf;

} GeneralMath_HandleTypeDef;

typedef struct {

	//float average_rms[];

} GeneralMath_Average_HandleTypeDef;

typedef struct {

	//float output_rms[];

} GeneralMath_RMS_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/

SME_StatusTypeDef SME_GeneralMath_Init(GeneralMath_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc, uint32_t Length);
SME_StatusTypeDef SME_GeneralMath_data_acquisition(GeneralMath_HandleTypeDef *generalmath, ADC_HandleTypeDef* hadc,uint8_t number_adc_channels,uint32_t size_block,float k_parameter,float input_block[number_adc_channels][size_block]);
SME_StatusTypeDef SME_GeneralMath_reset_dma_request(GeneralMath_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc, uint32_t Length);
SME_StatusTypeDef SME_GeneralMath_rms_float32(GeneralMath_HandleTypeDef *generalmath,uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_rms[number_adc_channels]);
SME_StatusTypeDef SME_GeneralMath_average_float32(GeneralMath_HandleTypeDef *generalmath,uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_average[number_adc_channels]);

#endif /* __STM32F4xx_SME_GENERALMATH_H */
