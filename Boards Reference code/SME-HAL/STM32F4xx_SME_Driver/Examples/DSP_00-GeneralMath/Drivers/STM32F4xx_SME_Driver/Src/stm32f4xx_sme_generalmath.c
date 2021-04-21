/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.c
  * @author  Ismael Poblete V.
  * @brief   GPS SME module driver.
  *          This file provides software functions to manage the GPS NMEA protocol
  *          with UART RX IT HAL functions.
  @verbatim
  ==============================================================================
                    	##### SME GPS features #####
  ==============================================================================


	Use sentences NMEA for data acquisition from SME GPS.

	Sentences compatibles and tested:
	(+)GPGGA	identified: SENTENCE_GPGGA

	Global functions for user:

	(+)SME_GPS_Init			: For initialization structure parameters
							  GPS_HandleTypeDef
	(+)SME_GPS_detecCommand	: Function to detect a GPS NMEA sentence
	(+)SME_GPS_DataProcess	: When the flag flag_data_ready = true it is ready
							  to process the buffer data of GPS NMEA structure
							  data_buffer.

                     ##### How to use this driver #####
  ==============================================================================

	(#) Set a structure type SME GPS GPS_HandleTypeDef

	(#) Set a structure type HAL UART with configuration as interrupt RX

	(#) Consider SENTENCE_GPGGA macro for GPGGA recognize in SME_GPS_detecCommand

	(#) Feed the GPS functions:

		(++)Init function
				SME_GPS_Init(&GPS_UART2,&GPS_NMEA).

		(++)For data interrupt acquisition in rx UART interrupt callback.
				SME_GPS_detecCommand(&GPS_UART2,&GPS_NMEA,SENTENCE_GPGGA);

		(++)For process data in infinite while loop execution.
				SME_GPS_DataProcess(&GPS_NMEA);

  @endverbatim
  ******************************************************************************

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_sme_generalmath.h"

#ifdef SME_GENERALMATH_MODULE_ENABLED

/* Private function prototypes -----------------------------------------------*/



/* Exported functions ---------------------------------------------------------*/
/*
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
 */

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_Start(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc,  uint32_t size_block,uint32_t number_adc_channels, float adc_k_parameter){
	generalmath->cont_databuff=0;
	generalmath->size_block=size_block;
	generalmath->flag_buffdata_ready = RESET;
	generalmath->adc_k_parameter = adc_k_parameter;
	generalmath->number_adc_channels = number_adc_channels;
	generalmath->adc_buf = malloc(generalmath->number_adc_channels);

	for(int i = 0; i < number_adc_channels;i++){
		generalmath->input_buff_voltage[i] = malloc(generalmath->number_adc_channels);
	}

	if(HAL_ADC_Start_DMA(hadc, (uint32_t*)generalmath->adc_buf, generalmath->number_adc_channels) == HAL_ERROR){
		return SME_ERROR;
	}
	return SME_OK;
}

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_data_acquisition(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	if(generalmath->flag_buffdata_ready == RESET){
		uint8_t number_adc_channels = generalmath->number_adc_channels;
		for (int i = 0; i < 2; i++)
		{
			generalmath->input_buff_voltage[i][generalmath->cont_databuff] = (float)generalmath->adc_buf[i] * generalmath->adc_k_parameter;
		}
		generalmath->cont_databuff++;
		if(generalmath->cont_databuff >= generalmath->size_block){
			generalmath->cont_databuff=0;
			generalmath->flag_buffdata_ready = SET;
			HAL_ADC_Stop_DMA(generalmath->adc_handler);
			return SME_NEWDATA;
		}
	}
	return SME_BUSY;
}

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
SME_StatusTypeDef SME_GeneralMath_reset_dma_request(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	generalmath->flag_buffdata_ready = RESET;
	free(generalmath->adc_buf);
	for(int i = 0; i < generalmath->number_adc_channels;i++){
		free(generalmath->input_buff_voltage[i]);
	}

	generalmath->adc_buf = malloc(generalmath->number_adc_channels);
	for(int i = 0; i < generalmath->number_adc_channels;i++){
		generalmath->input_buff_voltage[i] = malloc(generalmath->number_adc_channels);
	}
	if(HAL_ADC_Start_DMA(generalmath->adc_handler, (uint32_t*)generalmath->adc_buf, generalmath->number_adc_channels) == HAL_ERROR){
		return SME_ERROR;
	}
	return SME_OK;
}

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
SME_StatusTypeDef SME_GeneralMath_rms_float32(uint32_t number_adc_channels, uint32_t size_block, float **input_buff_voltage,float output_rms[number_adc_channels]){
		float sum_v2[number_adc_channels];

		for (int i =0; i < number_adc_channels; i++)
		{
			for (int j =0; j < size_block; j++)
			{
				/* Acquire the sum pot 2 */
				if(j==0){
					sum_v2[i] = input_buff_voltage[i][j] * input_buff_voltage[i][j];
				}else{
					sum_v2[i] = sum_v2[i] + input_buff_voltage[i][j] * input_buff_voltage[i][j];
				}
			}
			output_rms[i] = sqrt(sum_v2[i] / (float) size_block);
		}
		return SME_OK;
}

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
SME_StatusTypeDef SME_GeneralMath_average_float32(uint32_t number_adc_channels, uint32_t size_block, float **input_buff_voltage,float output_average[number_adc_channels]){

		float sum_average[number_adc_channels];

		for (int i =0; i < number_adc_channels; i++)
		{
			for (int j =0; j < size_block; j++)
			{
				/* Acquire the sum pot 2 */
				if(j==0){
					sum_average[i] = input_buff_voltage[i][j];
				}else{
					sum_average[i] = sum_average[i] + input_buff_voltage[i][j];
				}
			}
			output_average[i] = sum_average[i] / (float) size_block;
		}
		return SME_OK;
}


#endif /* SME_GENERALMATH_MODULE_ENABLED */
