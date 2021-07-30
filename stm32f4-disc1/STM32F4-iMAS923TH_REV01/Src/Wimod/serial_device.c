/**
 ******************************************************************************
 * @file           : serial_device.c
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

/* Includes ------------------------------------------------------------------*/
#include "serial_device.h"

/* Function prototypes -------------------------------------------------------*/
static void Ping(void);
static void GetNwkStatus(void);
static void GetCustomCFG(void);
static void SetCustomCFG(void);
static void ActivateABP(void);
static void ActivateOTA(void);
static void Reactivate(void);
static void Deactivate(void);
static void SetRadioStack(void);
static void GetRadioStack(void);
static void GetSupportBand(void);
static void GetDevEUI(void);
static void SetDevEUI(void);
static void GetOPMODE(void);
static void SetOPMODE(void);
static void ClearOPMODE(void);
static void ClearRTCAlarm(void);
static void GetRTC(void);
static void SetRTC(void);
static void GetAlarm(void);
static void SetAlarm(void);
static void GetFirmwareVersion(void);
static void GetDeviceInfo(void);
static void GetDeviceStatus(void);
static void Reset(void);
static void Join(void);
static void SendUData(void);
static void SendCData(void);
static void FactoryReset(void);
static void GetLinkADR(void);
static void SetLinkADR(void);

/* Private variables ---------------------------------------------------------*/
uint8_t UsartTextString[USARTTEXTSTRINGSIZE];

/**
 * @brief  Open serial device
 * @param  
 * @retval uint8_t
 */
uint8_t SerialDevice_Open(UART_HandleTypeDef *huart, uint32_t baudRate, int32_t dataBits, uint8_t parity)
{
    uint8_t ret = 0U;

    /* Init uart struture */
    huart->Init.BaudRate = baudRate;
    huart->Init.WordLength = dataBits;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = parity;

    if (HAL_UART_Init(huart) == HAL_OK)
    {
        return SERIAL_DEVICE_SUCCESS;
    }
    else
    {
        ret = SerialDevice_Close();
    }

    /* Error */
    return ret;
}

/**
 * @brief  close serial device
 * @param  
 * @retval uint8_t
 */
uint8_t SerialDevice_Close()
{
    /* Deinit uart struture */
    HAL_UART_DeInit(&huart3);

    /* Error */
    return SERIAL_DEVICE_FAILED;
}

/**
 * @brief  Send data via UART3
 * @param  
 * @retval int32_t
 */
int32_t SerialDevice_SendData(uint8_t *txBuffer, int32_t txLength)
{
    /* Transmit the data via UART3 */ 
    if (HAL_UART_Transmit(&huart3, txBuffer, txLength, HAL_MAX_DELAY) != HAL_ERROR)
    {
        return txLength;
    }

    /* Error */
    return SERIAL_DEVICE_FAILED;
}

/**
 * @brief  Send single byte via UART3
 * @param  
 * @retval uint8_t
 */
uint8_t SerialDevice_SendByte(uint8_t txByte)
{
    /* Transmit data 1 byte via UART3 */ 
    if (HAL_UART_Transmit(&huart3, &txByte, 1U, HAL_MAX_DELAY) != HAL_ERROR)
    {
        return SERIAL_DEVICE_SUCCESS;
    }

    /* Error */
    return SERIAL_DEVICE_FAILED;
}

/**
 * @brief  Read data via UART3
 * @param  
 * @retval int32_t
 */
int32_t SerialDevice_ReadData(uint8_t *rxBuffer, int32_t rxBufferSize)
{
    /* Read the data via UART3 */ 
    if (HAL_UART_Receive(&huart3, rxBuffer, rxBufferSize, 500U) != HAL_ERROR)
    {
        return rxBufferSize;
    }

    /* Error */
    return SERIAL_DEVICE_FAILED;
}

/**
 * @brief  Print data via UART2
 * @param  
 * @retval uint8_t
 */
uint8_t SerialDevice_Printf(const char* fmt, ...)
{
    char buff[USART2_TX_BUFF_MAX];
    va_list args;

    if (fmt == NULL)
    {
        /* Error */
        return SERIAL_DEVICE_FAILED;
    }

    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
    va_end(args);

    return SERIAL_DEVICE_SUCCESS;
}

/**
 * @brief  Check if any Application Command for L6470 has been entered by USART
 *         so to proceed to decode and perform the command.
 * @param  
 * @retval none
 */
