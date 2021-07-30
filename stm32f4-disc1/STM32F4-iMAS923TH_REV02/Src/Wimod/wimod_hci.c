/**
 ******************************************************************************
 * @file           : wimod_hci.c
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

/* Includes ------------------------------------------------------------------*/
#include "wimod_hci.h"

/* Function prototypes -------------------------------------------------------*/
/* SLIP Message Receiver Callback */
static uint8_t *WiMOD_HCI_ProcessRxMessage(uint8_t *rxData, int32_t rxLength);

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    uint32_t CRCErrors;                 /* CRC Error counter */
    TWiMOD_HCI_Message *RxMessage;      /* RxMessage */
    TWiMOD_HCI_CbRxMessage CbRxMessage; /* Receiver callback */
} TWiMOD_HCI_MsgLayer;

/* Private variables ---------------------------------------------------------*/
/* Reserve HCI Instance */
static TWiMOD_HCI_MsgLayer HCI;

/* Reserve one TxBuffer */
static uint8_t TxBuffer[((sizeof(TWiMOD_HCI_Message) * 2U) + 2U)];

/**
 * @brief  Init HCI Message layer
 * @param
 * @retval uint8_t
 */
uint8_t WiMOD_HCI_Init(UART_HandleTypeDef *comPort,
                       TWiMOD_HCI_CbRxMessage cbRxMessage, /* HCI msg recv. callback */
                       TWiMOD_HCI_Message *rxMessage)      /* intial rxMessage */
{
    /* Init error counter */
    HCI.CRCErrors = 0U;

    /* Save receiver callback */
    HCI.CbRxMessage = cbRxMessage;

    /* Save RxMessage */
    HCI.RxMessage = rxMessage;

    /* Init SLIP */
    SLIP_Init(WiMOD_HCI_ProcessRxMessage);

    /* Init first RxBuffer to SAP_ID of HCI message, size without 16-Bit Length Field */
    SLIP_SetRxBuffer(&rxMessage->SapID, sizeof(TWiMOD_HCI_Message) - sizeof(uint16_t));

    /* Init serial device */
    return (WiMOD_SERIAL_Open(comPort, WIMOD_SERIAL_BAUDRATE_115200,
            WIMOD_SERIAL_DATABITS_8, WIMOD_SERIAL_PARITY_NONE));
}

/**
 * @brief  Send a HCI message (with or without payload)
 * @param
 * @retval int32_t
 */
int32_t WiMOD_HCI_SendMessage(TWiMOD_HCI_Message *txMessage)
{
    uint8_t i        = 0U;
    int8_t  txLength = 0;

    /*
     * 1.  Check parameter
     * 1.1 Check ptr
     */
    if (!txMessage)
    {
        /* Error */
        return WIMOD_HCI_LAYER_FAILED;
    }

    /* 2. Calculate CRC16 over header and optional payload */
    uint16_t crc16 = WiMOD_CRC16_Calc(&txMessage->SapID,
                                      txMessage->Length + WIMOD_HCI_MSG_HEADER_SIZE,
                                      WIMOD_CRC16_INIT_VALUE);

    /* 2.1 Get 1's complement !!! */
    crc16 = ~crc16;

    /* 2.2 Attach CRC16 and correct length, LSB first */
    txMessage->Payload[txMessage->Length] = WIMOD_HCI_LOBYTE(crc16);
    txMessage->Payload[txMessage->Length + 1U] = WIMOD_HCI_HIBYTE(crc16);

    /*
     * 3. Perform SLIP encoding
     *    - start transmission with SAP ID
     *    - correct length by header size
     */
    txLength = SLIP_EncodeData(TxBuffer, sizeof(TxBuffer), &txMessage->SapID,
                               txMessage->Length + WIMOD_HCI_MSG_HEADER_SIZE + WIMOD_HCI_MSG_FCS_SIZE);

    /* Message ok ? */
    if (txLength > 0U)
    {
        /* Send wakeup chars */
        for (i = 0U; i < 40U; i++)
        {
            WiMOD_SERIAL_SendByte(SLIP_END);
        }

        /* 4. Send octet sequence over serial device */
        if (WiMOD_SERIAL_SendData(TxBuffer, txLength) > 0U)
        {
            /* Return ok */
            return WIMOD_HCI_LAYER_SUCCESS;
        }
    }

    /* Error - SLIP layer couldn't encode message - buffer to small ? */
    return WIMOD_HCI_LAYER_FAILED;
}

/**
 * @brief  Read incoming serial data
 * @param
 * @retval none
 */
void WiMOD_HCI_Process(void)
{
    uint8_t rxBuf[WIMOD_HCI_RX_BUF_MAX] = { 0U };
    int32_t rxLength = 0U;

    /* Read small chunk of data */
    rxLength = WiMOD_SERIAL_ReadData(rxBuf, sizeof(rxBuf));

    /* data available ? */
    if (rxLength > 0U)
    {
        /*
         * Yes, forward to SLIP decoder, decoded SLIP message will be passed to
         * function "WiMOD_HCI_ProcessRxMessage"
         */
        SLIP_DecodeData(rxBuf, rxLength);
    }
}

/**
 * @brief  process received SLIP message and return new rxBuffer
 * @param
 * @retval uint8_t
 */
static uint8_t *WiMOD_HCI_ProcessRxMessage(uint8_t *rxData, int32_t rxLength)
{
    /* Check min length */
    if (rxLength >= (WIMOD_HCI_MSG_HEADER_SIZE + WIMOD_HCI_MSG_FCS_SIZE))
    {
        if (WiMOD_CRC16_Check(rxData, rxLength, WIMOD_CRC16_INIT_VALUE))
        {
            /* Receiver registered ? */
            if (HCI.CbRxMessage)
            {
                /* Yes, complete message info */
                HCI.RxMessage->Length = rxLength - (WIMOD_HCI_MSG_HEADER_SIZE + WIMOD_HCI_MSG_FCS_SIZE);

                /* Call upper layer receiver and save new RxMessage */
                HCI.RxMessage = (*HCI.CbRxMessage)(HCI.RxMessage);
            }
        }
        else
        {
            HCI.CRCErrors++;
        }
    }

    /* Free HCI message available ? */
    if (HCI.RxMessage)
    {
        /* Yes, return pointer to first byte */
        return &HCI.RxMessage->SapID;
    }

    /* Error, disable SLIP decoder */
    return WIMOD_HCI_LAYER_FAILED;
}

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
