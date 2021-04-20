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
SME_StatusTypeDef SME_GeneralMath_Init(GeneralMath_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc, uint32_t number_adc_channels, float adc_k_parameter){
	generalmath->DAQ.cont_databuff=0;
	generalmath->DAQ.flag_buffdata_ready = RESET;
	generalmath->DAQ.adc_k_parameter = adc_k_parameter;
	generalmath->DAQ.number_adc_channels = number_adc_channels;
	generalmath->DAQ.adc_buf = malloc(generalmath->DAQ.number_adc_channels);

	for(int i = 0; i < number_adc_channels;i++){
		generalmath->DAQ.input_buff_voltage[i] = malloc(generalmath->DAQ.number_adc_channels);
	}

	if(HAL_ADC_Start_DMA(hadc, (uint32_t*)generalmath->DAQ.adc_buf, generalmath->DAQ.number_adc_channels) == HAL_ERROR){
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
SME_StatusTypeDef SME_GeneralMath_data_acquisition(GeneralMath_HandleTypeDef *generalmath, ADC_HandleTypeDef* hadc){
	if(generalmath->DAQ.flag_buffdata_ready == RESET){
		for (int i =0; i<generalmath->DAQ.number_adc_channels; i++)
		{
			generalmath->DAQ.input_buff_voltage[i][generalmath->DAQ.cont_databuff] = (float)generalmath->DAQ.adc_buf[i] * generalmath->DAQ.adc_k_parameter;
		}
		generalmath->DAQ.cont_databuff++;
		if(generalmath->DAQ.cont_databuff >= generalmath->size_block){
			generalmath->DAQ.cont_databuff=0;
			generalmath->DAQ.flag_buffdata_ready = SET;
			HAL_ADC_Stop_DMA(hadc);
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
SME_StatusTypeDef SME_GeneralMath_reset_dma_request(GeneralMath_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc){
	generalmath->DAQ.flag_buffdata_ready = RESET;
	free(generalmath->DAQ.adc_buf);
	for(int i = 0; i < generalmath->DAQ.number_adc_channels;i++){
		free(generalmath->DAQ.input_buff_voltage[i]);
	}

	generalmath->DAQ.adc_buf = malloc(generalmath->DAQ.number_adc_channels);
	for(int i = 0; i < generalmath->DAQ.number_adc_channels;i++){
		generalmath->DAQ.input_buff_voltage[i] = malloc(generalmath->DAQ.number_adc_channels);
	}
	if(HAL_ADC_Start_DMA(hadc, (uint32_t*)generalmath->DAQ.adc_buf, generalmath->DAQ.number_adc_channels) == HAL_ERROR){
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
SME_StatusTypeDef SME_GeneralMath_rms_float32(GeneralMath_HandleTypeDef *generalmath, float output_rms[generalmath->DAQ.number_adc_channels]){
	if(generalmath->DAQ.flag_buffdata_ready == SET){
		float sum_v2[generalmath->DAQ.number_adc_channels];

		for (int i =0; i < generalmath->DAQ.number_adc_channels; i++)
		{
			for (int j =0; j < generalmath->size_block; j++)
			{
				/* Acquire the sum pot 2 */
				if(j==0){
					sum_v2[i] = generalmath->DAQ.input_buff_voltage[i][j] * generalmath->DAQ.input_buff_voltage[i][j];
				}else{
					sum_v2[i] = sum_v2[i] + generalmath->DAQ.input_buff_voltage[i][j] * generalmath->DAQ.input_buff_voltage[i][j];
				}
			}
			output_rms[i] = sqrt(sum_v2[i] / (float) generalmath->size_block);
		}
		return SME_OK;
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
SME_StatusTypeDef SME_GeneralMath_average_float32(GeneralMath_HandleTypeDef *generalmath, float output_average[generalmath->DAQ.number_adc_channels]){
	if(generalmath->DAQ.flag_buffdata_ready == SET){
		float sum_average[generalmath->DAQ.number_adc_channels];

		for (int i =0; i < generalmath->DAQ.number_adc_channels; i++)
		{
			for (int j =0; j < generalmath->size_block; j++)
			{
				/* Acquire the sum pot 2 */
				if(j==0){
					sum_average[i] = generalmath->DAQ.input_buff_voltage[i][j];
				}else{
					sum_average[i] = sum_average[i] + generalmath->DAQ.input_buff_voltage[i][j];
				}
			}
			output_average[i] = sum_average[i] / (float) generalmath->size_block;
		}
		return SME_OK;
	}
	return SME_BUSY;
}


#endif /* SME_GENERALMATH_MODULE_ENABLED */
