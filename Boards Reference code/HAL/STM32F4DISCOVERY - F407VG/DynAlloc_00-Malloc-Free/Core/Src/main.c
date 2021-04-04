/**
  ******************************************************************************
  * @Project        : DynAlloc_00-Malloc-Free
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 04-04-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Program to see the behavior of malloc, free in heap of RAM.
  * 				  of the RAM.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		80MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @Note:
  * 	-pArrayOfGpsDataStr is a pointer witch is located in the stack of the RAM,
  * 	 high address value.
  * 	-The pointer (pArrayOfGpsDataStr[i]) point to the result of malloc witch
  * 	 keep the low address value of the heap of the RAM, it is the data location.
  * 		pArrayOfGpsDataStr[i] = (char*)malloc(80 * sizeof(char));
  *		-See in Memory Window and Live expression the:
  *			*pArrayOfGpsDataStr address: it is save in stack and is a high address
  *			 value of RAM
  *			*pArrayOfGpsDataStr value: Address witch point to the heap
  *			 low address value of RAM, Stack.
  *			*Value of the address pArrayOfGpsDataStr: It point to the heap where
  *			 the data value is store.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/
struct list_el {
   char weight[1024];
};

typedef struct list_el item;


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

	/* User Code */
	uint32_t cont=0;

	/* Start Code */
	while (1){

	/* Test malloc, dynamic allocation until RAM heap is fULL */
	/*
	  item * curr;
	  do {
		  cont++;
		  curr = (item *)malloc(sizeof(item));
		  free(curr);
	  } while (curr != NULL);
	  free(curr);
	*/

	/* Test malloc with array dynamic allocate */

	//Set a 14 pointers in stack, high address value of RAM.
	char *pArrayOfGpsDataStr[14];

	cont++;
	for (int i = 0; i < 14; i++) {

		//Each pointer [i], pointer to Heap of de low address value of RAM.
		//Each pointer is separate by 80 * sizeof(char), its means, 80bytes type char + 8 malloc, 88bytes.
		pArrayOfGpsDataStr[i] = (char*)malloc(80 * sizeof(char));
		if(pArrayOfGpsDataStr[i]!=NULL){
			//Clean the string in that location in [i] address
			memset(pArrayOfGpsDataStr[i],0,strlen(pArrayOfGpsDataStr[i]));
			//Create a string
			char *pData = "aaaaaaaaa";
			//The [i] address, than pointer to heap, write pData.
			strcpy(pArrayOfGpsDataStr[i], pData);
			strcat(pArrayOfGpsDataStr[i], "\0");
		}else{
			//Error no memory to allocate
			while(1);
		}
	}

	//Clean the heap and strings data values.
	for (int i = 0; i < 14; i++) {
		memset(pArrayOfGpsDataStr[i],0,strlen(pArrayOfGpsDataStr[i]));
		free(pArrayOfGpsDataStr[i]);
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}