void SerialDevice_CheckAppCmd(void)
{
    /* Checks the UART2 is in idle state */
    if (huart2.gState == HAL_UART_STATE_READY)
    {

        /* Checks one character has been at least entered */
        if (strchr((char *)UsartTextString, '\r') != NULL)
        {
            /* Decode the entered command string */
            strtok((char *)UsartTextString, "\r");

            /* Handle commands */
            switch (UsartTextString[0U])
            {
          	/* case 'e': */
           	/* case 'x': */
           	/* run = false; */
           	/* 	break; */
            case 'N':
                GetNwkStatus();
                break;
            case 'O':
                SetCustomCFG();
                break;
            case 'C':
                GetCustomCFG();
                break;
            case 'A':
                ActivateABP();
                break;
            case 'R':
                Reactivate();
                break;
            case 'D':
                Deactivate();
                break;
            case 'T':
                ActivateOTA();
                break;
            case 'j':
                /* Join network */
                Join();
                break;
            case 'S':
                SetRadioStack();
                break;
            case 'K':
                GetRadioStack();
                break;
            case 'G':
                GetSupportBand();
                break;
            case 'o':
                /* Get opmode */
                GetOPMODE();
                break;
            case 'm':
                /* Set opmode */
                SetOPMODE();
                break;
            case 'n':
                /* Clear opmode */
                ClearOPMODE();
                break;
            case 'E':
                GetDevEUI();
                break;
            case 'e':
                /* Set dev eui */
                SetDevEUI();
                break;
            case '-':
                /* Get rtc */
                ClearRTCAlarm();
                break;
            case 't':
                /* Get rtc */
                GetRTC();
                break;
            case '*':
                /* Set RTC */
                SetRTC();
                break;
            case 'a':
                /* Get rtc alarm */
                GetAlarm();
                break;
            case '+':
                /* Set rtc alarm */
                SetAlarm();
                break;
            case 's':
                /* Device status */
                GetDeviceStatus();
                break;
            case 'r':
                /* Seset device */
                Reset();
                break;
            case 'f':
                /* Get firmware version */
                GetFirmwareVersion();
                break;
            case 'i':
                /* Get device info */
                GetDeviceInfo();
                break;
            case 'p':
                /* Ping device */
                Ping();
                break;
            case 'u':
                /* Send u-data */
                SendUData();
                break;
            case 'c':
                /* Send c-data */
                SendCData();
                break;
            case 'Z':
                /* Factory reset */
                FactoryReset();
                break;
            case 'Q':
                /* Get Link ADR */
                GetLinkADR();
                break;
            case 'W':
                /* Set Link ADR */
                SetLinkADR();
                break;
            case ' ':
                USART_ShowMenu();
                break;
            }

            HAL_UART_Abort(&huart2);
            memset(&UsartTextString, 0x00U, sizeof(UsartTextString));
        }

        /* Prepare to receive a text string via USART with UART_IT_RXNE */
        HAL_UART_Receive_IT(&huart2, UsartTextString, USARTTEXTSTRINGSIZE);
    }
}

/**
 * @brief  GetNwkStatus
 * @param  
 * @retval none
 */
static void GetNwkStatus(void)
{
    SerialDevice_Printf("GetNwkStatus\n\r");

    /* Send get network status message */
    WiMOD_LoRaWAN_GetNkwStatus();
}

/**
 * @brief  Set Custom CFG
 * @param  
 * @retval none
 */
static void SetCustomCFG(void)
{
    SerialDevice_Printf("SetCustomCFG\n\r");

    /* Send set custom config message (dBd) */
    WiMOD_LoRaWAN_SetCustomConfig(0U);
}

/**
 * @brief  Get Custom CFG
 * @param  
 * @retval none
 */
static void GetCustomCFG(void)
{
    SerialDevice_Printf("GetCustomCFG\n\r");

    /* Send get custom config message */
    WiMOD_LoRaWAN_GetCustomConfig();
}

/**
 * @brief  Activate ABP
 * @param  
 * @retval none
 */
static void ActivateABP(void)
{
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_ACTIVATE_DEVICE_REQ, ABP_ACTIVATE_PAYLOAD, 36U);

    SerialDevice_Printf("Activate ABP\n\r");
}

/**
 * @brief  Activate OTA 
 * @param  
 * @retval none
 */
static void ActivateOTA(void)
{
    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_JOIN_PARAM_REQ, OTA_ACTIVATE_PAYLOAD, 24U);

    SerialDevice_Printf("Activate OTA\n\r");
}


/**
 * @brief  Reactivate ABP
 * @param  
 * @retval none
 */
static void Reactivate(void)
{
    SerialDevice_Printf("Reactivate\n\r");

    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_REACTIVATE_DEVICE_REQ, NULL, 0U);
}

/**
 * @brief  Deactivate device 
 * @param  
 * @retval none
 */
