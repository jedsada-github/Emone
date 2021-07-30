//------------------------------------------------------------------------------
//
//	File:		SerialDevice.h
//
//	Abstract:	Serial Device Abstraction
//
//	Version:	0.1
//
//	Date:		18.05.2016
//
//	Disclaimer:	This example code is provided by IMST GmbH on an "AS
//IS" basis 				without any warranties.
//
//	Maintain by : Anol Paisal <anol.paisal@emone.co.th> @ 2018
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
//  Include Files
//
//------------------------------------------------------------------------------

#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "usart.h"
#define Baudrate_9600 9600
#define Baudrate_115200 115200
#define DataBits_7 UART_WORDLENGTH_7B
#define DataBits_8 UART_WORDLENGTH_8B
#define Parity_Even UART_PARITY_EVEN
#define Parity_None UART_PARITY_NONE

typedef uint8_t UINT8;
typedef uint32_t UINT32;

#define USARTTEXTSTRINGSIZE                                                   \
    64 //!< Max number of characters can be entered from the PC.

/**
 * @brief The hexdecimal format to be used
 */
typedef enum
{
    HALFBYTE_F,  //!< 1 hex digit
    BYTE_F,      //!< 2 hex digits
    WORD_F,      //!< 4 hex digits
    DOUBLEWORD_F //!< 8 hex digits
} eHexFormat;

// open serial device
bool SerialDevice_Open (UART_HandleTypeDef *huart, UINT32 baudRate,
        int dataBits, UINT8 parity);

// close serial device
bool SerialDevice_Close ();

// send single byte
int SerialDevice_SendByte (UINT8 txByte);

// send data
int SerialDevice_SendData (UINT8 *txBuffer, int txLength);

// read data
int SerialDevice_ReadData (UINT8 *rxBuffer, int rxBufferSize);

void USART_ShowMenu (void);
void USART_TxWelcomeMessage (void);
void USART_Transmit_Data (UART_HandleTypeDef *huart, uint8_t *data);
void USART_Transmit (UART_HandleTypeDef *huart, const char *TextString);
void USART_CheckAppCmd (void);
void USART_ITCharManager (UART_HandleTypeDef *huart);
void num2str (uint32_t nbr, uint8_t *str);
uint8_t *num2hex (uint32_t num, eHexFormat HexFormat);

#endif // SERIAL_DEVICE_H
