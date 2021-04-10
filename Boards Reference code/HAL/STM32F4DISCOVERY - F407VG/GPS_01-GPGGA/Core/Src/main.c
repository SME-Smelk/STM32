/**
  ******************************************************************************
  * @Project        : GPS_01-GPGGA
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 10-04-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : GPS NMEA GPGGA RX IT data reception.
  * @Lib			: CMSIS, HAL.
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
  * 	-See GPS_HandleTypeDef GPS_NMEA
  * 		-Data adquisition:
  * 			-buffer to receive 1 byte rx uart data (recvd_data)
  * 			-counter for data fill(count_data)
  * 			-buffer to fill (data_buffer)
  * 			-Data process in loop just when have the full data buffer (flag_data_ready = true)
  * 		-Debugs values:
  * 			-GPS_StatusTypeDef Content return values of data status and errors
  * 		-GPGGA Sentence:
  * 			-GPS_GPGGA_t Content all data information of NMEA GPGGA
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/*Structure and type for storing GGA NMEA parsed message (location info)*/

typedef struct {

    uint8_t utc_time[3];
    float latitude;
    char lat_direction;
    float longitude;
    char lon_direction;
    uint8_t fix_quality;
    uint8_t num_sattelites;
    float h_duration;
    float altitude;
    char altitude_unit;
    float height_of_geoid;
    char hog_unit;
    uint16_t checksum;

} GPS_GPGGA_t;

typedef enum
{
  GPS_OK       			  = 0x00U,
  GPS_BUSY				  = 0x01U,
  GPS_DATA_RECEIVED		  = 0x02U,
  GPS_NODATA    		  = 0x03U,
  GPS_ERROR    			  = 0x04U,
  GPS_HAL_ERROR			  = 0x05U,
  GPS_BUF_NO_MEMORY       = 0x06U,
} GPS_StatusTypeDef;

typedef struct {

	char data_buffer[80];
	uint8_t count_data;
	uint8_t recvd_data;
	uint8_t flag_data_ready;
	GPS_GPGGA_t	GPGGA;
	//GPS_GPRMC_t	GPRMC;

} GPS_HandleTypeDef;

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

//GPS
//APIs
GPS_StatusTypeDef GPS_Init(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea);
GPS_StatusTypeDef GPS_DataProcess(GPS_HandleTypeDef *gps_nmea);
GPS_StatusTypeDef GPS_detecCommand(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea);

