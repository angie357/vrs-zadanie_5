/*
 * lps25hb.c
 *
 *  Created on: Nov 10, 2025
 *      Author: PC1
 */


#include "lps25hb.h"


uint8_t LPS25HB_f_deviceConnected;
float referencePressure;

uint8_t ma_count = 0, ma_index = 0;
float ma_sum = 0, ma_buff[MA_WINDOW];

static uint8_t (* I2C_ReadData)(uint8_t slave_address, uint8_t register_address, uint8_t* rx_data, uint8_t data_count) = 0;

static uint8_t (* I2C_WriteData)(uint8_t slave_address, uint8_t register_address, uint8_t* tx_data, uint8_t data_count) = 0;

uint8_t LPS25HB_read_byte(uint8_t register_address)
{
	uint8_t rx_data;
	I2C_ReadData(LPS25HB_DEVICE_ADDRESS, register_address, &rx_data, 1);
	return rx_data;
}

void LPS25HB_read_array(uint8_t register_address, uint8_t* data, uint8_t length)
{
	I2C_ReadData(LPS25HB_DEVICE_ADDRESS, register_address, data, length);
}

void LPS25HB_write_byte(uint8_t register_address, uint8_t byte_value)
{
	I2C_WriteData(LPS25HB_DEVICE_ADDRESS, register_address, &byte_value, 1);
}

void LPS25HB_write_array(uint8_t register_address, uint8_t* data, uint8_t length)
{
	I2C_WriteData(LPS25HB_DEVICE_ADDRESS, register_address, data, length);
}

void LPS25HB_Init(uint8_t (*readCallback)(uint8_t, uint8_t, uint8_t*, uint8_t),
				  uint8_t (*writeCallback)(uint8_t, uint8_t, uint8_t*, uint8_t))
{
	// Register Generic Read Callback
	if(readCallback != 0)
		I2C_ReadData = readCallback;
	// Register Generic Write Callback
	if(writeCallback != 0)
		I2C_WriteData = writeCallback;

	// Check if correct device is connected
	LPS25HB_f_deviceConnected = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRESS) == LPS25HB_WHO_AM_I_VALUE;

	if(LPS25HB_f_deviceConnected)
	{
		// Set CONTROL register 0xC2 = 0b1100 0010 (ODR 25Hz 25Hz, PD, RESET_AZ)
		LPS25HB_write_byte(LPS25HB_CTRL_REG1_ADDRESS, LPS25HB_CTRL_REG1_VALUE);

		// Set CONTROL register 0x10 = 0b0001 0000 (FIFO_MEAN_DEC)
		LPS25HB_write_byte(LPS25HB_CTRL_REG2_ADDRESS, LPS25HB_CTRL_REG2_VALUE);

		// Set RESOLUTION register 0x0F = 0bRRRR 1111 (avg 64C avg 512hPa)
		LPS25HB_write_byte(LPS25HB_RES_CONF_ADDRESS, LPS25HB_RES_CONF_VALUE);

		// Set FIFO_CONTROL register 0xDF = 00b1101 1111 (FIFO Mean mode, 32 sample moving avg)
		LPS25HB_write_byte(LPS25HB_FIFO_CTRL_ADDRESS, LPS25HB_FIFO_CTRL_VALUE);

		// Set RPDS registers
		uint8_t rpdsReg[2];
		rpdsReg[0] = 0x00U;
		rpdsReg[1] = 0x00U;
		LPS25HB_write_array(LPS25HB_RPDS_L_ADDRESS, rpdsReg, sizeof(rpdsReg));

		// Get average reference pressure for height calculation
		float sample_sum = 0;
		for (int i = 0; i < MA_WINDOW; i++)
		{
			LL_mDelay(100); // 10Hz
			sample_sum += LPS25HB_get_press();
		}
		referencePressure = sample_sum / MA_WINDOW;
	}
}

float LPS25HB_get_press()
{
	uint8_t rawPressure[3], minusSign = 0;
	LPS25HB_read_array(LPS25HB_PRESS_OUT_XL_ADDRESS, rawPressure, sizeof(rawPressure));
	if(rawPressure[2] & 0x80U) // If MSB of PRESS_OUT_H is 1
		minusSign = 1, rawPressure[2] &= 0x7FU;
	uint32_t pressure = (uint32_t)(((uint32_t)rawPressure[0]   << 0)  |
			 	 	 	 	 	   ((uint32_t)(rawPressure[1]) << 8)  |
								   ((uint32_t)(rawPressure[2]) << 16));
	if(minusSign) pressure |= 0x80000000U;

	return clampOutput((int32_t)pressure / 4096.0, LPS25HB_PRESS_MIN, LPS25HB_PRESS_MAX);
}

float LPS25HB_get_temp()
{
	uint8_t rawTemp[2];
	LPS25HB_read_array(LPS25HB_TEMP_OUT_L_ADDRESS, rawTemp, sizeof(rawTemp));
	int16_t temp = (int16_t)(((uint16_t)rawTemp[0] << 0) |
			 	 	 	 	 ((uint16_t)rawTemp[1] << 8));
	return clampOutput((42.5 + (temp / 480.0)), LPS25HB_TEMP_MIN, LPS25HB_TEMP_MAX);
}

float LPS25HB_get_relative_height()
{
	// Remove the oldest value from the sum
	ma_sum -= ma_buff[ma_index];

	// Add the new value to the buffer and to the sum
	ma_buff[ma_index] = riseRate * (1 - pow((LPS25HB_get_press() / referencePressure), exponent));
	ma_sum += ma_buff[ma_index];

	// Update the index to the next position, wrapping around with modulo
	ma_index = (ma_index + 1) % MA_WINDOW;

	// Increment count up to WINDOW_SIZE to avoid dividing by 0
	if (ma_count < MA_WINDOW)
		ma_count++;

	return ma_sum / ma_count;
}
