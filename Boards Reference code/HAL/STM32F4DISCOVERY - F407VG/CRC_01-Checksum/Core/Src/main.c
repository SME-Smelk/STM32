/**
  ******************************************************************************
  * @Project        : CRC_01-Checksum
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-26-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Use CRC with UART like a receiver data.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * 	*GPIO
  * 		PD12      	------> LD4 Green
  * 		PD13      	------> LD3 Orange
  * 		PD14      	------> LD5 Red
  * 		PD15      	------> LD6 Blue
  * 		PA0     	<------ User Button
  *@note
  *		-UART2 if the receive data is 05 51 E7 E9 AB 7C then the
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void USART2_Init(void);
void CRC_Init(void);

void printmsg(char *format,...);

void bootloader_jump_to_user_app(void);
void bootloader_uart_read_data(void);

/* Private user code ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
CRC_HandleTypeDef hcrc;

/*Commands */

#define RX_LEN 6
uint8_t rx_buffer[RX_LEN];

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

	/* Configure the peripherals */
	USART2_Init();
	CRC_Init();

	printmsg("Data via HOST with CRC. Waiting...\n");
	int8_t rcv_len=0;

	/* Start Code */
	while(1){

		/* Example Receive transmission.
		* 05 51 E7 E9 AB 7C
		* Length: 05
		* Data:   51
		* CRC:    E7 E9 AB 7C
		* */
		memset(rx_buffer,0,RX_LEN);
		/*here we will read and decode the commands coming from host
		first read only one byte from the host , which is the "length" field of the command packet*/
		HAL_UART_Receive(&huart2,rx_buffer,1,HAL_MAX_DELAY);
		rcv_len= rx_buffer[0];
		HAL_UART_Receive(&huart2,&rx_buffer[1],rcv_len,HAL_MAX_DELAY);

		/*Total length of the command packet
		* Length of receive rx_buffer[0] + 1
		* Lenght data without CRC: rx_buffer[0] + 1 - 4
		*/
		uint32_t command_packet_len = (rx_buffer[0] + 1 ) - 4 ;
		//extract the CRC32 sent by the Host
		uint32_t host_crc = *((uint32_t * ) (rx_buffer+command_packet_len) );
		if (! bootloader_verify_crc(&rx_buffer[0],command_packet_len,host_crc))
		{
		  printmsg("Checksum success!!\n");

		}else
		{
		  printmsg("Checksum fail...\n");
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

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;


  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Config MCO, signal output of SYSCLK in PC9 pin */
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_1);
}

void USART2_Init(void)
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

/* prints formatted string to console over UART */
 void printmsg(char *format,...)
{
#ifdef BL_DEBUG_MSG_EN
	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);
#endif
 }


void CRC_Init(void)
{

  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }

}


//This verifies the CRC of the given buffer in pData .
uint8_t bootloader_verify_crc (uint8_t *pData, uint32_t len, uint32_t crc_host)
{
    uint32_t uwCRCValue=0xff;

    for (uint32_t i=0 ; i < len ; i++)
	{
        uint32_t i_data = pData[i];
        uwCRCValue = HAL_CRC_Accumulate(&hcrc, &i_data, 1);
	}

	 /* Reset CRC Calculation Unit */
  __HAL_CRC_DR_RESET(&hcrc);

	if( uwCRCValue == crc_host)
	{
		return VERIFY_CRC_SUCCESS;
	}

	return VERIFY_CRC_FAIL;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */

}
