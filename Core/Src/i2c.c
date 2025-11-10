/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x2000090E;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */
  LL_I2C_DisableAutoEndMode(I2C1);
  LL_I2C_Enable(I2C1);
  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */
uint8_t I2C_TXIS_Wait(const I2C_TypeDef *I2Cx)
{
	while(!LL_I2C_IsActiveFlag_TXIS(I2Cx))
	{
		if (LL_I2C_IsActiveFlag_NACK(I2Cx))
		{
			LL_I2C_GenerateStopCondition((I2C_TypeDef *)I2Cx);
			return 1; // Error: NACK received
		}
	}
	return 0; // Success: ACK received
}

uint8_t I2C_RXNE_Wait(const I2C_TypeDef *I2Cx)
{
	while(!LL_I2C_IsActiveFlag_RXNE(I2Cx))
	{
		if (LL_I2C_IsActiveFlag_NACK(I2Cx) || LL_I2C_IsActiveFlag_ARLO(I2Cx))
		{
			LL_I2C_GenerateStopCondition((I2C_TypeDef *)I2Cx);
			return 1; // Error: NACK received
		}
	}
	return 0; // Success: ACK received
}

uint8_t I2C_GenerateStop(const I2C_TypeDef *I2Cx)
{
	LL_I2C_GenerateStopCondition((I2C_TypeDef *)I2Cx);			// Generate STOP condition.
	while (!LL_I2C_IsActiveFlag_STOP(I2Cx))		// Wait until the STOP condition has been transmitted.
	{
		if (LL_I2C_IsActiveFlag_NACK(I2Cx))
		{
			LL_I2C_GenerateStopCondition((I2C_TypeDef *)I2Cx);
			return 1; // Error: NACK received
		}
	}
	LL_I2C_ClearFlag_STOP(I2C1);				// Clear the STOP flag to be ready for the next communication.
	return 0; 									// Success: STOP generated successfully
}

uint8_t I2C1_MultiByteRead(uint8_t slave_address, uint8_t register_address, uint8_t* i2c_rx_data, uint8_t data_count)
{
	// If we want to receive multiple bytes the register_adress needs to be modified such that MSB is 1
	if(data_count > 1)
		register_address |= 0x80;

	// Byte 1. Initiate communication with START bit, SLAVE ADDRESS with a WRITE bit set up to send register address
	LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	// Wait until TXIS flag is set.
	if(I2C_TXIS_Wait(I2C1))
		return 1; // Error: NACK received, Connection problem, Possible timeout

	// Byte 2: Transmit the register address
	LL_I2C_TransmitData8(I2C1, register_address);
	// Wait until TC flag is set to confirm all data is sent
	while(!LL_I2C_IsActiveFlag_TC(I2C1)) {}

	// Byte 3: Send REPEATED START with read request
	LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, data_count, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);

	// Byte n: Read n "data_count" of data bytes
	for (int i = 0; i < data_count; i++)
	{
		// Wait until RXNE (Receive Not Empty) flag is set, indicating data is available
		if(I2C_RXNE_Wait(I2C1))
			return 1; // Error: NACK received, Connection problem, Possible timeout
		// Read data from the data register
		i2c_rx_data[i] = LL_I2C_ReceiveData8(I2C1);
	}

	// Generate STOP condition after all data is received
	I2C_GenerateStop(I2C1);

	return 0; // Success
}

uint8_t I2C1_MultiByteWrite(uint8_t slave_address, uint8_t register_address, uint8_t* i2c_tx_data, uint8_t data_count)
{
	// If we want to send multiple bytes the register_adress needs to be modified such that MSB is 1
	if(data_count > 1)
		register_address |= 0x80;

	// Byte 1. Initiate communication with START bit, SLAVE ADDRESS with a WRITE bit set up to send register address
	LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, data_count + 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	// Wait until TXIS flag is set.
	if(I2C_TXIS_Wait(I2C1))
		return 1; // Error: NACK received, Connection problem, Possible timeout

	// Byte 2: Transmit the register address
	LL_I2C_TransmitData8(I2C1, register_address);

	// Byte n: Send n "data_count" of data bytes
	for (int i = 0; i < data_count; i++)
	{
		// Wait until TXIS flag is set.
		if(I2C_TXIS_Wait(I2C1))
			return 1; // Error: NACK received, Connection problem, Possible timeout
		// Transmit data byte.
		LL_I2C_TransmitData8(I2C1, i2c_tx_data[i]);
	}

	// Wait until TC flag is set to confirm all data is sent before generating STOP.
	while(!LL_I2C_IsActiveFlag_TC(I2C1)) {}

	// Generate STOP condition after all data is received
	I2C_GenerateStop(I2C1);

	return 0; // Success
}
/* USER CODE END 1 */
