/*
 * hts221.h
 *
 *  Created on: Nov 10, 2025
 *      Author: PC1
 */

#ifndef HTS221_H_
#define HTS221_H_

/* USER CODE BEGIN Includes */
#include "main.h"
#include "hts221_map.h"
/* USER CODE END Includes */

/* Special Macros BEGIN */
#define		getIndexHTS221(addr)			((addr)-HTS221_CALIB_0_ADDRESS)
#define     clampOutput(value, min, max) 	((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
/* Special Macros END */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
uint8_t HTS221_read_byte(uint8_t register_address);
void HTS221_read_array(uint8_t register_address, uint8_t* data, uint8_t length);
void HTS221_write_byte(uint8_t register_address, uint8_t byte_value);
void HTS221_write_array(uint8_t register_address, uint8_t* data, uint8_t length);

void HTS221_Init(uint8_t (*readCallback)(uint8_t, uint8_t, uint8_t*, uint8_t),
				 uint8_t (*writeCallback)(uint8_t, uint8_t, uint8_t*, uint8_t));
void HTS221_parse_calib_data(uint8_t *rawData);

float HTS221_get_humi();
float HTS221_get_temp();
/* USER CODE END Prototypes */



#endif /* HTS221_H_ */
