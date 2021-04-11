/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.c
  * @author  Ismael Poblete V.
  * @brief   GPS SME module driver.
  *          This file provides software functions to manage the GPS NMEA protocol
  *          with UART RX IT HAL functions.
  @verbatim
  ==============================================================================
                    	##### SME GPS features #####
  ==============================================================================


	Use sentences NMEA for data acquisition from SME GPS.

	Sentences compatibles and tested:
	(+)GPGGA	identified: SENTENCE_GPGGA

	Global functions for user:

	(+)SME_GPS_Init			: For initialization structure parameters
							  GPS_HandleTypeDef
	(+)SME_GPS_detecCommand	: Function to detect a GPS NMEA sentence
	(+)SME_GPS_DataProcess	: When the flag flag_data_ready = true it is ready
							  to process the buffer data of GPS NMEA structure
							  data_buffer.

                     ##### How to use this driver #####
  ==============================================================================

	(#) Set a structure type SME GPS GPS_HandleTypeDef

	(#) Set a structure type HAL UART with configuration as interrupt RX

	(#) Consider SENTENCE_GPGGA macro for GPGGA recognize in SME_GPS_detecCommand

	(#) Feed the GPS functions:

		(++)Init function
				SME_GPS_Init(&GPS_UART2,&GPS_NMEA).

		(++)For data interrupt acquisition in rx UART interrupt callback.
				SME_GPS_detecCommand(&GPS_UART2,&GPS_NMEA,SENTENCE_GPGGA);

		(++)For process data in infinite while loop execution.
				SME_GPS_DataProcess(&GPS_NMEA);

  @endverbatim
  ******************************************************************************

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_sme_gps.h"

#ifdef SME_GPS_MODULE_ENABLED

/* Private function prototypes -----------------------------------------------*/

static double convertDegMinToDecDeg (float degMin);
static uint16_t nmea_checksum(char *nmea_data, uint8_t char_length);
static char *subString(const char* in_str, uint8_t offset, uint8_t len);
static int32_t truncateStr(char instr[], char *outstr[], const char* delimeter) ;

/* Exported functions ---------------------------------------------------------*/
/*
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
 */

/**
 * @brief  Function to detect NMEA sentence command
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @param  NMEA sentence to find.
 * @retval GPS SME Status
 */
GPS_StatusTypeDef SME_GPS_detecCommand(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea,const char* nmea_sentence){

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
			if(gps_nmea->recvd_data == nmea_sentence[gps_nmea->count_data]){
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

/**
 * @brief  Function to initialize the GPS parameters.
 * @param  Pointer to a UART_HandleTypeDef structure that contains
 *         the configuration information for the specified UART module.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @retval GPS SME Status
 */
GPS_StatusTypeDef SME_GPS_Init(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea){
	gps_nmea->flag_data_ready=RESET;
	gps_nmea->count_data=0;
	memset(&gps_nmea,0,sizeof(gps_nmea));
	if(HAL_UART_Receive_IT(huart,&gps_nmea->recvd_data,1) == HAL_ERROR){
		return GPS_HAL_ERROR;
	}
	return GPS_OK;
}

/**
 * @brief  Function to process buffer data of NMEA GPS.
 * @param  Pointer to a GPS_HandleTypeDef structure that contains
 *         the configuration information for the specified SME GPS driver.
 * @retval GPS SME Status
 */
GPS_StatusTypeDef SME_GPS_DataProcess(GPS_HandleTypeDef *gps_nmea){

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

			/*Good data, save to GPGGA*/
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

/**
 * @brief  Separate a data in strings arrays
 * @param  Input string data to separate
 * @param  Output to put the string data.
 * @param  Delimeter for data separation
 * @retval Number of array elements that have been separated
 */
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

/**
 * @brief  Convert degree, minutes and seconds to decimal degrees
 * @param  degree, minutes and seconds in float
 * @retval decimal degrees
 */
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

/**
 * @brief  Get the checksum nmea crc value from the input string data
 * @param  Pointer to input string
 * @param  Length of the string
 * @retval checksum nmea crc value
 */
uint16_t nmea_checksum(char *nmea_data, uint8_t char_length)
{
    int crc = 0;
    int i;

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
 * @retval char pointer to a resulting string
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
    return outstr;

}


#endif /* SME_GPS_MODULE_ENABLED */
