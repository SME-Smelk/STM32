/**
  ******************************************************************************
  * @file    stm32f4xx_sme_gps.h
  * @author  Ismael Poblete V.
  * @brief   Header file of SME GPS module.
  ******************************************************************************

  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_SME_GPS_H
#define __STM32F4xx_SME_GPS_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx_hal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stm32f4xx_sme_conf.h>

/**
  * @brief  NMEA GPGGA handle Structure definition
  */
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

/**
  * @brief  SME GPS Status structures definition
  */
/*
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
*/

/**
  * @brief  SME GPS structures definition
  */
typedef struct {

	char data_buffer[80]; 			/*!< Create a buffer to fill with GPS NMEA data.*/
	uint8_t count_data;				/*!< Counter like a motor to fill data to the buffer array.*/
	uint8_t recvd_data;				/*!< 1 byte to save the reception data of uart rx.*/
	uint8_t flag_data_ready;		/*!< Flag to indicate that the data is ready for loop function.*/
	GPS_GPGGA_t	GPGGA;				/*!< GPS NMEA GPGGA data parameters.*/

} GPS_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

#define SENTENCE_GPGGA "$GPGGA,"

/* Global function prototypes -----------------------------------------------*/

SME_StatusTypeDef SME_GPS_Init(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea);
SME_StatusTypeDef SME_GPS_DataProcess(GPS_HandleTypeDef *gps_nmea);
SME_StatusTypeDef SME_GPS_detecCommand(UART_HandleTypeDef *huart,GPS_HandleTypeDef *gps_nmea,const char* nmea_sentence);

#endif /* __STM32F4xx_SME_GPS_H */
