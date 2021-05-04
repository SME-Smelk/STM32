/**
  ******************************************************************************
  * @file    stm32f1xx_sme_ad7892.c
  * @author  Ismael Poblete V.
  * @brief   ADC AD7892 module driver.
  *          This file provides software functions to manage and use the ADC AD7829.
  *          It need GPIO HAL and SPI HAL configured.
  @verbatim
  ==============================================================================
                    ##### SME ADC AD7892 features #####
  ==============================================================================

	Use ADC AD7892 for data signals of +-10V or +-5V (AD7892-1). The input range
	depends by hardware configurations (Vin1 and Vin2).

	Sentences compatibles and tested:
	(+)FSR_PARAMETER_10V:	For +-10V Input Range, Vin2 = AGND
	(+)FSR_PARAMETER_5V :	For +-5V  Input Range, Vin1 = Vin

	Global functions for user:

	@AD7892 functions:

	(+)SME_AD7892_Init		: For initialization structure parameters SME_ADC_AD7892_t
						  	  Initialize GPIOs pins CONVST - EOC - RFS, and feed with
						  	  SPI.
	(+)SME_AD7892_Getdata	: Function to acquire data from SPI and process to a valid
						  	  voltage value.

	@Timer_Integration data acquisition functions:

	(+)SME_AD7892_DAQ_Init				: Start the TIMER acquisition and initialize
											  the buffer variable.
	(+)SME_AD7892_DAQ_data_acquisition	: Acquire TIMER data for each number of channels
	 	 	 	 	 	 	 	 	 	 	  and store to buffer input variable as a matrix
	 	 	 	 	 	 	 	 	 	 	  of number of channels and size block. It store
	 	 	 	 	 	 	 	 	 	 	  and full the buffer of input data and set
	 	 	 	 	 	 	 	 	 	 	  flag_buffdata_ready to indicate that buffer
	 	 	 	 	 	 	 	 	 	 	  is full. When it happens TIMER is STOP.
	(+)SME_AD7892_DAQ_reset_request		: Called when the full data buffer has been
										  	  processed. Reset and prepared the next
										  	  mount of data TIMER acquisition and START TIMER.
  ==============================================================================
                     ##### How to use this tools #####
  ==============================================================================

@Simple AD7892:

	(#) Set a structure type SME_AD7892_ADC_t

	(#) Set a structure type SPI_HandleTypeDef for SPI data acquisition and configuration:

		  SPI Frequency 16Mhz** optional
		  SPI_POLARITY_HIGH
		  SPI_PHASE_1EDGE
		  SPI_FIRSTBIT_MSB
		  SPI_NSS_SOFT; **No chip select

	(#) Feed the ADC functions with AD7892 and SPI structure:

		(++)  SME_AD7892_Init(&ad7892_1, &hspi2, FSR_PARAMETER_10V,
		  	  	  AD7892_1_CONVST_GPIO_Port,AD7892_1_CONVST_Pin,AD7892_1_RFS_GPIO_Port,AD7892_1_RFS_Pin,AD7892_1_EOC_GPIO_Port,AD7892_1_EOC_Pin,
		  	  	  OFFSET_P_ERR,GAIN_P_ERR,OFFSET_N_ERR,GAIN_N_ERR);

		Configuration:
			FSR_PARAMETER_10V or FSR_PARAMETER_5V

		Defines pins for ADC:
			CONVST_GPIO_Port	GPIOB
			CONVST_Pin			GPIO_PIN_1
			RFS_GPIO_Port
			RFS_Pin
			EOC_GPIO_Port
			EOC_Pin
		Defines errors:
			OFFSET_P_ERR
			GAIN_P_ERR
			OFFSET_N_ERR
			GAIN_N_ERR

	(#) Feed the ADC functions:

		(++)For process data in infinite loop execution or interrupt.
				float_variable=SME_AD7892_Getdata(&ad7892_1);

@Multiples AD7892 with TIMER acquisition:

	********* TIMER_Integration *********

	(#) Set a structure type SME_AD7892_ADC_t for parameter of AD7892

		SME_AD7892_ADC_t ad7892_1;
		SME_AD7892_ADC_t ad7892_2;

	(#) Set a structure type SME_AD7892_DAQ_t for variables for DAQ with Timer

		SME_AD7892_DAQ_t daq_data_adc;

	(#) Set a structure type TIM_HandleTypeDef. Configure without output and software interrupt.

		TIM_HandleTypeDef htim2;

	(#) Use a SME_AD7892_DAQ_Init function to start the TIMER data acquisition
		of ADC and configure the matrix buffer as a output of data.
		Use before the infinite loop transition.
		Matrix buffer: data_acq_buffer[NUMBER_ADC_CHANNELS][SIZE_BLOCK]

			SME_AD7892_DAQ_Init(&daq_data_adc,&htim2,NUMBER_ADC_CH,NUMBER_OF_DATA,ADC_K_PARAMETER);

	(#) Use SME_AD7892_Getdata function in HAL_TIM_PeriodElapsedCallback
		It acquire and store the next new data to fill the buffer. When the buffer is
		full, then it set a flag_buffdata_ready and stop the TIMER.
			***daq_data_adc.adc_buf[0][i]***

			daq_data_adc.adc_buf[0]=SME_AD7892_Getdata(&ad7892_1);
			daq_data_adc.adc_buf[1]=SME_AD7892_Getdata(&ad7892_2);

	(#) When flag_buffdata_ready is set it can used in the infinite loop for personal
		process, for use math functions or @GeneralMath_functions.

			if(daq_data_adc.flag_buffdata_ready == SET) {

				output_rms[0]=SME_GeneralMath_rms_float32(NUMBER_OF_DATA,daq_data_adc.data_acq_buffer[0]);
				output_rms[1]=SME_GeneralMath_rms_float32(NUMBER_OF_DATA,daq_data_adc.data_acq_buffer[1]);

				output_average[0]=SME_GeneralMath_average_float32(NUMBER_OF_DATA,daq_data_adc.data_acq_buffer[0]);
				output_average[1]=SME_GeneralMath_average_float32(NUMBER_OF_DATA,daq_data_adc.data_acq_buffer[1]);
				...
				...

	(#) Use SME_AD7892_DAQ_reset_request for reset and prepared the next data acquisition
		and start the TIMER for fill the buffer. Reset flag_buffdata_ready.

			if(daq_data_adc.flag_buffdata_ready == SET) {
				...
				...
				...
				SME_AD7892_DAQ_reset_request(&daq_data_adc);
			}

  ==============================================================================
                  ##### ADC AD7892 Hardware configurations #####
  ==============================================================================

   	-Data acquisition for AD7892ANZ-1 with SPI serial communication mode.

  	####################
   	AD7892-1 Pins
   	####################

		1 VDD 					- +5V
		2  ~STANDBY 			- +5V
		3 Vin2 					- GNDA
		4 Vin1 			 		- Input-Signal
		5 REF OUT/REF IN 		- Capacitor 0.1 uF to GND
		6 AGND 					- GNDA
		7 MODE 					- GNDD
		8 LOW 					- GNDD
		9 LOW 					- GNDD
		10,11,12,13 			- NC
		14 DGND 				- GNDD
		15 SDATA 				- SPI_MISO
		16 SCLK 				- SPI_SCK
		17 ~RFS 				- RFS
		18,19,20 				- NC
		21 ~RD 					- NC
		22 ~CS 					- NC
		23 ~EOC 				- EOC
		24 ~CONVST 				- CONVST

  	######################
   	Data Clock Form.
   	######################

  		SPI_SDATA	__________..___________DATA_#############_____..___ (0X00fe)
  		SPI_SCLK	############_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_######## (16MHz)
  		~RFS		############________________________________####### (12.45us)
  		~EOC		########__#########################################	(85ns)
  		~CONVST		###__############################################## (155ns)

  	######################
   	Data Form.
   	######################

	0000 0111 1111 1111 - 2047 - 0x7ff - 	+10V / +5V
	0000 0000 0000 0000 - 0	   - 0x0   - 		0V
	0000 1111 1111 1111	- 4095 - 0xfff - 		0V
	0000 1000 0000 0000	- 2048 - 0x800 - 	-10V / -5V

	4095	-		0V
	2048	-	 -10V/-5V
	2047	-	 +10V/+5V
	0		-		0V


  @endverbatim
  ******************************************************************************

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stm32f1xx_sme_AD7892.h>

#ifdef SME_ADC_AD7892_MODULE_ENABLED

/* Private function prototypes -----------------------------------------------*/