static void Deactivate(void)
{
    SerialDevice_Printf("Deactivate\n\r");

    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_DEACTIVATE_DEVICE_REQ, NULL, 0U);
}

/**
 * @brief  Get Support Band 
 * @param  
 * @retval none
 */
static void GetSupportBand(void)
{
    SerialDevice_Printf("GetSupportBand\n\r");
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_SUPPORTED_BANDS_REQ, NULL, 0U);
}

/**
 * @brief  Get Radio Stack
 * @param  
 * @retval none
 */
static void GetRadioStack(void)
{
    SerialDevice_Printf("GetRadioStack\n\r");
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_RSTACK_CONFIG_REQ, NULL, 0U);
}

/**
 * @brief  Set Radio Stack
 * @param  
 * @retval none
 */
static void SetRadioStack(void)
{
    SerialDevice_Printf("SetRadioStack\n\r");

    uint8_t data[7U];

    data[0U] = 0x05U; /* Default Data Rate Index */
    data[1U] = 0x10U; /* Default TX Power Level (EIRP) */
    /* Bit 0: 0 = Adaptive Data Rate disabled 1 = Adaptive Data Rate enabled */
    /* Bit 1: 0 = Duty Cycle Control disabled 1 = Duty Cycle Control enabled (Customer Mode required) */
    /* Bit 2: 0 = Class A selected 1 = Class C selected */
    /* Bit 6: 0 = standard RF packet output format 1 = extended RF packet output format: Tx/Rx channel info  */
    /* attached Bit 7: 0 = Rx MAC Command Forwarding disabled 1 = Rx MAC Command Forwarding enabled */
    data[2U] = 0b11000111;
    data[3U] = 0x00U; /* Power Saving Mode 0x00 : off 0x01 : automatic */
    data[4U] = 0x07U; /* Number of Retransmissions */
    data[5U] = 0x12U; /* Band Index AS923TH */
    data[6U] = 0x0FU; /* Header MAC Cmd Capacity */

    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_RSTACK_CONFIG_REQ, data, 7U);
}

/**
 * @brief  get DEV EUI 64 bit
 * @param  
 * @retval none
 */
static void GetDevEUI(void)
{
    SerialDevice_Printf("GetDevEUI\n\r");
    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_DEVICE_EUI_REQ, NULL, 0U);
}

/**
 * @brief  Set DEV EUI 64 bit
 *         this parameter can only be written in "Customer mode"
 * @param  
 * @retval none
 */
static void SetDevEUI(void)
{
    uint8_t eui[] = { 0x70U, 0xB3U, 0xD5U, 0x8FU, 0xF1U, 0x01U, 0x78U, 0x7CU };

    WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_DEVICE_EUI_REQ, eui, 8U);
}

/**
 * @brief  Set OPMODE
 *	Index Description
 *	0 Standard Application Mode / Default Mode
 *	1 Reserved
 *	2 Reserved
 *	3 Customer Mode
 *
 * @param  
 * @retval none
 */
static void SetOPMODE(void)
{
    SerialDevice_Printf("Set OPMODE\n\r");
    WiMOD_LoRaWAN_SetOPMODE(3U);
}

/**
 * @brief  Clear OPMODE
 * @param  
 * @retval none
 */
static void ClearOPMODE(void)
{
    SerialDevice_Printf("Clear OPMODE\n\r");
    WiMOD_LoRaWAN_SetOPMODE(0U);
}

/**
 * @brief  Get OPMODE
 * @param  
 * @retval none
 */
static void GetOPMODE(void)
{
    SerialDevice_Printf("Get OPMODE\n\r");
    WiMOD_LoRaWAN_GetOPMODE();
}

/**
 * @brief  Clear RTC alarm
 * @param  
 * @retval none
 */
static void ClearRTCAlarm(void)
{
    SerialDevice_Printf("Clear RTC alarm\n\r");
    WiMOD_LoRaWAN_ClearRTCAlarm();
}

/**
 * @brief  Get RTC
 * @param  
 * @retval none
 */
static void GetRTC(void)
{
    SerialDevice_Printf("Get RTC\n\r");
    WiMOD_LoRaWAN_GetRTC();
}

/**
 * @brief  Set RTC
 * @param  
 * @retval none
 */
static void SetRTC(void)
{
    SerialDevice_Printf("Set RTC\n\r");
    uint32_t time = 0U;
    /* 2018-5-20 [12:30:30] */
    /* Year */
    time |= 18U;
    time <<= 6U;
    /* Days */
    time |= 20U;
    time <<= 5U;
    /* Hours */
    time |= 12U;
    time <<= 5U;
    /* Month */
    time |= 5U;
    time <<= 4U;
    /* Minutes */
    time |= 30U;
    time <<= 6U;
    /* Second */
    time |= 30U;

    WiMOD_LoRaWAN_SetRTC(&time);
}

