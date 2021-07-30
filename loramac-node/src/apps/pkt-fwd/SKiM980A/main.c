/*!
 * \file      main.c
 *
 * \brief     Packet forwarding  test
 *
 * \remark    -
 *
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *
 *
 *
 * \endcode
 *
 * \author    Jedsada Udomsed
 *
 */
#include <stdio.h>
#include "board.h"
#include "gpio.h"
#include "timer.h"
#include "radio.h"
#include "uart-board.h"

#define DEBUG_PKT                      (1U)

#define RF_FREQUENCY                   (923200000UL) /* Hz */
#define LORA_BANDWIDTH                 (0U)          /* [0: 125 kHz, */
                                                     /*  1: 250 kHz, */
                                                     /*  2: 500 kHz, */
                                                     /*  3: Reserved] */
#define LORA_SPREADING_FACTOR          (7U)          /* [SF7..SF12] */
#define LORA_CODINGRATE                (1U)          /* [1: 4/5, */
                                                     /*  2: 4/6, */
                                                     /*  3: 4/7, */
                                                     /*  4: 4/8] */
#define LORA_SYMBOL_TIMEOUT            (5U)          /* Symbols */
#define LORA_PREAMBLE_LENGTH           (8U)          /* Same for Tx and Rx */
#define LORA_FIX_LENGTH_PAYLOAD_ON     (false)
#define LORA_IQ_INVERSION_ON           (false)

Uart_t Uart1;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*
 *  Received file computed CRC32
 */
static volatile uint32_t FileRxCrc = 0;

/*!
 * LED GPIO pins objects
 */
extern Gpio_t Led4;

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * Main application entry point.
 */
int main( void )
{
    uint16_t i = 0U;
    uint8_t tx_buf[255] = { '\0' };
    uint8_t c = 0;
    uint8_t size = 0;

    /* Target board initialization */
    BoardInitMcu();
    BoardInitPeriph();

    /* Radio initialization */
    RadioEvents.RxDone = OnRxDone;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig
        (
         MODEM_LORA,
         LORA_BANDWIDTH,
         LORA_SPREADING_FACTOR,
         LORA_CODINGRATE,
         0,
         LORA_PREAMBLE_LENGTH,
         LORA_SYMBOL_TIMEOUT,
         LORA_FIX_LENGTH_PAYLOAD_ON,
         0,
         true,
         0,
         0,
         LORA_IQ_INVERSION_ON,
         true
        );
    Radio.SetRxConfig
        (
         MODEM_LORA, LORA_BANDWIDTH,
         LORA_SPREADING_FACTOR,
         LORA_CODINGRATE,
         0U,
         LORA_PREAMBLE_LENGTH,
         LORA_SYMBOL_TIMEOUT,
         LORA_FIX_LENGTH_PAYLOAD_ON,
         0U,
         true,
         0U,
         0U,
         LORA_IQ_INVERSION_ON,
         true
        );
    Radio.SetMaxPayloadLength( MODEM_LORA, 255U);

    /* Continuous Rx */
    Radio.Rx(0U);

    while (1U)
    {
#if 0
        i = 0;
        while (UartGetChar( &Uart1, &c) != 0)
        {
            UartGetChar( &Uart1, &c );
            tx_buf[i] = c;
            i++;
        }
        Radio.Send(tx_buf, i);
#endif
#if 1
        UartMcuGetChar( &Uart1, &c);
        if (c == '$')
        {
            UartMcuGetChar( &Uart1, &c);
            size = c;
            for (i = 0; UartMcuGetChar( &Uart1, &c) != 0; i++)
            {
                tx_buf[i] = c;
            }
            Radio.Send(tx_buf, size);
        }
#endif

        BoardLowPowerHandler( );
    }
}

void OnTxDone( void )
{
  /* Do nothing */
}


void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    uint8_t rx_buf[8] = { '\0' };
#if 1
    rx_buf[0] = '$';
    rx_buf[1] = size << 8;
    rx_buf[2] = size;
    rx_buf[3] = rssi << 8;
    rx_buf[4] = rssi;
    rx_buf[5] = snr;
    rx_buf[6] = '\r';
    rx_buf[7] = '\n';

    UartMcuPutBuffer(&Uart1, rx_buf, 3);
    UartMcuPutBuffer(&Uart1, payload, size);
    UartMcuPutBuffer(&Uart1, rx_buf + 3, 6);
#endif
#if 0
    uint16_t i = 0U;
    printf("Payload Size: %d\n", size);
    printf("Rssi: %d\n", rssi);
    printf("Snr: %d\n", snr);
    printf("MHRD: 0x%02X\n", payload[0U] >> 4U);
    printf("DevAddr: 0x");
    printf("%02X", payload[4U]);
    printf("%02X", payload[3U]);
    printf("%02X", payload[2U]);
    printf("%02X", payload[1U]);
    printf("\n");
    printf("FCtrl: 0x%02X\n", payload[5U]);
    printf("FCnt: %d\n", payload[6U] | (payload[7U] << 8U));
    printf("Fport: %d\n", payload[8U]);
    printf("MIC: 0x");
    printf("%02X", payload[size - 1U]);
    printf("%02X", payload[size - 2U]);
    printf("%02X", payload[size - 3U]);
    printf("%02X", payload[size - 4U]);
    printf("\n");
    printf("FRMPayload: ");
    for (i = 9U; i < (size - 4U); i++)
    {
        printf("%02X ", payload[i]);
    }
    printf("\n");
    printf("PHYPayload: ");
    for (i = 0U; i < size; i++)
    {
        printf("%02X", payload[i]);
    }
    printf("\n");
    printf("PHYPayload:\n");
    for (i = 0U; i < size; i++)
    {
        printf("[%02d]: %02X\n", i, payload[i]);
    }
    printf("\r\n");

    static uint8_t ledState = 1U;
    /* Toggle LED 4 */
    ledState ^= 1U;
    GpioWrite( &Led4, ledState );
#endif
}
