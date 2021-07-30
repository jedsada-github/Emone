/**
 ******************************************************************************
 * @file           : wimod_hci.h
 * @abstract       : WiMOD HCI Message Layer
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
#ifndef __WIMOD_HCI_H
#define __WIMOD_HCI_H

/* Includes ------------------------------------------------------------------*/
#include "wimod_serial.h"
#include "wimod_crc16.h"
#include "wimod_slip.h"

/* Private define ------------------------------------------------------------*/
#define WIMOD_HCI_MSG_HEADER_SIZE    (2U)
#define WIMOD_HCI_MSG_PAYLOAD_SIZE   (300U)
#define WIMOD_HCI_MSG_FCS_SIZE       (2U)
#define WIMOD_HCI_RX_BUF_MAX         (255U)

/* Wimod HCI layer return status */
#define WIMOD_HCI_LAYER_FAILED       (0x00000000U) /*!< Wimod HCI layer failed */
#define WIMOD_HCI_LAYER_SUCCESS      (0x00000001U) /*!< Wimod HCI layer OK     */

#define WIMOD_HCI_LOBYTE(x)          (x)
#define WIMOD_HCI_HIBYTE(x)          ((uint16_t)(x) >> 8U)

/* Private HCI Message Structure ---------------------------------------------*/
typedef struct
{
    /*
     * Payload Length Information,
     * this field not transmitted over UART interface !!!
     */
    uint16_t Length;
    uint8_t SapID;                               /* Service Access Point Identifier */
    uint8_t MsgID;                               /* Message Identifier */
    uint8_t Payload[WIMOD_HCI_MSG_PAYLOAD_SIZE]; /* Payload Field */
    uint8_t CRC16[WIMOD_HCI_MSG_FCS_SIZE];       /* Frame Check Sequence Field */
} TWiMOD_HCI_Message;

/* Private typedef -----------------------------------------------------------*/
/* Message receiver callback */
typedef TWiMOD_HCI_Message *(*TWiMOD_HCI_CbRxMessage)(TWiMOD_HCI_Message *rxMessage);

/* Function prototypes -------------------------------------------------------*/
/* Init HCI Layer */
uint8_t WiMOD_HCI_Init(UART_HandleTypeDef *comPort, TWiMOD_HCI_CbRxMessage cbRxMessage, TWiMOD_HCI_Message *rxMessage);

/* Send HCI Message */
int32_t WiMOD_HCI_SendMessage(TWiMOD_HCI_Message *txMessage);

/* Receiver Process */
void WiMOD_HCI_Process(void);

#endif /* __WIMOD_HCI_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