/**
 * @brief  Get RTC Alarm device
 * @param  
 * @retval none
 */
static void GetAlarm(void)
{
    SerialDevice_Printf("Get RTC alarm\n\r");
    WiMOD_LoRaWAN_GetRTCAlarm();
}

/**
 * @brief  Set RTC Alarm device
 * @param  
 * @retval none
 */
static void SetAlarm(void)
{
    SerialDevice_Printf("Set RTC alarm\n\r");
    uint8_t payload[4U] = { 0U };

    /* 0x00 : single alarm */
    /* 0x01 : daily repeated alarm */
    payload[0U] = 1U;
    payload[1U] = 12U;   /* Hour */
    payload[2U] = 30U;   /* Minute */
    payload[3U] = 30U;   /* Second */
    WiMOD_LoRaWAN_SetRTCAlarm(payload);
}

/**
 * @brief  Ping device
 * @param  
 * @retval none
 */
static void Ping(void)
{
    SerialDevice_Printf("Ping request\n\r");
    WiMOD_LoRaWAN_SendPing();
}

/**
 * @brief  Get firmware version
 * @param  
 * @retval none
 */
static void GetFirmwareVersion(void)
{
    SerialDevice_Printf("Get firmware version\n\r");
    WiMOD_LoRaWAN_GetFirmwareVersion();
}

/**
 * @brief  Get device information
 * @param  
 * @retval none
 */
static void GetDeviceInfo(void)
{
    SerialDevice_Printf("Get device information\n\r");
    WiMOD_LoRaWAN_GetDeviceInfo();
}

/**
 * @brief  Get status device
 * @param  
 * @retval none
 */
static void GetDeviceStatus(void)
{
    SerialDevice_Printf("Device status\n\r");
    WiMOD_LoRaWAN_GetDeviceStatus();
}

/**
 * @brief  Reset device
 * @param  
 * @retval none
 */
static void Reset(void)
{
    SerialDevice_Printf("Reset device\n\r");
    WiMOD_LoRaWAN_Reset();
}

/**
 * @brief  Join device
 * @param  
 * @retval none
 */
static void Join(void)
{
    SerialDevice_Printf("Join network request\n\r");
    WiMOD_LoRaWAN_JoinNetworkRequest();
}

/**
 * @brief  Send unconfirmed radio message
 * @param  
 * @retval none
 */
static void SendUData(void)
{
    SerialDevice_Printf("Send U-Data\n\r");

    /* Port 0x21 (33) */
    uint8_t port = 0x21U;

    uint8_t data[6U];

    data[0U] = 0x06U;
    data[1U] = 0x71U;
    data[2U] = 0x04U;
    data[3U] = 0xD2U;
    data[4U] = 0xFBU;
    data[5U] = 0x2EU;

    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_SendURadioData(port, data, 6U);
}

/**
 * @brief  Send confirmed radio message
 * @param  
 * @retval none
 */
static void SendCData(void)
{
    SerialDevice_Printf("Send C-Data\n\r");

    /* Port 0x23 (35) */
    uint8_t port = 0x23U;

    uint8_t data[6U];

    data[0U] = 0x67U;
    data[1U] = 0x01U;
    data[2U] = 0x10U;
    data[3U] = 0x67U;
    data[4U] = 0x00U;
    data[5U] = 0xFFU;

    /* Send unconfirmed radio message */
    WiMOD_LoRaWAN_SendCRadioData(port, data, 6U);
}

/**
 * @brief  Factory reset
 * @param  
 * @retval none
 */
static void FactoryReset(void)
{
    SerialDevice_Printf("FactoryReset\n\r");
    WiMOD_LoRaWAN_FactoryReset();
}

/**
 * @brief  Get link ADR
 * @param  
 * @retval none
 */
static void GetLinkADR(void)
{
    SerialDevice_Printf("GetLinkADR\n\r");
    WiMOD_LoRaWAN_GetLinkADR();
}

/**
 * @brief  Set link ADR
 * @param  
 * @retval none
 */
static void SetLinkADR(void)
{
    uint8_t payload = 0U;

    SerialDevice_Printf("SetLinkADR\n\r");
    /* 
     * LinkADRReq Option:
     * 0: LoRaWAN v1.0.2 
     * 1: Semtech proposal 
     * 2: KPN/Actility proposal 
     */
    WiMOD_LoRaWAN_SetLinkADR(&payload);
}

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
