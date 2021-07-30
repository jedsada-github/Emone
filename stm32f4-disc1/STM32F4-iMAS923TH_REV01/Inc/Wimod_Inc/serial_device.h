/**
 ******************************************************************************
 * @file           : serial_device.h
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
#ifndef __SERIAL_DEVICE_H
#define __SERIAL_DEVICE_H


/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define Baudrate_9600           (9600U)
#define Baudrate_115200         (115200U)
#define DataBits_7              UART_WORDLENGTH_7B
#define DataBits_8              UART_WORDLENGTH_8B
#define Parity_Even             UART_PARITY_EVEN
#define Parity_None             UART_PARITY_NONE

/* Max number of characters can be entered from the PC. ----------------------*/
#define USARTTEXTSTRINGSIZE     (64U)

/* Private define ------------------------------------------------------------*/
/* Serial device return status -----------------------------------------------*/
#define SERIAL_DEVICE_FAILED    (0U) /*!< Serial device failed  */
#define SERIAL_DEVICE_SUCCESS   (1U) /*!< Serial device success */

/* Function prototypes -------------------------------------------------------*/
uint8_t SerialDevice_Open(UART_HandleTypeDef *huart, uint32_t baudRate, 
        int32_t dataBits, uint8_t parity);
uint8_t SerialDevice_Close();
uint8_t SerialDevice_SendByte(uint8_t txByte);
int32_t SerialDevice_SendData(uint8_t *txBuffer, int32_t txLength);
int32_t SerialDevice_ReadData(uint8_t *rxBuffer, int32_t rxBufferSize);
uint8_t SerialDevice_Printf(const char* fmt, ...);
void SerialDevice_CheckAppCmd(void);
extern uint8_t ABP_ACTIVATE_PAYLOAD[36U];
extern uint8_t OTA_ACTIVATE_PAYLOAD[24];

#endif /* __SERIAL_DEVICE_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