//private
double convertDegMinToDecDeg (float degMin);
uint16_t nmea_checksum(char *nmea_data, uint8_t char_length);
char *subString(const char* in_str, uint8_t offset, uint8_t len);
int32_t truncateStr(char instr[], char *outstr[], const char* delimeter) ;

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
	GPS_Init(&GPS_UART2,&GPS_NMEA);

	/* User Code */

	/* Start Code */
	while (1){
		switch(GPS_DataProcess(&GPS_NMEA)){
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
			/*
			memset(pData,0,sizeof(pData));
			sprintf(pData,"NODATA\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);
			*/
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

	GPS_detecCommand(&GPS_UART2,&GPS_NMEA);

}



GPS_StatusTypeDef GPS_detecCommand(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea){

	if(!gps_nmea->flag_data_ready){

		/* Get 1 byte data*/
		gps_nmea->data_buffer[gps_nmea->count_data] = gps_nmea->recvd_data;

		/*Command has been detected*/
		if((gps_nmea->count_data >= 7)){

			if(gps_nmea->recvd_data == '\r'){
			/*Data Ready*/
				gps_nmea->data_buffer[gps_nmea->count_data] = '\0';
				gps_nmea->flag_data_ready = SET;
			}else{
			/*Fill buffer*/
				gps_nmea->data_buffer[gps_nmea->count_data] =gps_nmea->recvd_data;
				gps_nmea->count_data++;
			}

		}else{
			/* Detecting command*/
			if(gps_nmea->recvd_data == SENTENCE_GPGGA[gps_nmea->count_data]){
				gps_nmea->count_data++;
			}else{
				gps_nmea->count_data = 0;
			}
		}

		if(HAL_UART_Receive_IT(huart,&gps_nmea->recvd_data,1) == HAL_ERROR){
			return GPS_ERROR;
		}

		return GPS_OK;
	}

	if(HAL_UART_Receive_IT(huart,&gps_nmea->recvd_data,1) == HAL_ERROR){
		return GPS_ERROR;
	}
	return GPS_BUSY;

}

GPS_StatusTypeDef GPS_Init(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea){
	gps_nmea->flag_data_ready=RESET;
	gps_nmea->count_data=0;
	memset(&gps_nmea,0,sizeof(gps_nmea));
	if(HAL_UART_Receive_IT(huart,&gps_nmea->recvd_data,1) == HAL_ERROR){
		return GPS_HAL_ERROR;
	}
	return GPS_OK;
}

GPS_StatusTypeDef GPS_DataProcess(GPS_HandleTypeDef *gps_nmea){

	if(gps_nmea->flag_data_ready){
/*
		//Data test
		//Signal
		char buffer[] = "$GPGGA,005314.00,2337.93836,S,07022.79995,W,1,04,6.54,107.4,M,34.1,M,,*59\0";
		//No signal
		//char buffer[] = "$GPGGA,154053.00,,,,,0,00,99.99,,,,,,*6";
		memset(GPS_NMEA.data_buffer, 0, strlen(GPS_NMEA.data_buffer));
		strncpy(GPS_NMEA.data_buffer, buffer, strlen(buffer));
*/

		/* Parse data */
		char *pDynamic_ArrayGpsData[14];
		char *pDynamic_buffer;

		/*Create a buffer for data*/
		pDynamic_buffer = malloc(strlen(gps_nmea->data_buffer)+1);

		if(pDynamic_buffer==NULL)
		{
			return GPS_BUF_NO_MEMORY;
		}

		/*Save data buffer to pDynamic_buffer*/
		strlcpy (pDynamic_buffer,(char*)gps_nmea->data_buffer,strlen((char*)gps_nmea->data_buffer) + 1);

		/*Create a buffer to strings vectors of sentence parameters*/
		for (int i = 0; i < 14; i++) {

			pDynamic_ArrayGpsData[i] = malloc(sizeof(gps_nmea->data_buffer) * sizeof(char));
			if(pDynamic_ArrayGpsData[i]==NULL){
				return GPS_BUF_NO_MEMORY;
			}
			memset(pDynamic_ArrayGpsData[i],0,strlen(pDynamic_ArrayGpsData[i]));

		}

		/*Clean GPGGA*/
		memset(&gps_nmea->GPGGA,0,sizeof(gps_nmea->GPGGA));



		/* Separate the string GPGGA Parameters*/
		const char* delimiter = ",";
		truncateStr(pDynamic_buffer, pDynamic_ArrayGpsData,delimiter);

		/*Checksum CRC NMEA;*/
		char *pptr;

		pptr = subString(pDynamic_ArrayGpsData[13],1,(strlen(pDynamic_ArrayGpsData[13])-1));
		gps_nmea->GPGGA.checksum = strtoul((char*)pptr, NULL, 16);
		free(pptr);

		uint16_t checksum = nmea_checksum(gps_nmea->data_buffer,strlen(pDynamic_ArrayGpsData[13]));

		/*Checksum fix quality*/
		pptr = (char*)subString(pDynamic_ArrayGpsData[6],0,strlen(pDynamic_ArrayGpsData[6]));
		gps_nmea->GPGGA.fix_quality = atoi(pptr);
		free(pptr);

		if((checksum == gps_nmea->GPGGA.checksum) && (gps_nmea->GPGGA.fix_quality != 0)){

			pptr = subString(pDynamic_ArrayGpsData[1],0,2);
			gps_nmea->GPGGA.utc_time[0] = atoi(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[1],2,2);
			gps_nmea->GPGGA.utc_time[1] = atoi(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[1],4,2);
			gps_nmea->GPGGA.utc_time[2] = atoi(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[2],0,strlen(pDynamic_ArrayGpsData[2]));
			gps_nmea->GPGGA.latitude = atof(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[3],0,strlen(pDynamic_ArrayGpsData[3]));
			gps_nmea->GPGGA.lat_direction = *pptr;
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[4],0,strlen(pDynamic_ArrayGpsData[4]));
			gps_nmea->GPGGA.longitude = atof(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[5],0,strlen(pDynamic_ArrayGpsData[5]));
			gps_nmea->GPGGA.lon_direction = *pptr;
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[7],0,strlen(pDynamic_ArrayGpsData[7]));
			gps_nmea->GPGGA.num_sattelites = atoi(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[8],0,strlen(pDynamic_ArrayGpsData[8]));
			gps_nmea->GPGGA.h_duration = atof(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[9],0,strlen(pDynamic_ArrayGpsData[9]));
			gps_nmea->GPGGA.altitude = atof(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[10],0,strlen(pDynamic_ArrayGpsData[10]));
			gps_nmea->GPGGA.altitude_unit = *pptr;
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[11],0,strlen(pDynamic_ArrayGpsData[11]));
			gps_nmea->GPGGA.height_of_geoid = atof(pptr);
			free(pptr);

			pptr = subString(pDynamic_ArrayGpsData[12],0,strlen(pDynamic_ArrayGpsData[12]));
			gps_nmea->GPGGA.hog_unit = *pptr;
			free(pptr);


			if(gps_nmea->GPGGA.lat_direction == 'N'){
				gps_nmea->GPGGA.latitude=convertDegMinToDecDeg(gps_nmea->GPGGA.latitude);
			}else if(gps_nmea->GPGGA.lat_direction == 'S'){
				gps_nmea->GPGGA.latitude=convertDegMinToDecDeg(gps_nmea->GPGGA.latitude * -1);
			}

			if(gps_nmea->GPGGA.lon_direction == 'E'){
				gps_nmea->GPGGA.longitude=convertDegMinToDecDeg(gps_nmea->GPGGA.longitude);
			}else if(gps_nmea->GPGGA.lon_direction == 'W'){
				gps_nmea->GPGGA.longitude=convertDegMinToDecDeg(gps_nmea->GPGGA.longitude * -1);
			}

			/* No error, good data */
			memset(&gps_nmea->data_buffer,0,sizeof(gps_nmea->data_buffer));
			free(pDynamic_buffer);
			for (int i = 0; i < 14; i++) {
				memset(pDynamic_ArrayGpsData[i],0,strlen(pDynamic_ArrayGpsData[i]));
				free(pDynamic_ArrayGpsData[i]);
			}

			gps_nmea->flag_data_ready = RESET;
			gps_nmea->count_data=0;
			return GPS_DATA_RECEIVED;
		}else{
			/* No signal or error in data integrity */
			free(pDynamic_buffer);
			memset(&gps_nmea->GPGGA,0,sizeof(gps_nmea->GPGGA));
			memset(&gps_nmea->data_buffer,0,sizeof(gps_nmea->data_buffer));
			for (int i = 0; i < 14; i++) {
				memset(pDynamic_ArrayGpsData[i],0,strlen(pDynamic_ArrayGpsData[i]));
				free(pDynamic_ArrayGpsData[i]);
			}

			gps_nmea->flag_data_ready = RESET;
			gps_nmea->count_data=0;
			return GPS_ERROR;
		}
	}
	/*No data*/
	return GPS_NODATA;
}

