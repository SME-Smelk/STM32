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
	ADC_HandleTypeDef* adc_handler;
	float adc_k_parameter;
	uint8_t number_adc_channels;
	uint32_t size_block;
	uint16_t *adc_buf;

	uint32_t cont_databuff;
	uint8_t flag_buffdata_ready;				/*!< Counter like a motor to fill data to the buffer array.*/

	float **input_buff_voltage;

} GeneralMath_DMA_DAQ_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/

SME_StatusTypeDef SME_GeneralMath_DMA_Start(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc,  uint32_t size_block,uint32_t number_adc_channels, float adc_k_parameter);
SME_StatusTypeDef SME_GeneralMath_DMA_data_acquisition(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath);
SME_StatusTypeDef SME_GeneralMath_reset_dma_request(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath);
SME_StatusTypeDef SME_GeneralMath_rms_float32(uint32_t number_adc_channels, uint32_t size_block, float **input_buff_voltage,float output_rms[number_adc_channels]);
SME_StatusTypeDef SME_GeneralMath_average_float32(uint32_t number_adc_channels, uint32_t size_block, float **input_buff_voltage,float output_average[number_adc_channels]);

#endif /* __STM32F4xx_SME_GENERALMATH_H */
