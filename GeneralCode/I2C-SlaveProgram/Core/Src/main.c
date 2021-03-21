/**
  ******************************************************************************
  * @Project        : I2C-SlaveProgram
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 03-21-2021
  * @Target			: NUCLEO-F411RE
  * @brief          : Program the MCU as a slave I2C1 and implement commands like
  * 				  id, version, data read, write to parameter.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSI)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*I2C
  * 		PB6     ------> I2C1_SCL
  *  		PB7     ------> I2C1_SDA
  * @note
  * -I2C1 functionality is through interrupt and sequential data.
  * -Slave Mode it waiting for master commands.
  *
  * -Slave identification:
  * 	SLAVE_ADDRESS 0x28
  * 	ID:			  0xE0
  * 	Version:	  1.0
  * -Commands from master:
  * 	Read cmd:
  *			CHECK_STATUS_CMD       	0xC1, Send a simple command of 1 byte
  *			READ_ID_VER_CMD        	0XC2, Send the id and version in 4 bytes.
  *			READ_DATA_CMD        	0XC3, Send 12 bytes of data, three float variables.
  *			RESET_SLAVE_CMD			0xC5, Send 1 byte command to reset the board by software
  *		Write cmd:
  *			WRITE_TO_PARAMETER_CMD  0xC4, Send 1 byte cmd + 4 bytes of data for parameter
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void I2C1_Init(void);
float ieee_float(uint32_t f);
/* Private user code ---------------------------------------------------------*/

//General buffer to receive a command
uint8_t slave_rcv_cmd;

// General buffer to transmit
uint8_t transmitBuffer[5];
uint8_t receiveBuffer[5];
uint32_t data_receive;
float variable_to_write = 0.0;

//Data variables
float data[3] = {0.0,0.0,0.0};


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
  I2C1_Init();

  HAL_I2C_EnableListen_IT(&hi2c1);


  /* Infinite loop */
  while (1)
  {

	  /*Simulate a data acquisition each 1 sec*/
	  if(data[0] > 100.0){
		  data[0]=0.0;
	  }
	  if(data[1] > 100.0){
		  data[1]=0.0;
	  }
	  if(data[2] > 100){
		  data[2]=0.0;
	  }

	  data[0] = data[0] + 0.1;
	  data[1] = data[1] + 0.2;
	  data[2] = data[2] + 1;
	  HAL_Delay(1000);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = OWN_ADDRESS;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

uint8_t first = SET;
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(slave_rcv_cmd == WRITE_TO_PARAMETER_CMD){
		if(first == SET){
			first = RESET;
		}else{
			data_receive = receiveBuffer[4] + (receiveBuffer[3]<< 8) + (receiveBuffer[2]<<16) + (receiveBuffer[1]<<24);
			variable_to_write = ieee_float(data_receive);
			first = SET;
		}
	}else if(slave_rcv_cmd == RESET_SLAVE_CMD){
		NVIC_SystemReset();
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{


}


int i = 0;


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	i++;
	if (TransferDirection == I2C_DIRECTION_TRANSMIT) {

		if (HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &slave_rcv_cmd, 1, I2C_FIRST_FRAME) != HAL_OK) {
			Error_Handler();
		}

		if(slave_rcv_cmd == WRITE_TO_PARAMETER_CMD){
			if(first == RESET){
				if (HAL_I2C_Slave_Sequential_Receive_IT(hi2c, (uint8_t*)&receiveBuffer, LEN_WRITE_TO_PARAMETER_CMD, I2C_LAST_FRAME) != HAL_OK) {
					Error_Handler();
				}
			}
		}

	//I2C_DIRECTION_RECEIVE
	}else{
		switch(slave_rcv_cmd){
		case CHECK_STATUS_CMD:

			memset(transmitBuffer,0, sizeof(transmitBuffer));
			transmitBuffer[0] = 0xFE;
			if (HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, &transmitBuffer[0], LEN_CHECK_STATUS_CMD, I2C_LAST_FRAME) != HAL_OK) {
				// Error here!!! (HAL_BUSY)
				Error_Handler();
			}

			break;
		case READ_ID_VER_CMD:

			memset(transmitBuffer,0, sizeof(transmitBuffer));
			transmitBuffer[0] = (uint8_t)SLAVE_ID;
			transmitBuffer[1] = SLAVE_VER[0];
			transmitBuffer[2] = SLAVE_VER[1];
			transmitBuffer[3] = SLAVE_VER[2];

			if (HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, (uint8_t*)transmitBuffer, LEN_READ_ID_VER_CMD, I2C_LAST_FRAME) != HAL_OK) {
				// Error here!!! (HAL_BUSY)
				Error_Handler();
			}

			break;

		case READ_DATA_CMD:

			/*
			//Test data
			data[0]=28.3;
			data[1]=50.0;
			data[2]=72.7;
			 */

			if (HAL_I2C_Slave_Sequential_Transmit_IT(hi2c,  (uint8_t *) &data, LEN_READ_DATA_CMD, I2C_LAST_FRAME) != HAL_OK) {
				// Error here!!! (HAL_BUSY)
				Error_Handler();
			}

			break;
		default:
			break;
		}
	}
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  HAL_I2C_EnableListen_IT(hi2c); // slave is ready again
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	Error_Handler();
}


float ieee_float(uint32_t f)
{
    union int_float{
        uint32_t i;
        float f;
    } tofloat;

    tofloat.i = f;
    return tofloat.f;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

