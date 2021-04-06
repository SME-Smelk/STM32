/**
  ******************************************************************************
  * @Project        : GPS_00-UART_RX_IT
  * @Autor          : Ismael Poblete V.
  * @Company		: -
  * @Date         	: 04-04-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Test a GPS RX IT data reception.
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		80MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART1
  * 		PA9			<-----> USART_TX
  * 		PA10			<-----> USART_RX
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * @Note:
  * 	-GPS used:
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

} GPGGA_t;

typedef enum
{
  GPS_DATA_RECEIVED       = 0x00U,
  GPS_NODATA    		  = 0x01U,
  GPS_ERROR    			  = 0x02U,
  GPS_BUF_NO_MEMORY       = 0x03U,
} GPS_Status;

typedef struct {
	char data_buffer[80];
	uint8_t count_data;
	uint8_t flag_data_ready;
	uint8_t recvd_data;
	GPGGA_t	GPGGA;
	GPS_Status GPS_Status;
} GPS_t;

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
void GPS_Init(void);
int GPS_DataProcess(void);
double convertDegMinToDecDeg (float degMin);
uint16_t nmea_checksum(char *nmea_data, uint8_t char_length);
const char* subString(const char* in_str, uint8_t offset, uint8_t len);
int32_t truncateStr(char instr[], char *outstr[], const char delimeter);

/* Private user code ---------------------------------------------------------*/
GPS_t GPS_NMEA;
char pData[50];

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
	GPS_Init();

	/* User Code */

	/* Start Code */
	while (1){
		if(GPS_DataProcess() == GPS_DATA_RECEIVED){

			sprintf(pData,"GPS:Data Received\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);

		}else if(GPS_DataProcess() == GPS_ERROR){

			//while(1);
			sprintf(pData,"GPS No Fix\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);

		}else if(GPS_DataProcess() == GPS_BUF_NO_MEMORY){

			//while(1);
			sprintf(pData,"GPS Mem buff err\n");
			HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);

		}else if(GPS_DataProcess() == GPS_NODATA){

			//while(1);
			//sprintf(pData,"GPS No Fix\n");
			//HAL_UART_Transmit(&DEBUG_UART1, (uint8_t*)pData, sizeof(pData), HAL_MAX_DELAY);

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

	if(GPS_NMEA.count_data >= 7){
	/* Data receive*/

		if(GPS_NMEA.recvd_data == '\r'){
		/*Data Ready*/
			GPS_NMEA.data_buffer[GPS_NMEA.count_data] = '\n';
			GPS_NMEA.flag_data_ready = SET;
		}else{
		/*Fill buffer*/
			GPS_NMEA.data_buffer[GPS_NMEA.count_data] = GPS_NMEA.recvd_data;
			GPS_NMEA.count_data++;
		}

	}else{
	/* Command option detect: $GPGGA*/
		GPS_NMEA.data_buffer[GPS_NMEA.count_data] = GPS_NMEA.recvd_data;
		if(GPS_NMEA.recvd_data == '$'){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == 'G') && (GPS_NMEA.count_data == 1)){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == 'P') && (GPS_NMEA.count_data == 2)){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == 'G') && (GPS_NMEA.count_data == 3)){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == 'G') && (GPS_NMEA.count_data == 4)){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == 'A') && (GPS_NMEA.count_data == 5)){
			GPS_NMEA.count_data++;
		}else if((GPS_NMEA.recvd_data == ',') && (GPS_NMEA.count_data == 6)){
			GPS_NMEA.count_data++;
		}else{
			GPS_NMEA.count_data = 0;
		}
	}

	HAL_UART_Receive_IT(&GPS_UART2,&GPS_NMEA.recvd_data,1);

}

void GPS_Init(void){
	GPS_NMEA.flag_data_ready=RESET;
	GPS_NMEA.count_data=0;
	HAL_UART_Receive_IT(&GPS_UART2,&GPS_NMEA.recvd_data,1);
}

