/*
 * hts221.c
 *
 *  Created on: Nov 10, 2025
 *      Author: PC1
 */

#include "HTS221.h"

const double inv_2 = 1.0 / 2.0;
const double inv_8 = 1.0 / 8.0;
float H0_rH, H1_rH;								// In rH
float T0_degC, T1_degC;							// In degC
int16_t H0_T0_OUT, H1_T0_OUT, T0_OUT, T1_OUT; 	// In relative units
uint8_t HTS221_f_deviceConnected;

static uint8_t (* I2C_ReadData)(uint8_t slave_address, uint8_t register_address, uint8_t* rx_data, uint8_t data_count) = 0;

static uint8_t (* I2C_WriteData)(uint8_t slave_address, uint8_t register_address, uint8_t* tx_data, uint8_t data_count) = 0;

uint8_t HTS221_read_byte(uint8_t register_address)
{
	uint8_t rx_data;
	I2C_ReadData(HTS221_DEVICE_ADDRESS, register_address, &rx_data, 1);
	return rx_data;
}

void HTS221_read_array(uint8_t register_address, uint8_t* data, uint8_t length)
{
	I2C_ReadData(HTS221_DEVICE_ADDRESS, register_address, data, length);
}

void HTS221_write_byte(uint8_t register_address, uint8_t byte_value)
{
	I2C_WriteData(HTS221_DEVICE_ADDRESS, register_address, &byte_value, 1);
}

void HTS221_write_array(uint8_t register_address, uint8_t* data, uint8_t length)
{
	I2C_WriteData(HTS221_DEVICE_ADDRESS, register_address, data, length);
}


void HTS221_Init(uint8_t (*readCallback)(uint8_t, uint8_t, uint8_t*, uint8_t),
				 uint8_t (*writeCallback)(uint8_t, uint8_t, uint8_t*, uint8_t))
{
	// Register Generic Read Callback
	if(readCallback != 0)
		I2C_ReadData = readCallback;
	// Register Generic Write Callback
	if(writeCallback != 0)
		I2C_WriteData = writeCallback;

	// Check if correct device is connected
	HTS221_f_deviceConnected = HTS221_read_byte(HTS221_WHO_AM_I_ADDRESS) == HTS221_WHO_AM_I_VALUE;

	if(HTS221_f_deviceConnected)
	{
		// Set CONTROL register to 0x83 = 0b1RRR R011 (ODR 12.5Hz 12.5Hz, PD)
		HTS221_write_byte(HTS221_CTRL_REG1_ADDRESS, HTS221_CTRL_REG1_VALUE);
		LL_mDelay(10);

		// Read calibration coefficients
		uint8_t calibData[HTS221_CALIB_F_ADDRESS-HTS221_CALIB_0_ADDRESS+1];
		HTS221_read_array(HTS221_CALIB_0_ADDRESS, calibData, sizeof(calibData));
		HTS221_parse_calib_data(calibData);
	}
}

void HTS221_parse_calib_data(uint8_t *rawData)
{
	uint8_t H0_rH_x2, H1_rH_x2;
	uint16_t T0_degC_x8, T1_degC_x8;

	H0_rH_x2 = (uint8_t)(rawData[getIndexHTS221(HTS221_CALIB_0_ADDRESS)]);
	H1_rH_x2 = (uint8_t)(rawData[getIndexHTS221(HTS221_CALIB_1_ADDRESS)]);
	T0_degC_x8 = (uint16_t)(((uint16_t)(rawData[getIndexHTS221(HTS221_CALIB_2_ADDRESS)] & 0xFFU) << 0) |
			                ((uint16_t)(rawData[getIndexHTS221(HTS221_CALIB_5_ADDRESS)] & 0x03U) << 8));
	T1_degC_x8 = (uint16_t)(((uint16_t)(rawData[getIndexHTS221(HTS221_CALIB_3_ADDRESS)] & 0xFFU) << 0) |
				            ((uint16_t)(rawData[getIndexHTS221(HTS221_CALIB_5_ADDRESS)] & 0x0CU) << 6));
	H0_T0_OUT = (int16_t)(((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_6_ADDRESS)] << 0) |
				 	 	  ((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_7_ADDRESS)] << 8));
	H1_T0_OUT = (int16_t)(((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_A_ADDRESS)] << 0) |
						  ((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_B_ADDRESS)] << 8));
	T0_OUT = (int16_t)(((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_C_ADDRESS)] << 0) |
			 	 	   ((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_D_ADDRESS)] << 8));
	T1_OUT = (int16_t)(((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_E_ADDRESS)] << 0) |
				 	   ((uint16_t)rawData[getIndexHTS221(HTS221_CALIB_F_ADDRESS)] << 8));

	H0_rH = H0_rH_x2 * inv_2;
	H1_rH = H1_rH_x2 * inv_2;
	T0_degC = T0_degC_x8 * inv_8;
	T1_degC = T1_degC_x8 * inv_8;
}

float HTS221_get_humi()
{
	uint8_t H_T_OUT[2];
	HTS221_read_array(HTS221_HUMIDITY_OUT_L_ADDRESS, H_T_OUT, sizeof(H_T_OUT));
	int16_t INT_H_T_OUT = (int16_t)(((uint16_t)H_T_OUT[0] << 0) |
							        ((uint16_t)H_T_OUT[1] << 8));
	return clampOutput((((H1_rH-H0_rH)*(INT_H_T_OUT-H0_T0_OUT)) / (H1_T0_OUT-H0_T0_OUT)) + H0_rH, HTS221_HUMI_MIN, HTS221_HUMI_MAX);
}

float HTS221_get_temp()
{
	uint8_t T_OUT[2];
	HTS221_read_array(HTS221_TEMP_OUT_L_ADDRESS, T_OUT, sizeof(T_OUT));
	int16_t INT_T_OUT = (int16_t)(((uint16_t)T_OUT[0] << 0) |
							      ((uint16_t)T_OUT[1] << 8));
	return clampOutput((((T1_degC-T0_degC)*(INT_T_OUT-T0_OUT)) / (T1_OUT-T0_OUT)) + T0_degC, HTS221_TEMP_MIN, HTS221_TEMP_MAX);
}

