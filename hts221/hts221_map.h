/**
  ******************************************************************************
  * @file    HTS221_map.h
  * @brief   This file provides the register mapping and device addresses
  *          for the HTS221 humidity and temperature sensor.
  * @author  MarekS
  * @created Oct 28, 2024
  *
  ******************************************************************************
  */

#ifndef INC_HTS221_MAP_H_
#define INC_HTS221_MAP_H_

/* I2C sensor general defines BEGIN */
#define 	HTS221_DEVICE_ADDRESS_W			0xBEU
#define 	HTS221_DEVICE_ADDRESS_R			0xBFU
#define 	HTS221_DEVICE_ADDRESS			HTS221_DEVICE_ADDRESS_W

#define		HTS221_HUMI_MIN					0
#define		HTS221_HUMI_MAX					100
#define		HTS221_TEMP_MIN					-40
#define		HTS221_TEMP_MAX					120
/* I2C sensor general defines END */

/* Register address mapping BEGIN */
// 			RESERVED 						0x00->0x0E	// -
#define 	HTS221_WHO_AM_I_ADDRESS			0x0FU		// R
#define 	HTS221_AV_CONF_ADDRESS			0x10U		// R/W
// 			RESERVED 						0x11->0x1C	// -
#define 	HTS221_CTRL_REG1_ADDRESS		0x20U		// R/W
#define 	HTS221_CTRL_REG2_ADDRESS		0x21U		// R/W
#define 	HTS221_CTRL_REG3_ADDRESS		0x22U		// R/W
// 			RESERVED 						0x23->0x26	// -
#define 	HTS221_STATUR_REG_ADDRESS		0x27U		// R
#define 	HTS221_HUMIDITY_OUT_L_ADDRESS	0x28U		// R
#define 	HTS221_HUMIDITY_OUT_H_ADDRESS	0x29U		// R
#define 	HTS221_TEMP_OUT_L_ADDRESS		0x2AU		// R
#define 	HTS221_TEMP_OUT_H_ADDRESS		0x2BU		// R
// 			RESERVED 						0x2C->0x2F	// -
#define 	HTS221_CALIB_0_ADDRESS			0x30U		// R/W | Do not modify
#define 	HTS221_CALIB_1_ADDRESS			0x31U		// R/W | Do not modify
#define 	HTS221_CALIB_2_ADDRESS			0x32U		// R/W | Do not modify
#define 	HTS221_CALIB_3_ADDRESS			0x33U		// R/W | Do not modify
// 			RESERVED 						0x34		// -
#define 	HTS221_CALIB_5_ADDRESS			0x35U		// R/W | Do not modify
#define 	HTS221_CALIB_6_ADDRESS			0x36U		// R/W | Do not modify
#define 	HTS221_CALIB_7_ADDRESS			0x37U		// R/W | Do not modify
// 			RESERVED 						0x38->0x39	// -
#define 	HTS221_CALIB_A_ADDRESS			0x3AU		// R/W | Do not modify
#define 	HTS221_CALIB_B_ADDRESS			0x3BU		// R/W | Do not modify
#define 	HTS221_CALIB_C_ADDRESS			0x3CU		// R/W | Do not modify
#define 	HTS221_CALIB_D_ADDRESS			0x3DU		// R/W | Do not modify
#define 	HTS221_CALIB_E_ADDRESS			0x3EU		// R/W | Do not modify
#define 	HTS221_CALIB_F_ADDRESS			0x3FU		// R/W | Do not modify
/* Register address mapping END */

/* Register values BEGIN */
#define 	HTS221_WHO_AM_I_VALUE			0xBCU
#define		HTS221_CTRL_REG1_VALUE			0x83U		// 0b1RRR R011 (ODR 12.5Hz 12.5Hz, PD)
/* Register values END */

#endif /* INC_HTS221_MAP_H_ */
