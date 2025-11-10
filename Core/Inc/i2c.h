/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t I2C_TXIS_Wait(const I2C_TypeDef *I2Cx);
uint8_t I2C_RXNE_Wait(const I2C_TypeDef *I2Cx);
uint8_t I2C_GenerateStop(const I2C_TypeDef *I2Cx);
uint8_t I2C1_MultiByteRead(uint8_t slave_address, uint8_t register_address, uint8_t* i2c_rx_data, uint8_t data_count);
uint8_t I2C1_MultiByteWrite(uint8_t slave_address, uint8_t register_address, uint8_t* i2c_tx_data, uint8_t data_count);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

