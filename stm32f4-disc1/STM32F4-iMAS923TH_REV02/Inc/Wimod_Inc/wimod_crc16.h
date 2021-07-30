/**
 ******************************************************************************
 * @file           : wimod_crc16.h
 * @abstract       : crc16 calculation
 * @disclaimer     : This example code is provided by IMST GmbH on an "ASIS"
 *                   basis without any warranties.
 * @version        : 0.2
 * @date           : 18.05.2016
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIMOD_CRC16_H
#define __WIMOD_CRC16_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Private define ------------------------------------------------------------*/
#define WIMOD_CRC16_INIT_VALUE   (0xFFFFU) /* Initial value for CRC algorithem */
#define WIMOD_CRC16_GOOD_VALUE   (0x0F47U) /* Constant compare value for check */
#define WIMOD_CRC16_POLYNOM      (0x8408U) /* 16-BIT CRC CCITT POLYNOM */

/* Function prototypes -------------------------------------------------------*/
/* Calculate CRC16 -----------------------------------------------------------*/
uint16_t WiMOD_CRC16_Calc(uint8_t *data, uint16_t length, uint16_t initVal);

/* Calculate and check CRC16 -------------------------------------------------*/
uint8_t WiMOD_CRC16_Check(uint8_t *data, uint16_t length, uint16_t initVal);

#endif /* __WIMOD_CRC16_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
