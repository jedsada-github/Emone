/**
 ******************************************************************************
 * @file           : wimod_serial.h
 * @abstract       : Serial Device Abstraction
 * @disclaimer     : This example code is provided by IMST GmbH on an "ASIS"
 *                   basis without any warranties.
 * @maintain by    : Anol Paisal <anol.paisal@emone.co.th> @ 2018
 * @version        : 0.1
 * @date           : 18.05.2016
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIMOD_SERIAL_H
#define __WIMOD_SERIAL_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define WIMOD_SERIAL_BAUDRATE_9600     (9600U)
#define WIMOD_SERIAL_BAUDRATE_115200   (115200U)
#define WIMOD_SERIAL_DATABITS_7        UART_WORDLENGTH_7B
#define WIMOD_SERIAL_DATABITS_8        UART_WORDLENGTH_8B
#define WIMOD_SERIAL_PARITY_EVEN       UART_PARITY_EVEN
#define WIMOD_SERIAL_PARITY_NONE       UART_PARITY_NONE

/* Max number of characters can be entered from the PC. ----------------------*/
#define WIMOD_SERIALTEXTSTRINGSIZE     (64U)

/* Private define ------------------------------------------------------------*/
/* Serial device return status -----------------------------------------------*/
#define WIMOD_SERIAL_FAILED            (0U) /*!< Wimod Serial device failed  */
#define WIMOD_SERIAL_SUCCESS           (1U) /*!< Wimod Serial device success */

/* Function prototypes -------------------------------------------------------*/
uint8_t WiMOD_SERIAL_Open(UART_HandleTypeDef *huart, const uint32_t baudrate,
        const uint32_t databits, const uint32_t parity);
uint8_t WiMOD_SERIAL_Close();
uint8_t WiMOD_SERIAL_SendByte(uint8_t txByte);
int32_t WiMOD_SERIAL_SendData(uint8_t *txBuffer, int32_t txLength);
int32_t WiMOD_SERIAL_ReadData(uint8_t *rxBuffer, int32_t rxBufferSize);
uint8_t WiMOD_SERIAL_Printf(const char *fmt, ...);
void    WiMOD_SERIAL_CheckAppCmd(void);

extern const uint8_t DEV_EUI[8U];
extern const uint8_t ABP_ACTIVATE_PAYLOAD[36U];
extern const uint8_t OTA_ACTIVATE_PAYLOAD[24U];

#endif /* __WIMOD_SERIAL_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
