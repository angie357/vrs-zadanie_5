/*
 * lps25hb.h
 *
 *  Created on: Nov 10, 2025
 *      Author: PC1
 */

#ifndef LPS25HB_H_
#define LPS25HB_H_


/* USER CODE BEGIN Includes */
#include "main.h"
#include "math.h"
/* USER CODE END Includes */

/* Special Macros BEGIN */
#define clampOutput(value, min, max) 	((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
/* Special Macros END */

/* USER CODE BEGIN Private defines */
#define g0  	 9.80665		// gravity constant in m/s^2
#define M0 		 0.0289644		// molar mass of Earth's air in kg/mol
#define R_star   8.3144598		// universal gas constant in J/(mol*K)
#define T_Mb	 293.15			// environment temperature in Kelvin (20°C)
#define L_Mb	 -0.0065		// temperature lapse rate in K/m
#define riseRate T_Mb / L_Mb
#define exponent ((R_star * L_Mb) / (g0 * M0))

#define MA_WINDOW 25
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
uint8_t LPS25HB_read_byte(uint8_t register_address);
void LPS25HB_read_array(uint8_t register_address, uint8_t* data, uint8_t length);
void LPS25HB_write_byte(uint8_t register_address, uint8_t byte_value);
void LPS25HB_write_array(uint8_t register_address, uint8_t* data, uint8_t length);

void LPS25HB_Init(uint8_t (*readCallback)(uint8_t, uint8_t, uint8_t*, uint8_t),
				  uint8_t (*writeCallback)(uint8_t, uint8_t, uint8_t*, uint8_t));

float LPS25HB_get_press();
float LPS25HB_get_temp();
float LPS25HB_get_relative_height();
/* USER CODE END Prototypes */


#endif /* LPS25HB_H_ */
