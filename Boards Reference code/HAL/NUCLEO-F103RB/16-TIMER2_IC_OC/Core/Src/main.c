/**
  ******************************************************************************
  * @Proyecto       : 16-TIMER2_IC_OC
  * @Date         	: 7-05-2020
  * @brief          : TIMER2 como modo Input Compare (IC) para leer frecuencia
  * 				  generada por el TIMER3 en modo Output Compare (OC) generando
  * 				  señales de 500Hz, 1.000Hz, 20.000Hz y 40.000Hz en sus canales
  * 				  CH1-4 respectivamente.
  *
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		8MHz
  * @Perf
  * 	*TIMER2
  * 		PA15	<------ IC_MODE
  * 	*TIMER3
  * 		PA		------> 6OC_CH1
  * 		PA7		------> OC_CH2
  * 		PB0		------> OC_CH3
  * 		PB1		------> OC_CH4
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);

/* Private user code ---------------------------------------------------------*/
uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if (Is_First_Captured==0)
		{
			IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			Is_First_Captured =1;
		}

		else if (Is_First_Captured)
		{
			IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

			if (IC_Value2 > IC_Value1)
			{
				Difference = IC_Value2-IC_Value1;
			}

			else if (IC_Value2 < IC_Value1)
			{
				Difference = ((0xffff-IC_Value1)+IC_Value2) +1;
			}

			else
			{
				Error_Handler();
			}

			Frequency = HAL_RCC_GetPCLK1Freq()/Difference*2;
			Is_First_Captured = 0;
		}
	}
}

volatile uint32_t pulse1_value = 8000; //to produce 500Hz	PB4 CH1
volatile uint32_t pulse2_value = 4000; //to produce 1000HZ PB5 CH2
volatile uint32_t pulse3_value = 2000;  //to produce 2000Hz PB0 CH3
volatile uint32_t pulse4_value = 1000;  //to produce 4000Hz PB1 CH4
volatile uint32_t ccr_content;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* TIM3_CH1 toggling with frequency = 500 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
  }

  /* TIM3_CH2 toggling with frequency = 1000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);

  }

  /* TIM3_CH3 toggling with frequency = 2000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);

  }

  /* TIM3_CH4 toggling with frequency = 4000 Hz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
	    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);

  }
}

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
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE */
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

  /* Infinite loop */
  while (1)
  {

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{



}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xffff;
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
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{


  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xFFFF;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = pulse1_value;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = pulse2_value;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = pulse3_value;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = pulse4_value;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

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
