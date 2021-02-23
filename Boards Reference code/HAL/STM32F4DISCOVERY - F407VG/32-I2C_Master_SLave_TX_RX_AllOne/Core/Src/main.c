/**
  ******************************************************************************
  * @Project        : 32-I2C_Master_Slave_TX_RX_AllOne
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-22-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : I2C Master and Slave programming.
  * 				  Master Write and Read from Slave.
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
  * 		PD13      	------> LED_RED
  * 	*I2C1 100khz
  * 		PB6			<-----> I2C1_SCL
  * 		PB7			<-----> I2C1_SDA
  * @note
  * 	-In this program we tested the I2C Master - Slave have. We have a two macros
  * 	 in main.h to config this program.
  * 	 -To convert this device a Slave comment MASTER_DEVICE
  * 	 -To convert this device a Master descomment MASTER_DEVICE
  * 	 	If we want to enable the program to write descomment MASTER_WRITE.
  * 	 	If we want to enable the program to read comment MASTER_WRITE.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void I2C1_Init(void);
static void GPIO_Init(void);
static void I2C1_Init(void);
static void USART2_UART_Init(void);

/* Private user code ---------------------------------------------------------*/
uint8_t master_tx_buffer[5]={0xa5, 0x55, 0xa5, 0x55, 0xb0};
uint8_t master_rx_buffer[5];

uint8_t slave_tx_buffer[5]={0xa5, 0x55, 0xa5, 0x55, 0xc0};
uint8_t slave_rx_buffer[5];

uint8_t master_write_req;
uint8_t master_read_req;

uint8_t slave_rcv_cmd;

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
	I2C1_Init();
	USART2_UART_Init();

	while(!HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin));
	if((HAL_I2C_IsDeviceReady(&hi2c1, SLAVE_ADDRESS, 10, 10) == HAL_OK)){
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	}

	while (1)
	{
		#ifdef MASTER_DEVICE

		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

		while(!HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin));

		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

		/********** MASTER: WRITE TX TO SLAVE *************/
		#ifdef MASTER_WRITE

		master_write_req = MASTER_WRITE_CMD;
		HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, (uint8_t*) &master_write_req, 1, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		/* Now send the number of bytes to be written */
		master_write_req = WRITE_LEN;
		HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, (uint8_t*) &master_write_req, 1, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		/* NOW send the data stream */
		HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, master_tx_buffer, WRITE_LEN, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		#else
		/************ MASTER: READ TX FROM SLAVE ************/

		/* first send the master read cmd to slave */
		master_read_req = MASTER_READ_CMD;
		HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, (uint8_t*) &master_read_req, 1, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		/* Now send the number of bytes to be read */
		master_write_req = READ_LEN;
		HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, (uint8_t*) &master_read_req, 1, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		/* NOW read the data stream */
		memset(master_rx_buffer,0, 5);
		HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS, (uint8_t*) &master_rx_buffer, READ_LEN, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		for(int i=0;i<10;i++){
		  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		  HAL_Delay(200);
		  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		  HAL_Delay(200);
		}

		#endif

		#else

		/**********SLAVE: WRITE TX TO MASTER *************/

		/* first rcv the commmand from the master */
		HAL_I2C_Slave_Receive(&hi2c1, &slave_rcv_cmd, 1, HAL_MAX_DELAY);
		while(hi2c1.State != HAL_I2C_STATE_READY);

		if(slave_rcv_cmd == MASTER_WRITE_CMD){

		  /* prepare to rcv from the master
		   * first rcv no bytes to be written by master*/
		  HAL_I2C_Slave_Receive(&hi2c1, &slave_rcv_cmd, 1, HAL_MAX_DELAY);
		  while(hi2c1.State != HAL_I2C_STATE_READY);

		  memset(slave_rx_buffer,0, sizeof(slave_rx_buffer));
		  HAL_I2C_Slave_Receive(&hi2c1, slave_rx_buffer, slave_rcv_cmd, HAL_MAX_DELAY);
		  while(hi2c1.State != HAL_I2C_STATE_READY);

		  for(int i=0;i<10;i++){
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			  HAL_Delay(100);
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			  HAL_Delay(100);
		  }

		}

		if(slave_rcv_cmd == MASTER_READ_CMD){

		  HAL_I2C_Slave_Receive(&hi2c1, &slave_rcv_cmd, 1, HAL_MAX_DELAY);
		  while(hi2c1.State != HAL_I2C_STATE_READY);

		  /* NOW send the data stream */
		  HAL_I2C_Slave_Transmit(&hi2c1, slave_tx_buffer, slave_rcv_cmd, HAL_MAX_DELAY);
		  while(hi2c1.State != HAL_I2C_STATE_READY);

		  for(int i=0;i<10;i++){
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			  HAL_Delay(100);
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			  HAL_Delay(100);
		  }

		}
		#endif
	}
	return 0;

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
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
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_GREEN_Pin|LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
#ifdef MASTER_DEVICE
  hi2c1.Init.OwnAddress1 = 0;
#else
  hi2c1.Init.OwnAddress1 = SLAVE_ADDRESS;
#endif
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
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