/*
 ===============================================================================
              ##### Initialization and APIs functions #####
 ===============================================================================
 */

/*
 * @brief  Function to initialize AD7892
 * @param  Pointer to a SME_ADC_AD7892_t structure that contains
 *         the configuration information for the AD7892.
 * @param  Pointer to a SPI_HandleTypeDef structure that contains
 *         the configuration information for the SPI for data receive.
 * @param  Parameter for input range configuration AD7892 @FSR_PARAMETER
 * @param  GPIO Port for CONVST
 * @param  GPIO Pin for CONVST
 * @param  GPIO Port for RFS
 * @param  GPIO Pin for RFS
 * @param  GPIO Port for EOC
 * @param  GPIO Pin for EOC
 * @param  Positive Offset Error
 * @param  Positive Gain Error
 * @param  Negative Offset Error
 * @param  Negative Gain Error
 * @retval SME Status
*/
SME_StatusTypeDef SME_AD7892_Init(SME_AD7892_ADC_t *ad7892,SPI_HandleTypeDef *hspi,uint8_t fsr_parameter,GPIO_TypeDef *convst_port,uint16_t convst_pin,GPIO_TypeDef *rfs_port,uint16_t rfs_pin,GPIO_TypeDef *eoc_port,uint16_t eoc_pin,float offset_p_err,float gain_p_err,float offset_n_err,float gain_n_err){

   	ad7892->offset_p_err = offset_p_err;
   	ad7892->gain_p_err= gain_p_err;
   	ad7892->offset_n_err = offset_n_err;
   	ad7892->gain_n_err= gain_n_err;

	ad7892->lsb_parameter = (float)fsr_parameter / 4096.0;
	ad7892->convst_port = convst_port;
	ad7892->rfs_port	= rfs_port;
	ad7892->eoc_port	= eoc_port;
	ad7892->convst_pin = convst_pin;
	ad7892->rfs_pin	= rfs_pin;
	ad7892->eoc_pin	= eoc_pin;

	/* SPI Init*/
	ad7892->hspi = hspi;

	/* Initialize GPIOs*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ad7892->rfs_port, ad7892->rfs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ad7892->convst_port, ad7892->convst_pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : EOC_Pin */
	GPIO_InitStruct.Pin = ad7892->eoc_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ad7892->eoc_port, &GPIO_InitStruct);

	/*Configure GPIO pin : RFS_Pin */
	GPIO_InitStruct.Pin = ad7892->rfs_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ad7892->rfs_port, &GPIO_InitStruct);

	/*Configure GPIO pin : CONVST_Pin */
	GPIO_InitStruct.Pin = ad7892->convst_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ad7892->convst_port, &GPIO_InitStruct);

	/* Set GPIOs */
	HAL_GPIO_WritePin(ad7892->convst_port,ad7892->convst_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(ad7892->rfs_port,ad7892->rfs_pin,GPIO_PIN_SET);

	return SME_OK;
}


