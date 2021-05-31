/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.h
  * @author  Ismael Poblete V.
  * @brief   Header file of SME GPS module.
  ******************************************************************************

  ******************************************************************************
  */

#ifndef __STM32F1xx_SME_AD7892_H
#define __STM32F1xx_SME_AD7892_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_spi.h>
#include <stm32f1xx_sme_conf.h>

/**
  * @brief  SME AD7892 structures definition
  */

typedef struct {
	SPI_HandleTypeDef *hspi;
	uint16_t spi_receive_data;
	float lsb_parameter;

	float offset_p_err;
	float gain_p_err;
	float offset_n_err;
	float gain_n_err;

	GPIO_TypeDef *convst_port;
	GPIO_TypeDef *rfs_port;
	GPIO_TypeDef *eoc_port;

	uint16_t convst_pin;
	uint16_t rfs_pin;
	uint16_t eoc_pin;

	float data_adc;
} SME_AD7892_ADC_t;

/**
  * @brief  DAQ AD7892 handle Structure definition
  */

typedef struct {
	TIM_HandleTypeDef *htim;
	float adc_k_parameter;
	uint8_t number_adc_channels;
	uint32_t size_block;
	uint32_t cont_databuff;
	uint8_t flag_buffdata_ready;
	float *adc_buf;
	float **data_acq_buffer;

} SME_AD7892_DAQ_t;


/* Exported constants --------------------------------------------------------*/

/* Range configuration
 *  @FSR_PARAMETER
 *	FSR_PARAMETER_10V for ADC Range +-10V
 *	FSR_PARAMETER_5V for ADC Range +-5V
 * */

#define FSR_PARAMETER_10V 	20.0
#define FSR_PARAMETER_5V 	10.0

/* Global function prototypes -----------------------------------------------*/

/* @AD7892 functions */
SME_StatusTypeDef SME_AD7892_Init(SME_AD7892_ADC_t *ad7892,SPI_HandleTypeDef *hspi,uint8_t fsr_parameter,GPIO_TypeDef *convst_port,uint16_t convst_pin,GPIO_TypeDef *rfs_port,uint16_t rfs_pin,GPIO_TypeDef *eoc_port,uint16_t eoc_pin,float offset_p_err,float gain_p_err,float offset_n_err,float gain_n_err);
float SME_AD7892_Getdata(SME_AD7892_ADC_t *ad7892);

/* @Timer_Integration data acquisition functions */
SME_StatusTypeDef SME_AD7892_DAQ_Init(SME_AD7892_DAQ_t *daq,TIM_HandleTypeDef *htim,uint8_t number_adc_channels,uint32_t size_block, float adc_k_parameter);
SME_StatusTypeDef SME_AD7892_DAQ_data_acquisition(SME_AD7892_DAQ_t *daq);
SME_StatusTypeDef SME_AD7892_DAQ_reset_request(SME_AD7892_DAQ_t *daq);

#endif /* STM32F1xx_SME_AD7892_H_ */
