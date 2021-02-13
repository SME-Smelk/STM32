/**
  ******************************************************************************
  * @Proyecto       : 02-UART2_TXRX
  * @Autor          : Ismael Poblete
  * @Date         	: 6-28-2020
  * @brief          : Programa que recepciona datos y lo transmite de vuelta
  * 				  via UART2. Usar COMx del PC
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		8MHz
  * @Perf
  * 	*UART2
  * 		PA2     ------> USART2_TX
  * 		PA3     ------> USART2_RX
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);

/* Private user code ---------------------------------------------------------*/
char *user_data = "The application is running\r\n";

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
  MX_USART2_UART_Init();

  /* User Code */
  uint16_t len_of_data = strlen(user_data);
  HAL_UART_Transmit(&huart2,(uint8_t*)user_data,len_of_data,HAL_MAX_DELAY);

  uint8_t rcvd_data;
  uint8_t data_buffer[100];
  uint32_t count=0;

  /* Infinite loop */
  while (1){
	  while (1)
	  {

		HAL_UART_Receive(&huart2,&rcvd_data,1,HAL_MAX_DELAY); //Tiene un while, modo bloqueante. Si hay recepcion avanza.
		if(rcvd_data == '\r')
		{
			break;
		}
		else
		{
			data_buffer[count++]= rcvd_data;
		}

	  }

	  data_buffer[count++]= '\r';
	  HAL_UART_Transmit(&huart2,data_buffer,count,HAL_MAX_DELAY);
	  count=0;
	  memset(data_buffer, 0, sizeof(data_buffer));
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
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
	while(1);
}