/*
 * @brief  Function to get ADC voltage data of AD7892
 * @param  Pointer to a SME_ADC_AD7892_t structure that contains
 *         the configuration information for the AD7892.
 * @retval SME Status
*/
float SME_AD7892_Getdata(SME_AD7892_ADC_t *ad7892){

	  /* Init
			HCKL 64MHz - APB1 64MHz
			CLOCK,CONVST,EOC,RFS must be HIGH
			(*): Must be done by the MCU

			*CONVST: Send pulse for data
			EOC: After 1.6us, it response with a 85ns pulse (data ready)
			*RFS: Must be LOW for SPI receive transmission and HIGH when it finished.
	  */

	  /* CONVST LOW TOGGLE 140ns (30ns minimum) */
	  ad7892->convst_port->BSRR = (uint32_t)ad7892->convst_pin << 16u; //RESET
	  ad7892->convst_port->BSRR = ad7892->convst_pin;				   //SET

	  /* Wait to EOC response (1.6us) and it will be a toggle LOW 85ns.
	    Wait to pulse ends 1.56us */
	  for(uint32_t i = 0; i < 6; i++);

	  /* Transmission Start Set RFS LOW - RESET PIN */
	  ad7892->rfs_port->BSRR = (uint32_t)ad7892->rfs_pin << 16u;

	  /* Receive data SPI */
	  if(HAL_SPI_Receive(ad7892->hspi,(uint8_t*)&ad7892->spi_receive_data,1,1)){
		  return SME_ERROR;
	  }

	  /* Transmission finish Set RFS HIGH - SET PIN*/
	  ad7892->rfs_port->BSRR = ad7892->rfs_pin;

	  /* Data processing */
	  if((ad7892->spi_receive_data >= 0) && (ad7892->spi_receive_data < 2048)){
		  ad7892->data_adc = ((float)ad7892->spi_receive_data - ad7892->offset_p_err) / (ad7892->gain_p_err);
	  }else if((ad7892->spi_receive_data >= 2047) && (ad7892->spi_receive_data < 4096)){
		  ad7892->data_adc = (4095 - (float)ad7892->spi_receive_data - ad7892->offset_n_err) / (ad7892->gain_n_err) * -1;
	  }else{
		 return 0;
	  }
	  return ad7892->data_adc;
}

