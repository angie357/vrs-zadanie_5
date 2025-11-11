/**
  ******************************************************************************
  * @file    LPS25HB_map.h
  * @brief   This file provides the register mapping and device addresses
  *          for the LPS25HB pressure sensor.
  * @author  MarekS
  * @created Oct 28, 2024
  *
  ******************************************************************************
  */

#ifndef INC_LPS25HB_MAP_H_
#define INC_LPS25HB_MAP_H_

/* I2C sensor general defines BEGIN */
#define 	LPS25HB_DEVICE_ADDRESS_W		0xBAU
#define 	LPS25HB_DEVICE_ADDRESS_R		0xBBU
#define 	LPS25HB_DEVICE_ADDRESS			LPS25HB_DEVICE_ADDRESS_W

#define		LPS25HB_PRESS_MIN				260
#define		LPS25HB_PRESS_MAX				1260
#define		LPS25HB_TEMP_MIN				-30
#define		LPS25HB_TEMP_MAX				105
/* I2C sensor general defines END */

/* Register address mapping BEGIN */
// 			RESERVED 						0x00->0x07	// -
#define 	LPS25HB_REF_P_XL_ADDRESS		0x08U		// R/W
#define 	LPS25HB_REF_P_L_ADDRESS			0x09U		// R/W
#define 	LPS25HB_REF_P_H_ADDRESS			0x0AU		// R/W
// 			RESERVED 						0x0D->0x0E	// -
#define 	LPS25HB_WHO_AM_I_ADDRESS		0x0FU		// R
#define 	LPS25HB_RES_CONF_ADDRESS		0x10U		// R/W
// 			RESERVED 						0x11->0x1F	// -
#define 	LPS25HB_CTRL_REG1_ADDRESS		0x20U		// R/W
#define 	LPS25HB_CTRL_REG2_ADDRESS		0x21U		// R/W
#define 	LPS25HB_CTRL_REG3_ADDRESS		0x22U		// R/W
#define 	LPS25HB_CTRL_REG4_ADDRESS		0x23U		// R/W
#define 	LPS25HB_INTERRUPT_CFG_ADDRESS	0x24U		// R/W
#define 	LPS25HB_INT_SOURCE_ADDRESS		0x25U		// R
// 			RESERVED 						0x26		// -
#define 	LPS25HB_STATUR_REG_ADDRESS		0x27U		// R
#define 	LPS25HB_PRESS_OUT_XL_ADDRESS	0x28U		// R
#define 	LPS25HB_PRESS_OUT_L_ADDRESS		0x29U		// R
#define 	LPS25HB_PRESS_OUT_H_ADDRESS		0x2AU		// R
#define 	LPS25HB_TEMP_OUT_L_ADDRESS		0x2BU		// R
#define 	LPS25HB_TEMP_OUT_H_ADDRESS		0x2CU		// R
// 			RESERVED 						0x2D		// -
#define 	LPS25HB_FIFO_CTRL_ADDRESS		0x2EU		// R/W
#define 	LPS25HB_FIFO_STATUS_ADDRESS		0x2FU		// R
#define 	LPS25HB_THS_P_L_ADDRESS			0x30U		// R/W
#define 	LPS25HB_THS_P_H_ADDRESS			0x31U		// R/W
// 			RESERVED 						0x32->0x38	// -
#define 	LPS25HB_RPDS_L_ADDRESS			0x39U		// R/W
#define 	LPS25HB_RPDS_H_ADDRESS			0x3AU		// R/W
/* Register address mapping END */

/* Register values BEGIN */
#define 	LPS25HB_WHO_AM_I_VALUE			0xBDU
#define 	LPS25HB_RES_CONF_VALUE			0x0FU		// 0bRRRR 1111 (avg 64C avg 512hPa)
#define		LPS25HB_CTRL_REG1_VALUE			0xC2U		// 0b1100 0010 (ODR 25Hz 25Hz, PD, RESET_AZ)
#define		LPS25HB_CTRL_REG2_VALUE			0x10U		// 0b0001 0000 (FIFO_MEAN_DEC)
#define		LPS25HB_FIFO_CTRL_VALUE			0xDFU		// 0b1101 1111 (FIFO Mean mode, 32 sample moving avg)
/* Register values END */

#endif /* INC_LPS25HB_MAP_H_ */