int32_t truncateStr(char instr[], char *outstr[], const char* delimeter) {
    char *tempBuff;
    int32_t numberOfRawsOutArray = 0;
    tempBuff = strtok((char*)instr, delimeter);
    while (tempBuff != NULL) {
        strcpy(outstr[numberOfRawsOutArray], tempBuff);
        strcat(outstr[numberOfRawsOutArray], "\0");
        numberOfRawsOutArray++;
        tempBuff = strtok(NULL, delimeter);
    }
    return numberOfRawsOutArray;
}


double convertDegMinToDecDeg (float degMin)
{
  double min = 0.0;
  double decDeg = 0.0;

  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);

  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );

  return decDeg;
}

uint16_t nmea_checksum(char *nmea_data, uint8_t char_length)
{
    int crc = 0;
    int i;
    //uint8_t err_off;

    //err_off=strlen(strstr(nmea_data,'\n'));

    // Between $ and * sign.
    for (i = 1; i < strlen(nmea_data) - char_length; i ++) {
        crc ^= nmea_data[i];
    }

    return crc;
}

/**
 * @brief  Retrieving specific part of the string
 * @param  Pointer to input string
 * @param  Start position of the required part of the string
 * @param  Length of the required part of the string
 * @retval Constant pointer to a resulting string
 */
 char *subString(const char* in_str, uint8_t offset, uint8_t len) {

	uint8_t instr_length = strlen(in_str);
    if (offset + len > instr_length) {
        return NULL;
    }

    char *outstr = malloc(len + 1);
    if(outstr==NULL){
    	//Data Overflow

    }
    memset((char*)outstr,0,strlen(outstr));
    strncpy(outstr,&in_str[offset], len);
    outstr[offset + len] = '\0';
    //free(outstr);
    return outstr;

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}

