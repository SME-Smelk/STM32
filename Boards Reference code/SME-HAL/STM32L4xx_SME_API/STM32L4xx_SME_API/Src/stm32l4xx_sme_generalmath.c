/**
  ******************************************************************************
  * @file    stm32l4xx_sme_generalmath.c
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

	@GeneralMath_functions:

	(+)SME_GeneralMath_rms_float32			: Compute the RMS from data input matrix,
											  considering the size of block of data for
											  each one. The output is a float value.
	(+)SME_GeneralMath_average_float32		: Compute the average from data input matrix,
											  considering the size of block of data for
											  each one. The output is a float value.

	********* Private functions *********
	(*) Without private functions

  @endverbatim
  ******************************************************************************

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_sme_generalmath.h"

#ifdef SME_GENERALMATH_MODULE_ENABLED

/* Private function prototypes -----------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/
/*
 ===============================================================================
              	  	  	 ##### Utility Functions #####
 ===============================================================================
 */

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
