/**
  ******************************************************************************
  * @Project        : ADC_00-RMS_AVERAGE_DMA
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 04-18-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : RMS and Average processing data with DMA.
  * 				  ADC Continuos Conversion mode and DMA circular mode
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PD12      	------> LED_GREEN
  * 	*ADC
  * 		PA4    	 	------> ADC1,IN4
  * 		PA5    	 	------> ADC1,IN5
  * @note
  * 	-RMS and average processing data. It need a buffer with fully data to before
  * 	 call the functions.
  * 	    -data_acquisition: Function for DMA data acquisition.
  * 		-calc_rms_float32: Calculate rms float data values from buffer full
  * 		-calc_average_float32: Calculate averages float data values from buffer full
  * 		-reset_data_request: Reset flag of ready data and start DMA ADC data acquisition
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma2;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void USART2_UART_Init(void);
static void ADC1_Init(void);
static void DMA_Init(void);

/* DMA - RMS AVERAGE */
void reset_data_request(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
void calc_average_float32(uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_average[number_adc_channels]);
void calc_rms_float32(uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_rms[number_adc_channels]);
void data_acquisition(float k_parameter, uint8_t number_adc_channels,uint32_t size_block,float input_block[number_adc_channels][size_block]);
/* Private user code ---------------------------------------------------------*/

/* Defines for ADC and RMS */
/* ADC */
#define NUMBER_ADC_CHANNELS 2

/* Mount of data to process*/
#define SIZE_RMS_BLOCK 1000
#define SIZE_AVERAGE_BLOCK 1000
#define ADC_K_PARAMETER (3.0 / 4096.0)

/* Data to store rms and average */
float output_rms[NUMBER_ADC_CHANNELS];
float average_rms[NUMBER_ADC_CHANNELS];

/*Buf for DMA - ADC channels*/
uint16_t adc_buf[NUMBER_ADC_CHANNELS];
/* Buf for store data for channels*/
float input_buff_voltage[NUMBER_ADC_CHANNELS][SIZE_RMS_BLOCK];
/*Flag to ready data*/
uint8_t flag_buffdata_ready = RESET;

/* User data*/
char msg[100];
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	GPIO_Init();
	USART2_UART_Init();
	DMA_Init();
	ADC1_Init();

	/* User Code */
	char msg[100];

	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK   : %ldHz\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1  : %ldHz\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2  : %ldHz\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);


	/* Start */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, NUMBER_ADC_CHANNELS);

	while (1)
	{
		/* Calculate RMS*/
		if(flag_buffdata_ready == SET){
			/* The data is reayd and the buffers are full*/
			calc_rms_float32(NUMBER_ADC_CHANNELS, SIZE_RMS_BLOCK,input_buff_voltage,output_rms);
			calc_average_float32(NUMBER_ADC_CHANNELS, SIZE_AVERAGE_BLOCK,input_buff_voltage,average_rms);

			/* Start DMA ofr nex data and reset flag ready*/
			reset_data_request(&hadc1, (uint32_t*)adc_buf, NUMBER_ADC_CHANNELS);

			/* Print data */
			memset(msg,0,sizeof(msg));
			sprintf(msg,"Va:%.3f,%.3f,Vb:%.3f,%.3f\r\n",output_rms[0],average_rms[0],output_rms[1],average_rms[1]);
			HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

		}
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /* Config MCO, signal output of SYSCLK in PC9 pin */
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_1);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = NUMBER_ADC_CHANNELS;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GREEN_Port, &GPIO_InitStruct);
}


/**
  * Enable DMA controller clock
  */
static void DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}


/* Called when buffer is completely filled */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	data_acquisition(ADC_K_PARAMETER,NUMBER_ADC_CHANNELS,SIZE_RMS_BLOCK,input_buff_voltage);

}

uint32_t cont_buff_adc_rms = 0;

void data_acquisition(float k_parameter, uint8_t number_adc_channels,uint32_t size_block,float input_block[number_adc_channels][size_block]){
	if(flag_buffdata_ready == RESET){
		for (int i =0; i<number_adc_channels; i++)
		{
			input_block[i][cont_buff_adc_rms] = (float)adc_buf[i] * k_parameter;
		}
		cont_buff_adc_rms++;
		if(cont_buff_adc_rms >= size_block){
			cont_buff_adc_rms=0;
			flag_buffdata_ready = SET;
			HAL_ADC_Stop_DMA(&hadc1);
		}

	}
}

void reset_data_request(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length){
	flag_buffdata_ready = RESET;
	HAL_ADC_Start_DMA(hadc, pData, Length);
}

void calc_rms_float32(uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_rms[number_adc_channels]){

	float sum_v2[number_adc_channels];

	for (int i =0; i < number_adc_channels; i++)
	{
		for (int j =0; j < block_size; j++)
		{
			/* Acquire the sum pot 2 */
			if(j==0){
				sum_v2[i] = input_block[i][j] * input_block[i][j];
			}else{
				sum_v2[i] = sum_v2[i] + input_block[i][j] * input_block[i][j];
			}
		}
		output_rms[i] = sqrt(sum_v2[i] / (float) block_size);
	}
}

void calc_average_float32(uint8_t number_adc_channels,uint32_t block_size, float input_block[number_adc_channels][block_size], float output_average[number_adc_channels]){

	float sum_average[number_adc_channels];

	for (int i =0; i < number_adc_channels; i++)
	{
		for (int j =0; j < block_size; j++)
		{
			/* Acquire the sum pot 2 */
			if(j==0){
				sum_average[i] = input_block[i][j];
			}else{
				sum_average[i] = sum_average[i] + input_block[i][j];
			}
		}
		output_average[i] = sum_average[i] / (float) block_size;
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */


}
