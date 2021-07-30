/**
 ******************************************************************************
 * @file           : wimod_slip.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIMOD_SLIP_H
#define __WIMOD_SLIP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Private define ------------------------------------------------------------*/
/* SLIP Protocol Characters */
#define SLIP_END                (0xC0U)
#define SLIP_ESC                (0xDBU)
#define SLIP_ESC_END            (0xDCU)
#define SLIP_ESC_ESC            (0xDDU)

/* SLIP Receiver/Decoder States */
#define SLIPDEC_IDLE_STATE       (0U)
#define SLIPDEC_START_STATE      (1U)
#define SLIPDEC_IN_FRAME_STATE   (2U)
#define SLIPDEC_ESC_STATE        (3U)

/* Slip return status */
#define SLIP_FAILED              (0x00000000U) /*!< SLIP failed       */
#define SLIP_SUCCESS             (0x00000001U) /*!< SLIP OK           */
#define SLIP_LENGTH_ERROR        (-1)          /*!< SLIP Length error */

/* Function prototypes -------------------------------------------------------*/
/* SLIP message receiver callback */
typedef uint8_t *(*TSLIP_CbRxMessage)(uint8_t *message, int32_t length);

/* Declare SLIP Variables ----------------------------------------------------*/
typedef struct
{
    /* Decoder */
    int32_t RxState;
    int32_t RxIndex;
    int32_t RxBufferSize;
    uint8_t *RxBuffer;
    TSLIP_CbRxMessage CbRxMessage;
    /* Encoder */
    int32_t TxIndex;
    int32_t TxBufferSize;
    uint8_t *TxBuffer;
} TSLIP;

/* Init SLIP layer -----------------------------------------------------------*/
void SLIP_Init(TSLIP_CbRxMessage cbRxMessage);

/* Init first receiver buffer ------------------------------------------------*/
uint8_t SLIP_SetRxBuffer(uint8_t *rxBuffer, int32_t rxBufferSize);

/* Encode outgoing Data ------------------------------------------------------*/
int32_t SLIP_EncodeData(uint8_t *dstBuffer, int32_t txBufferSize, uint8_t *srcData, int32_t srcLength);

/* Decode incoming Data ------------------------------------------------------*/
void SLIP_DecodeData(uint8_t *srcData, int32_t srcLength);

#endif /* __WIMOD_SLIP_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