uint32_t cont=0;
int GPS_DataProcess(void){

	//GPS_NMEA.flag_data_ready=SET;
	if(GPS_NMEA.flag_data_ready){
/*
		//Data test
		//Signal
		char buffer[] = "$GPGGA,005314.00,2337.93836,S,07022.79995,W,1,04,6.54,107.4,M,34.1,M,,*59\ndga";
		//No signal
		//char buffer[] = "$GPGGA,154053.00,,,,,0,00,99.99,,,,,,*6";
		memset(GPS_NMEA.data_buffer, 0, strlen(GPS_NMEA.data_buffer));
		strncpy(GPS_NMEA.data_buffer, buffer, strlen(buffer));
*/

		/* Parse data */
		char *pArrayOfGpsDataStr[14];
		char *str;
		cont++;

		/*Create a buffer for data*/
		str = malloc(strlen(GPS_NMEA.data_buffer)+1);
		if(str==NULL)
		{
			GPS_NMEA.GPS_Status=GPS_BUF_NO_MEMORY;
			return GPS_BUF_NO_MEMORY;
		}
		memset(str,0,strlen(str));
		/*Save data buffer to str*/
		strlcpy (str,(char*)GPS_NMEA.data_buffer,strlen((char*)GPS_NMEA.data_buffer) + 1);

		/*Clean GPGGA*/
		memset(&GPS_NMEA.GPGGA,0,sizeof(GPS_NMEA.GPGGA));

		/*Create a buffer to strings vectors of GPGGA parameters*/
		for (int i = 0; i < 14; i++) {

			pArrayOfGpsDataStr[i] = malloc(sizeof(GPS_NMEA.data_buffer) * sizeof(char));
			if(pArrayOfGpsDataStr[i]==NULL){
				GPS_NMEA.GPS_Status=GPS_BUF_NO_MEMORY;
				return GPS_BUF_NO_MEMORY;
			}
			memset(pArrayOfGpsDataStr[i],0,strlen(pArrayOfGpsDataStr[i]));

		}

		/* Separate the string GPGGA Parameters*/
		truncateStr(str, pArrayOfGpsDataStr, ',');

		/*Checksum CRC NMEA;*/
		GPS_NMEA.GPGGA.checksum = strtoul(subString(pArrayOfGpsDataStr[13],1,(strlen(pArrayOfGpsDataStr[13])-1)), NULL, 16);
		uint16_t checksum = nmea_checksum(GPS_NMEA.data_buffer,strlen(pArrayOfGpsDataStr[13]));

		/*Checksum fix quality*/
		GPS_NMEA.GPGGA.fix_quality = atoi((char*)subString(pArrayOfGpsDataStr[6],0,strlen(pArrayOfGpsDataStr[6])));

		if((checksum == GPS_NMEA.GPGGA.checksum) && (GPS_NMEA.GPGGA.fix_quality != 0)){

			GPS_NMEA.GPGGA.utc_time[0] = atoi(subString(pArrayOfGpsDataStr[1],0,2));
			GPS_NMEA.GPGGA.utc_time[1] = atoi(subString(pArrayOfGpsDataStr[1],2,2));
			GPS_NMEA.GPGGA.utc_time[2] = atoi(subString(pArrayOfGpsDataStr[1],4,2));
			GPS_NMEA.GPGGA.latitude = atof(subString(pArrayOfGpsDataStr[2],0,strlen(pArrayOfGpsDataStr[2])));
			GPS_NMEA.GPGGA.lat_direction = *subString(pArrayOfGpsDataStr[3],0,strlen(pArrayOfGpsDataStr[3]));
			GPS_NMEA.GPGGA.longitude = atof(subString(pArrayOfGpsDataStr[4],0,strlen(pArrayOfGpsDataStr[4])));
			GPS_NMEA.GPGGA.lon_direction = *subString(pArrayOfGpsDataStr[5],0,strlen(pArrayOfGpsDataStr[5]));
			GPS_NMEA.GPGGA.num_sattelites = atoi(subString(pArrayOfGpsDataStr[7],0,strlen(pArrayOfGpsDataStr[7])));
			GPS_NMEA.GPGGA.h_duration = atof(subString(pArrayOfGpsDataStr[8],0,strlen(pArrayOfGpsDataStr[8])));
			GPS_NMEA.GPGGA.altitude = atof(subString(pArrayOfGpsDataStr[9],0,strlen(pArrayOfGpsDataStr[9])));
			GPS_NMEA.GPGGA.altitude_unit = *subString(pArrayOfGpsDataStr[10],0,strlen(pArrayOfGpsDataStr[10]));
			GPS_NMEA.GPGGA.height_of_geoid = atof(subString(pArrayOfGpsDataStr[11],0,strlen(pArrayOfGpsDataStr[11])));
			GPS_NMEA.GPGGA.hog_unit = *subString(pArrayOfGpsDataStr[12],0,strlen(pArrayOfGpsDataStr[12]));

			if(GPS_NMEA.GPGGA.lat_direction == 'N'){
			GPS_NMEA.GPGGA.latitude=convertDegMinToDecDeg(GPS_NMEA.GPGGA.latitude);
			}else if(GPS_NMEA.GPGGA.lat_direction == 'S'){
			GPS_NMEA.GPGGA.latitude=convertDegMinToDecDeg(GPS_NMEA.GPGGA.latitude * -1);
			}

			if(GPS_NMEA.GPGGA.lon_direction == 'E'){
			GPS_NMEA.GPGGA.longitude=convertDegMinToDecDeg(GPS_NMEA.GPGGA.longitude);
			}else if(GPS_NMEA.GPGGA.lon_direction == 'W'){
			GPS_NMEA.GPGGA.longitude=convertDegMinToDecDeg(GPS_NMEA.GPGGA.longitude * -1);
			}

			//check buff memory of function subString
			if(GPS_NMEA.GPS_Status==GPS_BUF_NO_MEMORY){
				return GPS_BUF_NO_MEMORY;
			}

			/* No error, good data */
			memset(&GPS_NMEA.data_buffer,0,sizeof(GPS_NMEA.data_buffer));
			for (int i = 0; i < 14; i++) {
				memset(pArrayOfGpsDataStr[i],0,strlen(pArrayOfGpsDataStr[i]));
				free(pArrayOfGpsDataStr[i]);
			}
			free(str);
			GPS_NMEA.flag_data_ready = RESET;
			GPS_NMEA.count_data=0;
			GPS_NMEA.GPS_Status=GPS_DATA_RECEIVED;
			return GPS_DATA_RECEIVED;
		}else{
			/* No signal or error in data integrity */
			memset(&GPS_NMEA.GPGGA,0,sizeof(GPS_NMEA.GPGGA));
			memset(&GPS_NMEA.data_buffer,0,sizeof(GPS_NMEA.data_buffer));
			for (int i = 0; i < 14; i++) {
				memset(pArrayOfGpsDataStr[i],0,strlen(pArrayOfGpsDataStr[i]));
				free(pArrayOfGpsDataStr[i]);
			}
			free(str);
			GPS_NMEA.flag_data_ready = RESET;
			GPS_NMEA.count_data=0;
			GPS_NMEA.GPS_Status=GPS_ERROR;
			return GPS_ERROR;
		}

	}

	/*No data*/
	GPS_NMEA.GPS_Status=GPS_NODATA;
	return GPS_NODATA;
}

int32_t truncateStr(char instr[], char *outstr[], const char delimeter) {
    char *tempBuff;
    int32_t numberOfRawsOutArray = 0;
    tempBuff = strtok((char*)instr, &delimeter);
    while (tempBuff != NULL) {
        strcpy(outstr[numberOfRawsOutArray], tempBuff);
        strcat(outstr[numberOfRawsOutArray], "\0");
        numberOfRawsOutArray++;
        tempBuff = strtok(NULL, &delimeter);
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
const char* subString(const char* in_str, uint8_t offset, uint8_t len) {

	uint8_t instr_length = strlen(in_str);
    if (offset + len > instr_length) {
        return NULL;
    }

    char *outstr = malloc(len + 1);
    if(outstr==NULL){
    	GPS_NMEA.GPS_Status=GPS_BUF_NO_MEMORY;
    }
    memset((char*)outstr,0,strlen(outstr));
    strncpy(outstr,&in_str[offset], len);
    outstr[offset + len] = '\0';
    free(outstr);
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

