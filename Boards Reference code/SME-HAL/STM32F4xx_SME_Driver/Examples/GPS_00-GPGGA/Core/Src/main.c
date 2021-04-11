/**
  ******************************************************************************
  * @Project        : GPS_00-GPGGA
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 10-04-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : GPS NMEA GPGGA RX IT data reception with SME_GPS Driver.
  * 				  SME use HAL drivers.
  * @Lib			: CMSIS, HAL, SME.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		80MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART1
  * 		PA9			<-----> USART_TX
  * 		PA10		<-----> USART_RX
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * @Note:
  * 	-GPS NEO 6M
  * 	-NMEA GPGGA detected by uart rx interrupt.
  * 	-How to use:
  * 		-Set a struct type GPS_HandleTypeDef (GPS_NMEA)
  * 		-Initialize UART to GPS (GPS_UART2)
  * 		-Consider SENTENCE_GPGGA macro for GPGGA recognize
  * 		-Feed the UART and GPS struct to:
  * 			-Init function
  * 				-SME_GPS_Init(&GPS_UART2,&GPS_NMEA);
  * 			-For data interrupt adquisition in rx UART interrupt:
  * 				-SME_GPS_detecCommand(&GPS_UART2,&GPS_NMEA,SENTENCE_GPGGA);
  * 			-For process data in loop
  * 				-SME_GPS_DataProcess(&GPS_NMEA)
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart1;

#define GPS_UART2 huart2
#define DEBUG_UART1 huart1

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPS_USART2_Init(void);
static void DEBUG_USART1_Init(void);

/* Private user code ---------------------------------------------------------*/
GPS_HandleTypeDef GPS_NMEA;
char pData[100];

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
	GPS_USART2_Init();
	DEBUG_USART1_Init();
	SME_GPS_Init(&GPS_UART2,&GPS_NMEA);

	/* User Code */

	/* Start Code */
	while (1){
		switch(SME_GPS_DataProcess(&GPS_NMEA)){
		case GPS_DATA_RECEIVED:
			memset(pData,0,sizeof(pData));
			sprintf(pData,"GPS:%d:%d:%d,%f,%f,%d,%d,%f\n",
				GPS_NMEA.GPGGA.utc_time[0],
				GPS_NMEA.GPGGA.utc_time[1],
				GPS_NMEA.GPGGA.utc_time[2],
				GPS_NMEA.GPGGA.latitude,
				GPS_NMEA.GPGGA.longitude,
				GPS_NMEA.GPGGA.fix_quality,
				GPS_NMEA.GPGGA.num_sattelites,
				GPS_NMEA.GPGGA.altitude
			);
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);

			break;
		case GPS_ERROR:
			memset(pData,0,sizeof(pData));
			sprintf(pData,"GPS No Fix\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);
			break;
		case GPS_BUF_NO_MEMORY:
			memset(pData,0,sizeof(pData));
			sprintf(pData,"GPS Mem buff err\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);
			break;
		default:
			break;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

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
static void GPS_USART2_Init(void)
{
  GPS_UART2.Instance = USART2;
  GPS_UART2.Init.BaudRate = 9600;
  GPS_UART2.Init.WordLength = UART_WORDLENGTH_8B;
  GPS_UART2.Init.StopBits = UART_STOPBITS_1;
  GPS_UART2.Init.Parity = UART_PARITY_NONE;
  GPS_UART2.Init.Mode = UART_MODE_TX_RX;
  GPS_UART2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  GPS_UART2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&GPS_UART2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void DEBUG_USART1_Init(void)
{
	DEBUG_UART1.Instance = USART1;
	DEBUG_UART1.Init.BaudRate = 115200;
	DEBUG_UART1.Init.WordLength = UART_WORDLENGTH_8B;
	DEBUG_UART1.Init.StopBits = UART_STOPBITS_1;
	DEBUG_UART1.Init.Parity = UART_PARITY_NONE;
	DEBUG_UART1.Init.Mode = UART_MODE_TX_RX;
	DEBUG_UART1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	DEBUG_UART1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&DEBUG_UART1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CallBack UART
  * @param None
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	SME_GPS_detecCommand(&GPS_UART2,&GPS_NMEA,SENTENCE_GPGGA);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}

