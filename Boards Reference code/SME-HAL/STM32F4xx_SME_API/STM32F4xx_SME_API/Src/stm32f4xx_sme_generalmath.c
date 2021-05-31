/**
  ******************************************************************************
  * @file    stm32f4xx_sme_generalmath.c
  * @author  Ismael Poblete V.
  * @brief   SME utility functions of general math.
  *          This file provides software functions:
  *          	-To manage the DMA data acquisition of amount of data from ADC.
  *          	-Math functions to process a data block.
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
											  considering the size of block of data for
											  each one. The output is a float value.
	(+)SME_GeneralMath_average_float32		: Compute the average from data input matrix,
											  considering the size of block of data for
											  each one. The output is a float value.

	********* Private functions *********
	(*) Without private functions

  ==============================================================================

                     ##### How to use this tools #####

  ==============================================================================

	********* DMA_Integration *********

	(#) Set a structure type SME GeneralMath_DMA_DAQ_HandleTypeDef

	(#) Set a structure type ADC_HandleTypeDef and DMA_HandleTypeDef. In DMA circular mode.

	(#) Use a SME_GeneralMath_DMA_Start function to start the DMA data acquisition
		of a ADC and configure the matrix buffer as a output of data.
		Use before the infinite loop transition.
		Matrix buffer: data_acq_buffer[NUMBER_ADC_CHANNELS][SIZE_BLOCK]

			SME_GeneralMath_DMA_Start(&GeneralMath_data,&hadc1,NUMBER_ADC_CHANNELS,SIZE_BLOCK,ADC_K_PARAMETER);

	(#) Use SME_GeneralMath_DMA_data_acquisition function in HAL_ADC_ConvCpltCallback
		It acquire and store the next new data to fill the buffer. When the buffer is
		full, then it set a flag_buffdata_ready and stop the DMA.

			SME_GeneralMath_DMA_data_acquisition(&GeneralMath_data);

	(#) When flag_buffdata_ready is set it can used in the infinite loop for personal
		process, for use math functions or @GeneralMath_functions.

			if(GeneralMath_data.flag_buffdata_ready == SET) {

				output_rms[0]=SME_GeneralMath_rms_float32(SIZE_BLOCK,GeneralMath_data.data_acq_buffer[0]);
				output_rms[1]=SME_GeneralMath_rms_float32(SIZE_BLOCK,GeneralMath_data.data_acq_buffer[1]);

				output_average[0]=SME_GeneralMath_average_float32(SIZE_BLOCK,GeneralMath_data.data_acq_buffer[0]);
				output_average[1]=SME_GeneralMath_average_float32(SIZE_BLOCK,GeneralMath_data.data_acq_buffer[1]);
				...
				...

	(#) Use SME_GeneralMath_DMA_reset_request for reset and prepared the next data acquisition
		and start the DMA for fill the buffer. Reset flag_buffdata_ready.

			if(GeneralMath_data.flag_buffdata_ready == SET) {
				...
				...
				...
				SME_GeneralMath_reset_dma_request(&GeneralMath_DMA_DAQ);
			}

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
 * @brief  Initialize parameters of GeneralMath_DMA_DAQ_HandleTypeDef parameters.
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the information for GeneralMath of acquisition data
 * @param  Pointer to a ADC_HandleTypeDef structure that contains
 *         the information for the specified adc.
 * @param  Number of channels for matrix dimension [n_channel][x]
 * @param  Size of block of amount data for matrix dimension [x][size_block]
 * @param  Constant value that is multiplied to every element of the matrix data buffer.
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_Start(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath,ADC_HandleTypeDef* hadc,uint8_t number_adc_channels,uint32_t size_block, float adc_k_parameter){

	/* Initialize Parameters */
	generalmath->cont_databuff = 0;
	generalmath->flag_buffdata_ready = RESET;
	generalmath->size_block = size_block;
	generalmath->adc_k_parameter = adc_k_parameter;
	generalmath->number_adc_channels = number_adc_channels;
	generalmath->adc_handler = hadc;

	/* Dynamic allocation for DMA ADC BUFF*/
	generalmath->adc_dma_buf = calloc(generalmath->number_adc_channels,sizeof(float*));
	if(generalmath->adc_dma_buf == NULL){
		return SME_STACKOVERFLOW;
	}

	/* Dynamic allocation for store data values
	 * Generate a dynamic matrix input_buff_voltage[number_adc_channels][size_block]
	 * */
	generalmath->data_acq_buffer = (float**)malloc(sizeof(float*) * generalmath->number_adc_channels);
	if(generalmath->data_acq_buffer == NULL){
		return SME_STACKOVERFLOW;
	}
	for(int i=0; i<generalmath->number_adc_channels;i++){
		generalmath->data_acq_buffer[i] = (float*) calloc(generalmath->size_block,sizeof(float*));
		if(generalmath->data_acq_buffer == NULL){
			return SME_STACKOVERFLOW;
		}
	}

	/* Start DMA */
	if(HAL_ADC_Start_DMA(generalmath->adc_handler, (uint32_t*)generalmath->adc_dma_buf, generalmath->number_adc_channels) == HAL_ERROR){
		return SME_ERROR;
	}

	return SME_OK;
}

/**
 * @brief  Acquisition ADC-DMA data multiplied for adc_k_parameter and store in matrix data buffer.
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for GeneralMath of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_data_acquisition(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	if(generalmath->flag_buffdata_ready == RESET){
		for (int i =0; i<generalmath->number_adc_channels; i++){
			generalmath->data_acq_buffer[i][generalmath->cont_databuff] = (float) generalmath->adc_dma_buf[i] * generalmath->adc_k_parameter;
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
 * @brief  Reset ready data flag and re-start DMA data acquisition.
 * @param  Pointer to a GeneralMath_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for GeneralMath of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_GeneralMath_DMA_reset_request(GeneralMath_DMA_DAQ_HandleTypeDef *generalmath){
	generalmath->flag_buffdata_ready = RESET;
	generalmath->cont_databuff=0;

	if(HAL_ADC_Start_DMA(generalmath->adc_handler, (uint32_t*)generalmath->adc_dma_buf, generalmath->number_adc_channels) == HAL_ERROR){
		return SME_ERROR;
	}

	return SME_OK;
}

/**
 * @brief  Function to calculate the RMS value from a block of data.
 * @param  Size of block of amount data to process
 * @param  Pointer to the matrix of input data
 * @retval Result of RMS value as a float value
*/
float SME_GeneralMath_rms_float32(uint32_t size_block, float *input_buff_voltage){
	float sum_v2;
	for (int i =0; i < size_block; i++)
	{
		/* Acquire the sum pot 2 */
		if(i==0){
			sum_v2 = input_buff_voltage[i] * input_buff_voltage[i];
		}else{
			sum_v2 = sum_v2 + input_buff_voltage[i] * input_buff_voltage[i];
		}
	}
	return sqrt(sum_v2 / (float) size_block);
}

/**
 * @brief  Function to calculate the Average value from a block of data.
 * @param  Size of block of amount data to process
 * @param  Pointer to the matrix of input data
 * @retval Result of RMS value as a float value
*/
float SME_GeneralMath_average_float32(uint32_t size_block, float *input_buff_voltage){

		float sum_average;

		for (int i =0; i < size_block; i++)
		{
			/* Acquire the sum pot 2 */
			if(i==0){
				sum_average = input_buff_voltage[i];
			}else{
				sum_average = sum_average + input_buff_voltage[i];
			}
		}
		return (sum_average / (float) size_block);
}


#endif /* SME_GENERALMATH_MODULE_ENABLED */
