/**
  ******************************************************************************
  * @file    stm32f4xx_sme_generalmath.c
  * @author  Ismael Poblete V.
  * @brief   SME utility functions of general math.
  *          This file provides software functions:
  *          	-To manage the DMA data acquisition of amount of data of ADC.
  *          	-Math functions from block amount of data
  *          		-RMS
  *          		-Average
  @verbatim
  ==============================================================================

                    	##### SME GeneralMath features #####

  ==============================================================================

	Use SME GeneralMath for start a DMA data acquisition for a numbers of channels
	with a size block for each one. Its provided a functions math with the same format:
	matrix of numbers of channels and size block for data.

	*******Sentences compatibles and tested*******
	(*) Without macros or sentences

	******Global functions for user*******

	@DMA_Integration data acquisition functions:

	(+)SME_GeneralMath_DMA_Start			: Start the DMA acquisition and initialize
											  the buffer variable.
	(+)SME_GeneralMath_DMA_data_acquisition	: Acquire DMA data for each number of channels
	 	 	 	 	 	 	 	 	 	 	  and store to buffer input variable as a matrix
	 	 	 	 	 	 	 	 	 	 	  of number of channels and size block. It store
	 	 	 	 	 	 	 	 	 	 	  and full the buffer of input data and set
	 	 	 	 	 	 	 	 	 	 	  flag_buffdata_ready to indicate that buffer
	 	 	 	 	 	 	 	 	 	 	  is full. When it happens DMA is STOP.
	(+)SME_GeneralMath_DMA_reset_request	: Called when the full data buffer has been
											  processed. Reset and prepared the next
											  mount of data DMA acquisition and START DMA.
	@GeneralMath_functions:

	(+)SME_GeneralMath_rms_float32			: Compute the RMS from data input matrix,
											  considering number of channels and size
											  of block of data for each one.
											  The output is a array of float value.
	(+)SME_GeneralMath_average_float32		: Compute the average from data input matrix,
											  considering number of channels and size
											  of block of data for each one.
											  The output is a array of float value.

	********* Private functions *********
	(*) Without private functions

  ==============================================================================

                     ##### How to use this tools #####

  ==============================================================================

	********* DMA_Integration *********

	(#) Set a structure type SME GeneralMath_DMA_DAQ_HandleTypeDef

	(#) Set a structure type ADC_HandleTypeDef and DMA_HandleTypeDef. In DMA circular mode.

	(#) Use a SME_GeneralMath_DMA_Start function to start the DMA data acquisition
		of a ADC. Use before the infinite loop transition.

			SME_GeneralMath_DMA_Start(&GeneralMath_DMA_DAQ, &hadc1, SIZE_RMS_BLOCK, NUMBER_ADC_CHANNELS,ADC_K_PARAMETER);


	(#) Use SME_GeneralMath_DMA_data_acquisition function in HAL_ADC_ConvCpltCallback
		It acquire and store the next new data to fill the buffer. When the buffer is
		full, then it set a flag_buffdata_ready and stop the DMA.

			SME_GeneralMath_DMA_data_acquisition(&GeneralMath_DMA_DAQ);

	(#) When flag_buffdata_ready is set it can used in the infinite loop for personal
		process, for use math functions or @GeneralMath_functions.

			SME_GeneralMath_rms_float32(NUMBER_ADC_CHANNELS,SIZE_RMS_BLOCK,GeneralMath_DMA_DAQ.input_buff_voltage,output_rms);
			SME_GeneralMath_average_float32(NUMBER_ADC_CHANNELS,SIZE_RMS_BLOCK,GeneralMath_DMA_DAQ.input_buff_voltage,output_average);

	(#) Use SME_GeneralMath_DMA_reset_request for reset and prepared the next data acquisition
		and start the DMA for fill the buffer. Reset flag_buffdata_ready.

			SME_GeneralMath_reset_dma_request(&GeneralMath_DMA_DAQ);

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
              	  	  	 ##### Utility Functions #####
 ===============================================================================
 */

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the information for GeneralMath of acquisition data
 * @param  Pointer to a ADC_HandleTypeDef structure that contains
 *         the information for the specified adc.
 * @param  Size of block of amount data for matrix dimension [x][size_block]
 * @param  Number of channels for matrix dimension [n_channel][x]
 * @param  float array to send the output data for each channel.
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_Start(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc, uint32_t size_block, uint32_t number_channels, float adc_k_parameter){
	generalmath->cont_databuff=0;
	generalmath->adc_handler=hadc;
	generalmath->size_block=size_block;
	generalmath->flag_buffdata_ready = RESET;
	generalmath->adc_k_parameter = adc_k_parameter;
	generalmath->number_channels = number_channels;

	generalmath->adc_buf = malloc(generalmath->number_channels);

	generalmath->input_buff_voltage = malloc(generalmath->number_channels);
	for(int i = 0; i < generalmath->number_channels;i++){
		generalmath->input_buff_voltage[i] = malloc(generalmath->size_block);
	}

	if(HAL_ADC_Start_DMA(hadc, (uint32_t*)generalmath->adc_buf, generalmath->number_channels) == HAL_ERROR){
		return SME_ERROR;
	}
	return SME_OK;
}

/**
 * @brief  Function to acquisition DMA data for n_channels of data in matrix with length of size_block
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for GeneralMath of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_data_acquisition(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	if(generalmath->flag_buffdata_ready == RESET){

		for (int i = 0; i < generalmath->number_channels; i++)
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
 * @brief  Reset the ready data flag, reassign memory to data matrix and start DMA data acquisition.
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for GeneralMath of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_reset_request(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	generalmath->flag_buffdata_ready = RESET;
/*	free(generalmath->adc_buf);
	for(int i = 0; i < generalmath->number_channels;i++){
		free(generalmath->input_buff_voltage[i]);
	}

	generalmath->input_buff_voltage = malloc(generalmath->number_channels);
	for(int i = 0; i < generalmath->number_channels;i++){
		generalmath->input_buff_voltage[i] = malloc(generalmath->size_block);
	}
*/
	if(HAL_ADC_Start_DMA(generalmath->adc_handler, (uint32_t*)generalmath->adc_buf, generalmath->number_channels) == HAL_ERROR){
		return SME_ERROR;
	}
	return SME_OK;
}

/**
 * @brief  Function to calculate the rms value of a block of data to a variable.
 * @param  Number of channels for matrix dimension [n_channel][x]
 * @param  Size of block of amount data for matrix dimension [x][size_block]
 * @param  Pointer to matrix of input data
 * @param  Float output variable array of number of channels
 * @retval SME Status
*/
SME_StatusTypeDef SME_GeneralMath_rms_float32(uint32_t number_channels, uint32_t size_block, float **input_buff_voltage,float output_rms[number_channels]){
		float sum_v2[number_channels];

		for (int i =0; i < number_channels; i++)
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
 * @brief  Function to calculate the average value of a block of data to a variable.
 * @param  Number of channels for matrix dimension [n_channel][x]
 * @param  Size of block of amount data for matrix dimension [x][size_block]
 * @param  Pointer to matrix of input data
 * @param  Float output variable array of number of channels
 * @retval SME Status
*/
SME_StatusTypeDef SME_GeneralMath_average_float32(uint32_t number_channels, uint32_t size_block, float **input_buff_voltage,float output_average[number_channels]){

		float sum_average[number_channels];

		for (int i =0; i < number_channels; i++)
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