/**
 * @brief  Initialize parameters of daq_DMA_DAQ_HandleTypeDef parameters.
 * @param  Pointer to a daq_DMA_DAQ_HandleTypeDef structure that contains
 *         the information for daq of acquisition data
 * @param  Pointer to a ADC_HandleTypeDef structure that contains
 *         the information for the specified adc.
 * @param  Number of channels for matrix dimension [n_channel][x]
 * @param  Size of block of amount data for matrix dimension [x][size_block]
 * @param  Constant value that is multiplied to every element of the matrix data buffer.
 * @retval SME Status
 */
SME_StatusTypeDef SME_AD7892_DAQ_Init(SME_AD7892_DAQ_t *daq,TIM_HandleTypeDef *htim,uint8_t number_adc_channels,uint32_t size_block, float adc_k_parameter){

	/* SPI Timer*/
	daq->htim = htim;

	/* Initialize Parameters */
	daq->cont_databuff = 0;
	daq->flag_buffdata_ready = RESET;
	daq->size_block = size_block;
	daq->adc_k_parameter = adc_k_parameter;
	daq->number_adc_channels = number_adc_channels;

	/* Dynamic allocation for DMA ADC BUFF*/
	daq->adc_buf = calloc(daq->number_adc_channels,sizeof(float*));
	if(daq->adc_buf == NULL){
		return SME_STACKOVERFLOW;
	}

	/* Dynamic allocation for store data values
	 * Generate a dynamic matrix input_buff_voltage[number_adc_channels][size_block]
	 * */
	daq->data_acq_buffer = (float**)malloc(sizeof(float*) * daq->number_adc_channels);
	if(daq->data_acq_buffer == NULL){
		return SME_STACKOVERFLOW;
	}
	for(int i=0; i<daq->number_adc_channels;i++){
		daq->data_acq_buffer[i] = (float*) calloc(daq->size_block,sizeof(float*));
		if(daq->data_acq_buffer == NULL){
			return SME_STACKOVERFLOW;
		}
	}

	  HAL_TIM_Base_Start_IT(htim);

	return SME_OK;
}

/**
 * @brief  Acquisition ADC-DMA data multiplied for adc_k_parameter and store in matrix data buffer.
 * @param  Pointer to a daq_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for daq of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_AD7892_DAQ_data_acquisition(SME_AD7892_DAQ_t *daq){
	if(daq->flag_buffdata_ready == RESET){
		for (int i =0; i<daq->number_adc_channels; i++){
			daq->data_acq_buffer[i][daq->cont_databuff] = (float) daq->adc_buf[i] * daq->adc_k_parameter;
		}
		daq->cont_databuff++;

		if(daq->cont_databuff >= daq->size_block){
			daq->cont_databuff=0;
			daq->flag_buffdata_ready = SET;
			HAL_TIM_Base_Stop(daq->htim);
			return SME_NEWDATA;
		}
	}
	return SME_BUSY;
}

/**
 * @brief  Reset ready data flag and re-start DMA data acquisition.
 * @param  Pointer to a daq_DMA_DAQ_HandleTypeDef structure that contains
 *         the configuration information for daq of acquisition data
 * @retval SME Status
 */
SME_StatusTypeDef SME_AD7892_DAQ_reset_request(SME_AD7892_DAQ_t *daq){
	daq->flag_buffdata_ready = RESET;
	daq->cont_databuff=0;
	HAL_TIM_Base_Start_IT(daq->htim);
	return SME_OK;
}
#endif /* __STM32F1xx_SME_AD7892_H */

