/**
  ******************************************************************************
  * @Project        : AD7893-1_00_Simple50KHz
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 04-27-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Simple ADC AD7891-1 SPI Serial Communication
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSI)
  * 	SYSCLK: 		64MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PA5      	------> LED
  * 	*TIMER2 (Software 50khz - 20us)
  * 	*SPI-ADC 16MHz
  *			PB14	->	SDATA (SPI2_MISO)
  *			PB13	->	SCLK  (SPI2_SCK)
  * 		PB2		->	RFS
  * 		PB11	->	EOC
  * 		PB1		->	CONVST
  *
  * @note
  * 	-Data acquisition for AD7892ANZ-1 with SPI serial communication mode.
  * 	-Timer as a 50Khz - 20us for data acquisition.
  * 	-LD2 Led to observe the interrupt timer transition
  *
  *		####################
  * 	AD7892-1 Pins
  * 	####################
  *
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
		15 SDATA 				- MISO
		16 SCLK 				- SCK
		17 ~RFS 				- RFS
		18,19,20 				- NC
		21 ~RD 					- NC
		22 ~CS 					- NC
		23 ~EOC 				- EOC
		24 ~CONVST 				- CONVST
  *
  *		######################
  * 	Data Clock Form.
  * 	######################
  *
  *		SDATA	__________..___________DATA_#############_____..___ (0Xfe)
  *		SCLK	############_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_######## (16MHz)
  *		~RFS	############________________________________####### (12.54us)
  *		~EOC	########__#########################################	(85ns)
  *		~CONVST	###__############################################## (140us)
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stdio.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void USART2_UART_Init(void);
static void SPI2_Init(void);
static void TIM2_Init(void);

void AD7892_Init(void);
void AD7892_Getdata(void);

/* Private user code ---------------------------------------------------------*/

char msg[100];
uint8_t spidata = 0xFA;
uint16_t spi_receive_data;
uint16_t data =0;
float data_adc = 0.0;
#define FSR_PARAMETER_10 	20.0
#define LSB_PARAMETER 		FSR_PARAMETER_10 / 4096

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  GPIO_Init();
  AD7892_Init();
  USART2_UART_Init();
  SPI2_Init();
  TIM2_Init();
  HAL_TIM_Base_Start_IT(&htim2);

  /* Infinite loop */
  while (1)
  {
	  /* Test the interrupt cycle - RESET*/
	  LD2_GPIO_Port->BSRR = (uint32_t)LD2_Pin << 16u;

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void SPI2_Init(void)
{
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */

static void TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = TIMER2_PRESCALER;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIMER2_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Timer callback
  * @param htim
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	/* Test the interrupt cycle - SET*/
	LD2_GPIO_Port->BSRR = LD2_Pin;

	AD7892_Getdata();
}

void AD7892_Init(void){
	/* Initialize GPIOs*/
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(RFS_GPIO_Port, RFS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : EOC_Pin */
	GPIO_InitStruct.Pin = EOC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(EOC_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : RFS_Pin */
	GPIO_InitStruct.Pin = RFS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(RFS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CONVST_Pin */
	GPIO_InitStruct.Pin = CONVST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(CONVST_GPIO_Port, &GPIO_InitStruct);

	/* Set GPIOs */
	HAL_GPIO_WritePin(CONVST_GPIO_Port,CONVST_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(RFS_GPIO_Port,RFS_Pin,GPIO_PIN_SET);
}

void AD7892_Getdata(void){

	  /* Init
			HCKL 64MHz - APB1 64MHz
			CLOCK,CONVST,EOC,RFS must be HIGH
			(*): Must be done by the MCU

			*CONVST: Send pulse for data
			EOC: After 1.6us, it response with a 85ns pulse (data ready)
			*RFS: Must be LOW for SPI receive transmission and HIGH when it finished.
	  */

	  /* CONVST LOW TOGGLE 140ns (30ns minimum) */
	  CONVST_GPIO_Port->BSRR = (uint32_t)CONVST_Pin << 16u; //RESET
	  CONVST_GPIO_Port->BSRR = CONVST_Pin;					//SET

	  /* Wait to EOC response (1.6us) and it will be a toggle LOW 85ns.
	    Wait to pulse ends 1.56us */
	  for(uint32_t i = 0; i < 6; i++);

	  /* Start transmission Set RFS LOW - RESET PIN */
	  RFS_GPIO_Port->BSRR = (uint32_t)RFS_Pin << 16u;

	  /* Receive data SPI */
	  HAL_SPI_Receive(&hspi2,(uint8_t*)&spi_receive_data,1,1);

	  /* Finish of transmission Set RFS HIGH - SET PIN*/
	  RFS_GPIO_Port->BSRR = RFS_Pin;

	  /* Data processing */
	  if(spi_receive_data < 2048){
		  data_adc = (float)spi_receive_data * LSB_PARAMETER;
	  }else if(spi_receive_data > 2047){
		  data_adc = ((4095 - (float)spi_receive_data) * LSB_PARAMETER * -1);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
