/**
 ******************************************************************************
 * @file           : slip.c
 * @abstract       : Slip encode/decoder
 * @disclaimer     : This example code is provided by IMST GmbH on an "ASIS"
 *                   basis without any warranties.
 * @maintain by    : Anol Paisal <anol.paisal@emone.co.th> @ 2018
 * @version        : 0.2
 * @date           : 18.05.2016
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "slip.h"

/* SLIP Instance */
static TSLIP SLIP;

/* Function prototypes -------------------------------------------------------*/
static void SLIP_StoreTxByte(uint8_t txByte);
static void SLIP_StoreRxByte(uint8_t rxByte);

/**
 * @brief  Init SLIP decoder
 * @param  
 * @retval none
 */
void SLIP_Init(TSLIP_CbRxMessage cbRxMessage)
{
    /* Init decoder to idle state, no rx-buffer avaliable */
    SLIP.RxState      = SLIPDEC_IDLE_STATE;
    SLIP.RxIndex      = 0U;
    SLIP.RxBuffer     = 0U;
    SLIP.RxBufferSize = 0U;

    /* Save message receiver callback */
    SLIP.CbRxMessage = cbRxMessage;

    /* Init encoder */
    SLIP.TxIndex      = 0U;
    SLIP.TxBuffer     = 0U;
    SLIP.TxBufferSize = 0U;
}

/**
 * @brief  Store a byte into TxBuffer
 * @param  
 * @retval none
 */
static void SLIP_StoreTxByte(uint8_t txByte)
{
    if (SLIP.TxIndex < SLIP.TxBufferSize)
    {
        SLIP.TxBuffer[SLIP.TxIndex++] = txByte;
    }
}

/**
 * @brief  Encode a messages into dstBuffer
 * @param  
 * @retval int32_t
 */
int32_t SLIP_EncodeData(uint8_t *dstBuffer, int32_t dstBufferSize, uint8_t *srcData, int32_t srcLength)
{
    /* Init TxBuffer */
    SLIP.TxBuffer = dstBuffer;

    /* Init TxIndex */
    SLIP.TxIndex = 0U;

    /* Init size */
    SLIP.TxBufferSize = dstBufferSize;

    /* Send start of SLIP message */
    SLIP_StoreTxByte(SLIP_END);

    /* Iterate over all message bytes */
    while (srcLength--)
    {
        switch (*srcData)
        {
            case SLIP_END:
                SLIP_StoreTxByte(SLIP_ESC);
                SLIP_StoreTxByte(SLIP_ESC_END);
                break;

            case SLIP_ESC:
                SLIP_StoreTxByte(SLIP_ESC);
                SLIP_StoreTxByte(SLIP_ESC_ESC);
                break;

            default:
                SLIP_StoreTxByte(*srcData);
                break;
        }

        /* Next byte */
        srcData++;
    }

    /* Send end of SLIP message */
    SLIP_StoreTxByte(SLIP_END);

    /* Length ok ? */
    if (SLIP.TxIndex <= SLIP.TxBufferSize)
    {
        return SLIP.TxIndex;
    }

    /* Return tx length error */
    return SLIP_LENGTH_ERROR;
}

/**
 * @brief  Configure a rx-buffer and enable receiver/decoder
 * @param  
 * @retval uint8_t
 */
uint8_t SLIP_SetRxBuffer(uint8_t *rxBuffer, int32_t rxBufferSize)
{
    /* Receiver in IDLE state and client already registered ? */
    if ((SLIP.RxState == SLIPDEC_IDLE_STATE) && SLIP.CbRxMessage)
    {
        /* Same buffer params */
        SLIP.RxBuffer = rxBuffer;
        SLIP.RxBufferSize = rxBufferSize;

        /* Enable decoder */
        SLIP.RxState = SLIPDEC_START_STATE;

        return SLIP_SUCCESS;
    }

    return SLIP_SUCCESS;
}

/**
 * @brief  Store SLIP decoded rxByte
 * @param  
 * @retval none
 */
static void SLIP_StoreRxByte(uint8_t rxByte)
{
    if (SLIP.RxIndex < SLIP.RxBufferSize)
    {
        SLIP.RxBuffer[SLIP.RxIndex++] = rxByte;
    }
}

/**
 * @brief  Process received byte stream
 * @param  
 * @retval none
 */
void SLIP_DecodeData(uint8_t *srcData, int32_t srcLength)
{
    /* Iterate over all received bytes */
    while (srcLength--)
    {
        /* Get rxByte */
        uint8_t rxByte = *srcData++;

        /* Decode according to current state */
        switch (SLIP.RxState)
        {
            case SLIPDEC_START_STATE:
                /* Start of SLIP frame ? */
                if (rxByte == SLIP_END)
                {
                    /* Init read index */
                    SLIP.RxIndex = 0U;

                    /* Next state */
                    SLIP.RxState = SLIPDEC_IN_FRAME_STATE;
                }
                break;
            case SLIPDEC_IN_FRAME_STATE:
                switch (rxByte)
                {
                    case SLIP_END:
                        /* Data received ? */
                        if (SLIP.RxIndex > 0U)
                        {
                            /* Yes, receiver registered ? */
                            if (SLIP.CbRxMessage)
                            {
                                /* Yes, call message receive */
                                SLIP.RxBuffer = (*SLIP.CbRxMessage)(SLIP.RxBuffer, SLIP.RxIndex);

                                /* New buffer available ? */
                                if (!SLIP.RxBuffer)
                                {
                                    SLIP.RxState = SLIPDEC_IDLE_STATE;
                                }
                                else
                                {
                                    SLIP.RxState = SLIPDEC_START_STATE;
                                }
                            }
                            else
                            {
                                /* Disable decoder, temp. no buffer avaliable */
                                SLIP.RxState = SLIPDEC_IDLE_STATE;
                            }
                        }

                        /* Init read index */
                        SLIP.RxIndex = 0U;
                        break;

                    case SLIP_ESC:
                        /* Enter escape sequence state */
                        SLIP.RxState = SLIPDEC_ESC_STATE;
                        break;

                    default:
                        /* Store byte */
                        SLIP_StoreRxByte(rxByte);
                        break;
                }
                break;
            case SLIPDEC_ESC_STATE:
                switch (rxByte)
                {
                    case SLIP_ESC_END:
                        SLIP_StoreRxByte(SLIP_END);
                        /* Quit escape sequence state */
                        SLIP.RxState = SLIPDEC_IN_FRAME_STATE;
                        break;
                    case SLIP_ESC_ESC:
                        SLIP_StoreRxByte(SLIP_ESC);
                        /* Quit escape sequence state */
                        SLIP.RxState = SLIPDEC_IN_FRAME_STATE;
                        break;
                    default:
                        /* Abort frame receiption */
                        SLIP.RxState = SLIPDEC_START_STATE;
                        break;
                }
                break;
            default:
                break;
        }
    }
}

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
