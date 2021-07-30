//------------------------------------------------------------------------------
//
//	File:		WiMOD_LoRaWAN_API.cpp
//
//	Abstract:	API Layer of LoRaWAN Host Controller Interface
//
//	Version:	0.1
//
//	Date:		18.05.2016
//
//	Disclaimer:	This example code is provided by IMST GmbH on an "AS
// IS"
// basis 				without any warranties.
//
//	Maintain by : Anol Paisal <anol.paisal@emone.co.th> @ 2018
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Include Files
//
//------------------------------------------------------------------------------
#include "wimod_lorawan_api.h"
#include <string.h>

// HCI Message Receiver callback
static TWiMOD_HCI_Message *
WiMOD_LoRaWAN_Process_RxMessage (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_DevMgmt_Message (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp (TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_LoRaWAN_Message (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Reactivate_Rsp (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_U_DATA_TxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_C_DATA_TxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_JoinTxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_JoinNetworkIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_U_DataRxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_C_DataRxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_MAC_CMDRxIndication (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Set_RSTACK_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_RSTACK_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_Support_Band_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_Network_Status_RSP (TWiMOD_HCI_Message *rxMessage);

static void
WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP (TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_ShowResponse (const char *string,
        const TIDString *statusTable,
        UINT8 statusID);

//------------------------------------------------------------------------------
//
//  Section RAM
//
//------------------------------------------------------------------------------

// reserve one Tx-Message
TWiMOD_HCI_Message TxMessage;

// reserve one Rx-Message
TWiMOD_HCI_Message RxMessage;

//------------------------------------------------------------------------------
//
//  Section Const
//
//------------------------------------------------------------------------------

// Status Codes for DeviceMgmt Response Messages
static const TIDString WiMOD_DeviceMgmt_StatusStrings[]
= { { DEVMGMT_STATUS_OK, "ok" },
    { DEVMGMT_STATUS_ERROR, "error" },
    { DEVMGMT_STATUS_CMD_NOT_SUPPORTED, "command not supported" },
    { DEVMGMT_STATUS_WRONG_PARAMETER, "wrong parameter" },
    { DEVMGMT_STATUS_WRONG_DEVICE_MODE, "wrong device mode" },

    // end of table
    { 0, 0 } };

// Status Codes for LoRaWAN Response Messages
static const TIDString WiMOD_LoRaWAN_StatusStrings[] = {
    { LORAWAN_STATUS_OK, "ok" },
    { LORAWAN_STATUS_ERROR, "error" },
    { LORAWAN_STATUS_CMD_NOT_SUPPORTED, "command not supported" },
    { LORAWAN_STATUS_WRONG_PARAMETER, "wrong parameter" },
    { LORAWAN_STATUS_WRONG_DEVICE_MODE, "wrong device mode" },
    { LORAWAN_STATUS_DEVICE_NOT_ACTIVATED, "device not activated" },
    { LORAWAN_STATUS_DEVICE_BUSY, "device busy - command rejected" },
    { LORAWAN_STATUS_QUEUE_FULL, "message queue full - command rejected" },
    { LORAWAN_STATUS_LENGTH_ERROR, "HCI message length error" },
    { LORAWAN_STATUS_NO_FACTORY_SETTINGS, "no factory settings available" },
    { LORAWAN_STATUS_CHANNEL_BLOCKED_BY_DC,
        "error: channel blocked due to duty cycle, please try later again" },
    { LORAWAN_STATUS_CHANNEL_NOT_AVAILABLE, "error: channel not available" },

    // end of table
    { 0, 0 }
};

// Status Codes for DeviceMgmt Response Messages
static const TIDString WiMOD_DeviceMgmt_ModuleTypes[]
= { { 0x90, "iM880A (obsolete)" },
    { 0x92, "iM880A-L (128k)" },
    { 0x93, "iU880A (128k)" },
    { 0x98, "iM880B-L" },
    { 0x99, "iU880B" },
    { 0x9A, "iM980A (iMAS923TH for Thailand)" },
    { 0xA0, "iM881A" },
    // end of table
    { 0, 0 } };

//------------------------------------------------------------------------------
//
//  Section Code
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
//  Init
//
//  @brief: init complete interface
//
//------------------------------------------------------------------------------

    bool
WiMOD_LoRaWAN_Init (UART_HandleTypeDef *comPort)
{
    // init HCI layer
    return WiMOD_HCI_Init (comPort,                         // comPort
            WiMOD_LoRaWAN_Process_RxMessage, // receiver callback
            &RxMessage);                     // rx message
}

    int
WiMOD_DevMgmt_Msg_Req (uint8_t msg_id, uint8_t *val, uint16_t len)
{
    TxMessage.SapID = DEVMGMT_SAP_ID;
    TxMessage.MsgID = msg_id;

    memset (TxMessage.Payload, 0x00, WIMOD_HCI_MSG_PAYLOAD_SIZE);

    switch (msg_id)
    {
        case DEVMGMT_MSG_PING_REQ:
        case DEVMGMT_MSG_GET_DEVICE_INFO_REQ:
        case DEVMGMT_MSG_RESET_REQ:
        case DEVMGMT_MSG_GET_FW_VERSION_REQ:
        case DEVMGMT_MSG_GET_DEVICE_STATUS_REQ:
        case DEVMGMT_MSG_GET_RTC_REQ:
        case DEVMGMT_MSG_GET_RTC_ALARM_REQ:
        case DEVMGMT_MSG_CLEAR_RTC_ALARM_REQ:
        case DEVMGMT_MSG_GET_OPMODE_REQ:
            TxMessage.Length = 0;
            break;
        case DEVMGMT_MSG_SET_RTC_REQ:
        case DEVMGMT_MSG_SET_RTC_ALARM_REQ:
            TxMessage.Length = 4;
            break;
        case DEVMGMT_MSG_SET_OPMODE_REQ:
            TxMessage.Length = 1;
            break;
        default:
            return DEVMGMT_STATUS_CMD_NOT_SUPPORTED;
    }

    if (TxMessage.Length > 0)
    {
        memcpy (TxMessage.Payload, val, TxMessage.Length);
    }

    return (WiMOD_HCI_SendMessage (&TxMessage));
}

    int
WiMOD_LoRaWAN_Msg_Req (uint8_t msg_id, uint8_t *val, uint16_t len)
{
    TxMessage.SapID = LORAWAN_SAP_ID;
    TxMessage.MsgID = msg_id;

    memset (TxMessage.Payload, 0x00, WIMOD_HCI_MSG_PAYLOAD_SIZE);

    switch (msg_id)
    {
        case LORAWAN_MSG_ACTIVATE_DEVICE_REQ:
            TxMessage.Length = 36U;
            break;
        case LORAWAN_MSG_REACTIVATE_DEVICE_REQ:
        case LORAWAN_MSG_JOIN_NETWORK_REQ:
        case LORAWAN_MSG_GET_RSTACK_CONFIG_REQ:
        case LORAWAN_MSG_GET_SUPPORTED_BANDS_REQ:
        case LORAWAN_MSG_GET_DEVICE_EUI_REQ:
        case LORAWAN_MSG_GET_CUSTOM_CFG_REQ:
        case LORAWAN_MSG_GET_LINKADRREQ_CONFIG_REQ:
        case LORAWAN_MSG_FACTORY_RESET_REQ:
        case LORAWAN_MSG_DEACTIVATE_DEVICE_REQ:
        case LORAWAN_MSG_GET_NWK_STATUS_REQ:
            TxMessage.Length = 0U;
            break;
        case LORAWAN_MSG_SET_CUSTOM_CFG_REQ:
        case LORAWAN_MSG_SET_LINKADRREQ_CONFIG_REQ:
            TxMessage.Length = 1U;
            break;
        case LORAWAN_MSG_SET_JOIN_PARAM_REQ:
            TxMessage.Length = 24U;
            break;
        case LORAWAN_MSG_SEND_UDATA_REQ:
        case LORAWAN_MSG_SEND_CDATA_REQ:
        case LORAWAN_MSG_SEND_MAC_CMD_REQ:
            TxMessage.Length = len;
            break;
        case LORAWAN_MSG_SET_RSTACK_CONFIG_REQ:
            TxMessage.Length = 7U;
            break;
        case LORAWAN_MSG_SET_DEVICE_EUI_REQ:
            TxMessage.Length = 8U;
            break;
        default:
            return DEVMGMT_STATUS_CMD_NOT_SUPPORTED;
    }

    if (TxMessage.Length > 0U)
    {
        memcpy (TxMessage.Payload, val, TxMessage.Length);
    }

    return (WiMOD_HCI_SendMessage (&TxMessage));
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_GetCustomConfig
//
//  @brief: Get custom config
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetCustomConfig (void)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_GET_CUSTOM_CFG_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_SetCustomConfig
//
//  @brief: set custom config
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SetCustomConfig (int8_t val)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_SET_CUSTOM_CFG_REQ,
            (uint8_t *)&val, 1);
}

//------------------------------------------------------------------------------
//
//  SetOPMODE
//
//  @brief: set OPMODE
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SetOPMODE (uint8_t val)
{
    if (val > 1)
    {
        return -1;
    }
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_SET_OPMODE_REQ, &val, 1);
}

//------------------------------------------------------------------------------
//
//  GetOPMODE
//
//  @brief: get OPMODE
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetOPMODE ()
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_OPMODE_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  ClearRTCAlarm
//
//  @brief: clear RTC Alarm
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_ClearRTCAlarm ()
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_CLEAR_RTC_ALARM_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  GetRTCAlarm
//
//  @brief: get RTC Alarm
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetRTCAlarm ()
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_RTC_ALARM_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  SetRTC
//
//  @brief: set RTC
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SetRTCAlarm (uint8_t *alarm)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_SET_RTC_ALARM_REQ, alarm, 4);
}

//------------------------------------------------------------------------------
//
//  GetRTC
//
//  @brief: get RTC
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetRTC ()
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_RTC_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  SetRTC
//
//  @brief: set RTC
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SetRTC (uint32_t *time)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_SET_RTC_REQ, (uint8_t *)time, 4);
}

//------------------------------------------------------------------------------
//
//  SendPing
//
//  @brief: ping device
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SendPing ()
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_PING_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  GetDeviceInfo
//
//  @brief: get device information
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetDeviceInfo (void)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_DEVICE_INFO_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  GetFirmwareVersion
//
//  @brief: get firmware version
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetFirmwareVersion (void)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_FW_VERSION_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_GetNkwStatus
//
//  @brief: get network status
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetNkwStatus (void)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_GET_NWK_STATUS_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_FactoryReset
//
//  @brief: factory reset
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_FactoryReset (void)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_FACTORY_RESET_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_GetLinkADR
//
//  @brief: GetLinkADR
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetLinkADR (void)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_GET_LINKADRREQ_CONFIG_REQ, NULL,
            0);
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_SetLinkADR
//
//  @brief: SetLinkADR
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_SetLinkADR (uint8_t *payload)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_SET_LINKADRREQ_CONFIG_REQ, payload,
            1);
}

// set mac cmd
    int
WiMOD_LoRaWAN_SetMAC_CMD (uint8_t *payload)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_SEND_MAC_CMD_REQ, payload, 16);
}

//------------------------------------------------------------------------------
//
//  Reset
//
//  @brief: reset device
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_Reset (void)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_RESET_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  GetDeviceStatus
//
//  @brief: get device status
//
//------------------------------------------------------------------------------
    int
WiMOD_LoRaWAN_GetDeviceStatus (void)
{
    return WiMOD_DevMgmt_Msg_Req (DEVMGMT_MSG_GET_DEVICE_STATUS_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  JoinNetworkRequest
//
//  @brief: send join radio message
//
//------------------------------------------------------------------------------

    int
WiMOD_LoRaWAN_JoinNetworkRequest (void)
{
    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_JOIN_NETWORK_REQ, NULL, 0);
}

//------------------------------------------------------------------------------
//
//  SendURadioData
//
//  @brief: send unconfirmed radio message
//
//------------------------------------------------------------------------------

    int
WiMOD_LoRaWAN_SendURadioData (UINT8 port,     // LoRaWAN Port
        UINT8 *srcData, // application payload
        int srcLength)  // length of application payload
{
    uint8_t payload[WIMOD_HCI_MSG_PAYLOAD_SIZE] = { 0 };

    // 1. check length
    if (srcLength > (WIMOD_HCI_MSG_PAYLOAD_SIZE - 1))
    {
        // overflow error
        return -1;
    }

    // 3.  init payload
    // 3.1 init port
    payload[0] = port;

    // 3.2 init radio message payload
    memcpy (&payload[1], srcData, srcLength);

    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_SEND_UDATA_REQ, payload,
            1 + srcLength);
}

//------------------------------------------------------------------------------
//
//  SendCRadioData
//
//  @brief: send confirmed radio message
//
//------------------------------------------------------------------------------

    int
WiMOD_LoRaWAN_SendCRadioData (UINT8 port,     // LoRaWAN Port
        UINT8 *srcData, // application data
        int srcLength)  // length of application data
{
    uint8_t payload[WIMOD_HCI_MSG_PAYLOAD_SIZE] = { 0 };

    // 1. check length
    if (srcLength > (WIMOD_HCI_MSG_PAYLOAD_SIZE - 1))
    {
        // overflow error
        return -1;
    }

    // 3.  init payload
    // 3.1 init port
    payload[0] = port;

    // 3.2 init radio message payload
    memcpy (&payload[1], srcData, srcLength);

    return WiMOD_LoRaWAN_Msg_Req (LORAWAN_MSG_SEND_CDATA_REQ, payload,
            1 + srcLength);
}

//------------------------------------------------------------------------------
//
//  Process
//
//  @brief: handle receiver process
//
//------------------------------------------------------------------------------

    void
WiMOD_LoRaWAN_Process (void)
{
    // call HCI process
    WiMOD_HCI_Process ();
}

//------------------------------------------------------------------------------
//
//  Process
//
//  @brief: handle receiver process
//
//------------------------------------------------------------------------------

    static TWiMOD_HCI_Message *
WiMOD_LoRaWAN_Process_RxMessage (TWiMOD_HCI_Message *rxMessage)
{
    switch (rxMessage->SapID)
    {
        case DEVMGMT_SAP_ID:
            WiMOD_LoRaWAN_Process_DevMgmt_Message (rxMessage);
            break;

        case LORAWAN_SAP_ID:
            WiMOD_LoRaWAN_Process_LoRaWAN_Message (rxMessage);
            break;
    }

    return &RxMessage;
}

//------------------------------------------------------------------------------
//
//  Process_DevMgmt_Message
//
//  @brief: handle received Device Management SAP messages
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_Process_DevMgmt_Message (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    switch (rxMessage->MsgID)
    {
        case DEVMGMT_MSG_PING_RSP:
            WiMOD_LoRaWAN_ShowResponse ("ping response",
                    WiMOD_DeviceMgmt_StatusStrings,
                    rxMessage->Payload[0]);
            break;
        case DEVMGMT_MSG_GET_DEVICE_INFO_RSP:
            WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp (rxMessage);
            break;
        case DEVMGMT_MSG_GET_FW_VERSION_RSP:
            WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp (rxMessage);
            break;
        case DEVMGMT_MSG_RESET_RSP:
            WiMOD_LoRaWAN_ShowResponse ("reset response",
                    WiMOD_DeviceMgmt_StatusStrings,
                    rxMessage->Payload[0]);
            break;
        case DEVMGMT_MSG_GET_DEVICE_STATUS_RSP:
            WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp (rxMessage);
            break;
        case DEVMGMT_MSG_GET_RTC_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp (rxMessage);
            break;
        case DEVMGMT_MSG_SET_RTC_RSP:
            WiMOD_LoRaWAN_ShowResponse ("set RTC response",
                    WiMOD_DeviceMgmt_StatusStrings,
                    rxMessage->Payload[0]);
            break;
        case DEVMGMT_MSG_GET_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp (rxMessage);
            break;
        case DEVMGMT_MSG_SET_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_ShowResponse ("set RTC Alarm response",
                    WiMOD_DeviceMgmt_StatusStrings,
                    rxMessage->Payload[0]);
            break;
        case DEVMGMT_MSG_CLEAR_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_ShowResponse ("clear RTC Alarm response",
                    WiMOD_DeviceMgmt_StatusStrings,
                    rxMessage->Payload[0]);
            break;
        case DEVMGMT_MSG_GET_OPMODE_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp (rxMessage);
            break;

        default:
            sprintf (buf,
                    "Unhandled DeviceMgmt message received - MsgID "
                    ": 0x%02X\n\r",
                    (UINT8)rxMessage->MsgID);
            USART_Transmit (&huart2, buf);
            break;
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp
//
//  @brief: Get OPMODE
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse ("get OPMODE response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {
        USART_Transmit (&huart2, "OPMODE Set: 0x");
        USART_Transmit (&huart2, (const char *)num2hex (
                    (uint32_t)rxMessage->Payload[1], BYTE_F));
        USART_Transmit (&huart2, "\n\r");
    }
}
//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp
//
//  @brief: Get RTC alarm
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };
    uint32_t help;
    WiMOD_LoRaWAN_ShowResponse ("get RTC ALARM response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {
        USART_Transmit (&huart2, "RTC ALARM Set: 0x");
        USART_Transmit (&huart2, (const char *)num2hex (
                    (uint32_t)rxMessage->Payload[1], BYTE_F));
        USART_Transmit (&huart2, "\n\r");

        USART_Transmit (&huart2, "RTC ALARM Daily: 0x");
        USART_Transmit (&huart2, (const char *)num2hex (
                    (uint32_t)rxMessage->Payload[2], BYTE_F));
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[3], 3);
        USART_Transmit (&huart2, "RTC ALARM Time: [");
        num2str ((help & 0xFF), str); // Hour
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str ((help & 0xFF00) >> 8, str); // Minute
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str ((help & 0xFF0000) >> 16, str); // second
        USART_Transmit (&huart2, (const char *)str);

        USART_Transmit (&huart2, "]\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp
//
//  @brief: Get RTC
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };
    uint32_t help;
    WiMOD_LoRaWAN_ShowResponse ("get RTC response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {
        memcpy ((uint8_t *)&help, &rxMessage->Payload[1], 4);
        USART_Transmit (&huart2, "RTC Time: ");
        num2str (((help & 0xfc000000) >> 26) + 2000, str); // Year
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "-");
        num2str (((help & 0xf000) >> 12), str); // Months
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "-");
        num2str (((help & 0x3e00000) >> 21), str); // Day
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, " [");
        num2str (((help & 0x1f0000) >> 16), str); // Hour
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str (((help & 0xfc0) >> 6), str); // Minutes
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str (((help & 0x3f)), str); // Seconds
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "]\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp
//
//  @brief: show device status
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };
    uint32_t help;
    WiMOD_LoRaWAN_ShowResponse ("device status response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {
        memcpy ((uint8_t *)&help, &rxMessage->Payload[1], 1);
        num2str (help & 0xFF, str);
        USART_Transmit (&huart2, "System Tick Resolution: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, " ms\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[2], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "System Tick: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[6], 4);
        USART_Transmit (&huart2, "Target Time: ");
        num2str (((help & 0xfc000000) >> 26) + 2000, str); // Year
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "-");
        num2str (((help & 0xf000) >> 12), str); // Months
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "-");
        num2str (((help & 0x3e00000) >> 21), str); // Day
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, " [");
        num2str (((help & 0x1f0000) >> 16), str); // Hour
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str (((help & 0xfc0) >> 6), str); // Minutes
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ":");
        num2str (((help & 0x3f)), str); // Seconds
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "]\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[10], 2);
        num2str (help & 0xFFFF, str);
        USART_Transmit (&huart2, "NVM Status: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[12], 2);
        num2str (help & 0xFFFF, str);
        USART_Transmit (&huart2, "Battery level: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, " mV\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[16], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "TX U-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[20], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "TX C-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[24], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "TX Error: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[28], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX1 U-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[32], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX1 C-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[36], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX1 MIC-Error: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[40], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX2 U-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[44], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX2 C-Data: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[48], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX2 MIC-Error: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[52], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "TX Join: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[56], 4);
        num2str (help, str);
        USART_Transmit (&huart2, "RX Accept: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp
//
//  @brief: show device information
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };
    uint32_t help;
    WiMOD_LoRaWAN_ShowResponse ("device information response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {
        WiMOD_LoRaWAN_ShowResponse ("Module type", WiMOD_DeviceMgmt_ModuleTypes,
                rxMessage->Payload[1]);

        memcpy ((uint8_t *)&help, &rxMessage->Payload[2], 4);

        USART_Transmit (&huart2, "Device address: 0x");
        USART_Transmit (&huart2,
                (const char *)num2hex ((uint32_t)help, DOUBLEWORD_F));
        USART_Transmit (&huart2, "\n\r");

        memcpy ((uint8_t *)&help, &rxMessage->Payload[6], 4);
        USART_Transmit (&huart2, "Device ID: 0x");
        USART_Transmit (&huart2,
                (const char *)num2hex ((uint32_t)help, DOUBLEWORD_F));
        USART_Transmit (&huart2, "(");
        num2str (help, str);
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ")\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp
//
//  @brief: show firmware version
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    char help[80];
    char buf[350];

    WiMOD_LoRaWAN_ShowResponse ("firmware version response",
            WiMOD_DeviceMgmt_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == DEVMGMT_STATUS_OK)
    {

        sprintf (buf, "version: V%d.%d\n\r", (int)rxMessage->Payload[2],
                (int)rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "build-count: %d\n\r",
                (int)MAKEWORD (rxMessage->Payload[3], rxMessage->Payload[4]));
        USART_Transmit (&huart2, buf);

        memcpy (help, &rxMessage->Payload[5], 10);
        help[10] = 0;
        sprintf (buf, "build-date: %s\n\r", help);
        USART_Transmit (&huart2, buf);

        // more information attached ?
        if (rxMessage->Length > 15)
        {
            // add string termination
            rxMessage->Payload[rxMessage->Length] = 0;
            sprintf (buf, "firmware-content: %s\n\r", &rxMessage->Payload[15]);
            USART_Transmit (&huart2, buf);
        }
    }
}

//------------------------------------------------------------------------------
//
//  Process_LoRaWAN_Message
//
//  @brief: handle received LoRaWAN SAP messages
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_Process_LoRaWAN_Message (TWiMOD_HCI_Message *rxMessage)
{
    switch (rxMessage->MsgID)
    {
        case LORAWAN_MSG_ACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_ShowResponse ("activate device response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_REACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_Process_Reactivate_Rsp (rxMessage);
            break;

        case LORAWAN_MSG_DEACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_ShowResponse ("deactivate device response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_JOIN_NETWORK_RSP:
            WiMOD_LoRaWAN_ShowResponse ("join network response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_SEND_UDATA_RSP:
            WiMOD_LoRaWAN_ShowResponse ("send U-Data response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_SEND_UDATA_IND:
            WiMOD_LoRaWAN_Process_U_DATA_TxIndication (rxMessage);
            break;

        case LORAWAN_MSG_SEND_CDATA_RSP:
            WiMOD_LoRaWAN_ShowResponse ("send C-Data response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_SEND_CDATA_IND:
            WiMOD_LoRaWAN_Process_C_DATA_TxIndication (rxMessage);
            break;

        case LORAWAN_MSG_JOIN_TRANSMIT_IND:
            WiMOD_LoRaWAN_Process_JoinTxIndication (rxMessage);
            break;

        case LORAWAN_MSG_JOIN_NETWORK_IND:
            WiMOD_LoRaWAN_Process_JoinNetworkIndication (rxMessage);
            break;

        case LORAWAN_MSG_RECV_UDATA_IND:
            WiMOD_LoRaWAN_Process_U_DataRxIndication (rxMessage);
            break;

        case LORAWAN_MSG_RECV_CDATA_IND:
            WiMOD_LoRaWAN_Process_C_DataRxIndication (rxMessage);
            break;

        case LORAWAN_MSG_SEND_MAC_CMD_RSP:
            WiMOD_LoRaWAN_ShowResponse ("send MAC CMD response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);

            break;
        case LORAWAN_MSG_RECV_MAC_CMD_IND:
            WiMOD_LoRaWAN_Process_MAC_CMDRxIndication (rxMessage);
            break;

        case LORAWAN_MSG_RECV_NODATA_IND:
            USART_Transmit (&huart2, "no data received indication\n\r");
            break;

        case LORAWAN_MSG_SET_RSTACK_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Set_RSTACK_RSP (rxMessage);
            break;

        case LORAWAN_MSG_GET_RSTACK_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Get_RSTACK_RSP (rxMessage);
            break;

        case LORAWAN_MSG_GET_SUPPORTED_BANDS_RSP:
            WiMOD_LoRaWAN_Process_Get_Support_Band_RSP (rxMessage);
            break;

        case LORAWAN_MSG_GET_DEVICE_EUI_RSP:
            WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP (rxMessage);
            break;

        case LORAWAN_MSG_GET_CUSTOM_CFG_RSP:
            WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP (rxMessage);
            break;

        case LORAWAN_MSG_SET_CUSTOM_CFG_RSP:
            WiMOD_LoRaWAN_ShowResponse ("set custom cfg response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_FACTORY_RESET_RSP:
            WiMOD_LoRaWAN_ShowResponse ("factory reset response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        case LORAWAN_MSG_GET_NWK_STATUS_RSP:
            WiMOD_LoRaWAN_Process_Get_Network_Status_RSP (rxMessage);
            break;

        case LORAWAN_MSG_GET_LINKADRREQ_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP (rxMessage);
            break;

        case LORAWAN_MSG_SET_LINKADRREQ_CONFIG_RSP:
            WiMOD_LoRaWAN_ShowResponse ("set link adr response",
                    WiMOD_LoRaWAN_StatusStrings,
                    rxMessage->Payload[0]);
            break;

        default:
            USART_Transmit (&huart2, "Unhandled LoRaWAN SAP message "
                    "received - MsgID : 0x");
            USART_Transmit (&huart2,
                    (const char *)num2hex (rxMessage->MsgID, BYTE_F));
            USART_Transmit (&huart2, "\n\r");
            break;
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Reactivate_Rsp
//
//  @brief: reactivate device
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_Process_Reactivate_Rsp (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };
    uint32_t help = 0;
    WiMOD_LoRaWAN_ShowResponse ("reactivate device response",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {
        memcpy ((uint8_t *)&help, &rxMessage->Payload[1], 4);
        num2str (help, str); // Second
        USART_Transmit (&huart2, "Device address: 0x");
        USART_Transmit (&huart2,
                (const char *)num2hex ((uint32_t)help, DOUBLEWORD_F));
        USART_Transmit (&huart2, "(");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, ")\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_C_DATA_TxIndication
//
//  @brief: unconfirm data transmit indicaton
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_C_DATA_TxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];

    if (rxMessage->Payload[0] == 0)
    {
        sprintf (buf, "c data tx event : radio packet sent\n\r");
        USART_Transmit (&huart2, buf);
    }
    // channel info attached ?
    else if (rxMessage->Payload[0] == 1)
    {
        sprintf (buf, "c data tx event : radio packet sent with "
                "tx channel info "
                "attached\n\r");
        USART_Transmit (&huart2, buf);
        sprintf (buf, "c data tx event ChIdx %d\n\r",
                (int)rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tDR: %ld\n\r", (int32_t)rxMessage->Payload[2]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tNum: %ld\n\r", (int32_t)rxMessage->Payload[3]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tPower(dBm): %ld\n\r", (int32_t)rxMessage->Payload[4]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tAirtime (ms): %ln\n\r",
                (uint32_t *)&rxMessage->Payload[5]);
        USART_Transmit (&huart2, buf);
    }
    else if (rxMessage->Payload[0] & 0x02)
    {
        sprintf (buf, "error, maximum number of retransmissions "
                "reached\n\r");
        USART_Transmit (&huart2, buf);
    }
    else if (rxMessage->Payload[0] & 0x04)
    {
        sprintf (buf, "maximum payload size exceeded for current "
                "data rate\n\r");
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "c data tx event - Status : error\n\r");
        USART_Transmit (&huart2, buf);
    }
}
//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_U_DATA_TxIndication
//
//  @brief: unconfirm data transmit indicaton
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_U_DATA_TxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    if (rxMessage->Payload[0] == 0)
    {
        sprintf (buf, "u data tx event : radio packet sent\n\r");
        USART_Transmit (&huart2, buf);
    }
    // channel info attached ?
    else if (rxMessage->Payload[0] == 1)
    {
        sprintf (buf, "u data tx event : radio packet sent with "
                "tx channel info "
                "attached\n\r");
        USART_Transmit (&huart2, buf);
        sprintf (buf, "u data tx event ChIdx %ld\n\r",
                (int32_t)rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tDR: %ld\n\r", (int32_t)rxMessage->Payload[2]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tNum: %ld\n\r", (int32_t)rxMessage->Payload[3]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tPower(dBm): %ld\n\r", (int32_t)rxMessage->Payload[4]);
        USART_Transmit (&huart2, buf);
        sprintf (buf, "\tAirtime (ms): %ln\n\r",
                (uint32_t *)&rxMessage->Payload[5]);
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "u data tx event - Status : error\n\r");
        USART_Transmit (&huart2, buf);
    }
}
//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_JoinTxIndication
//
//  @brief: show join transmit indicaton
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_Process_JoinTxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];

    if (rxMessage->Payload[0] == 0)
    {
        sprintf (buf, "join tx event - Status : ok\n\r");
        USART_Transmit (&huart2, buf);
    }
    // channel info attached ?
    else if (rxMessage->Payload[0] == 1)
    {
        sprintf (buf,
                "join tx event:%d, ChnIdx:%d, DR:%d - Status : "
                "ok\n\r",
                (int)rxMessage->Payload[3], (int)rxMessage->Payload[1],
                (int)rxMessage->Payload[2]);
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "join tx event - Status : error\n\r");
        USART_Transmit (&huart2, buf);
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_JoinNetworkIndication
//
//  @brief: show join network indicaton
//
//------------------------------------------------------------------------------

    void
WiMOD_LoRaWAN_Process_JoinNetworkIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];

    if (rxMessage->Payload[0] == 0)
    {
        UINT32 address
            = MAKELONG (MAKEWORD (rxMessage->Payload[1], rxMessage->Payload[2]),
                    MAKEWORD (rxMessage->Payload[3], rxMessage->Payload[4]));

        sprintf (buf,
                "join network accept event - "
                "DeviceAddress:0x%08lX\n\r",
                address);
        USART_Transmit (&huart2, buf);
    }
    else if (rxMessage->Payload[0] == 1)
    {
        UINT32 address
            = MAKELONG (MAKEWORD (rxMessage->Payload[1], rxMessage->Payload[2]),
                    MAKEWORD (rxMessage->Payload[3], rxMessage->Payload[4]));

        sprintf (buf,
                "join network accept event - "
                "DeviceAddress:0x%08lX, ChnIdx:%d, "
                "DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                address, (int)rxMessage->Payload[5], (int)rxMessage->Payload[6],
                (int)rxMessage->Payload[7], (int)rxMessage->Payload[8],
                (int)rxMessage->Payload[9]);
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "join network timeout event\n\r");
        USART_Transmit (&huart2, buf);
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_U_DataRxIndication
//
//  @brief: show received U-Data
//
//------------------------------------------------------------------------------

    void
WiMOD_LoRaWAN_Process_U_DataRxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    int payloadSize = rxMessage->Length - 1;

    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
        payloadSize -= 5;

    if (payloadSize >= 1)
    {
        USART_Transmit (&huart2, "U-Data rx event: port:0x");
        USART_Transmit (&huart2,
                (const char *)num2hex (rxMessage->Payload[1], BYTE_F));
        USART_Transmit (&huart2, "\n\rapp-payload: 0x");
        for (int i = 1; i < payloadSize; i++)
            USART_Transmit (&huart2, (const char *)num2hex (
                        rxMessage->Payload[1 + i], BYTE_F));
        USART_Transmit (&huart2, "\n\r");
    }

    if (rxMessage->Payload[0] & 0x02)
    {
        sprintf (buf, "ack for uplink packet:yes\n\r");
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "ack for uplink packet:no\n\r");
        USART_Transmit (&huart2, buf);
    }

    if (rxMessage->Payload[0] & 0x04)
    {
        sprintf (buf, "frame pending:yes\n\r");
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "frame pending:no\n\r");
        USART_Transmit (&huart2, buf);
    }

    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
    {
        UINT8 *rxInfo = &rxMessage->Payload[1 + payloadSize];
        sprintf (buf, "ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                (int)rxInfo[0], (int)rxInfo[1], (int)rxInfo[2], (int)rxInfo[3],
                (int)rxInfo[4]);
        USART_Transmit (&huart2, buf);
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP
//
//  @brief: Get LINK ADR ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP (TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse (" Get Network Status ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {
        USART_Transmit (&huart2, "LinkADRReq Option: ");
        switch (rxMessage->Payload[1])
        {
            case 0:
                USART_Transmit (&huart2, "LoRaWAN v1.0.2");
                break;
            case 1:
                USART_Transmit (&huart2, "Semtech proposal");
                break;
            case 2:
                USART_Transmit (&huart2, "KPN/Actility proposal");
                break;
            default:
                USART_Transmit (&huart2, "Unknown proposal");
                break;
        }
        USART_Transmit (&huart2, "\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_Network_Status_RSP
//
//  @brief: Get Network Status ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_Network_Status_RSP (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    uint8_t str[8] = { 0 };
    uint32_t help;
    WiMOD_LoRaWAN_ShowResponse (" Get Network Status ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {

        sprintf (buf, "Network Status: 0x%02X\n\r", rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
        switch (rxMessage->Payload[1])
        {
            case 0x01:
                sprintf (buf, "active (ABP)\n\r");
                USART_Transmit (&huart2, buf);
                break;
            case 0x02:
                sprintf (buf, "active (OTAA)\n\r");
                USART_Transmit (&huart2, buf);
                break;
            case 0x03:
                sprintf (buf, "joining (OTAA)\n\r");
                USART_Transmit (&huart2, buf);
                break;
            default:
                sprintf (buf, "device inactive\n\r");
                USART_Transmit (&huart2, buf);
                return;
        }
        memcpy ((uint8_t *)&help, &rxMessage->Payload[2], 4);

        USART_Transmit (&huart2, "\tDevice address: 0x");
        USART_Transmit (&huart2,
                (const char *)num2hex ((uint32_t)help, DOUBLEWORD_F));
        USART_Transmit (&huart2, "\n\r");

        num2str (rxMessage->Payload[6], str);
        USART_Transmit (&huart2, "\tDR: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");

        num2str (rxMessage->Payload[7], str);
        USART_Transmit (&huart2, "\tPower: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "dBm\n\r");

        num2str (rxMessage->Payload[8], str);
        USART_Transmit (&huart2, "\tMax. payload size: ");
        USART_Transmit (&huart2, (const char *)str);
        USART_Transmit (&huart2, "\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP
//
//  @brief: Get custom config ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP (TWiMOD_HCI_Message *rxMessage)
{
    char buf[300];

    WiMOD_LoRaWAN_ShowResponse (" Get custom cfg ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);
    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {
        sprintf (buf, "RF Gain: %d dBd\n\r", (int)rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP
//
//  @brief: Get device eui ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];

    WiMOD_LoRaWAN_ShowResponse (" Get device eui ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);
    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {
        sprintf (buf, "Device EUI: \n\r");
        USART_Transmit (&huart2, buf);
        for (int var = 1; var < rxMessage->Length; ++var)
        {
            sprintf (buf, " %02X", rxMessage->Payload[var]);
            USART_Transmit (&huart2, buf);
        }
        USART_Transmit (&huart2, "\n\r");
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_Support_Band_RSP
//
//  @brief: Get support band ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_Support_Band_RSP (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];

    WiMOD_LoRaWAN_ShowResponse (" Get support band ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == LORAWAN_STATUS_OK)
    {
        for (int var = 1; var < rxMessage->Length; var += 2)
        {
            sprintf (buf, "Band Idx: %d\n\r", rxMessage->Payload[var]);
            USART_Transmit (&huart2, buf);
            sprintf (buf, "Max. EIRP: %d dBm\n\r", rxMessage->Payload[1 + var]);
            USART_Transmit (&huart2, buf);
        }
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Set_RSTACK_RSP
//
//  @brief: Set Radio STACK ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Set_RSTACK_RSP (TWiMOD_HCI_Message *rxMessage)
{
    char buf[300];

    WiMOD_LoRaWAN_ShowResponse ("Set Radio STACK ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    if (rxMessage->Payload[0] == LORAWAN_STATUS_WRONG_PARAMETER)
    {

        if (rxMessage->Payload[1] & 0x01)
        {
            sprintf (buf, "Wrong Data Rate\n\r");
            USART_Transmit (&huart2, buf);
        }
        if (rxMessage->Payload[1] & 0x02)
        {
            sprintf (buf, "Wrong TX Power Level\n\r");
            USART_Transmit (&huart2, buf);
        }
        if (rxMessage->Payload[1] & 0x20)
        {
            sprintf (buf, "Wrong Band Index\n\r");
            USART_Transmit (&huart2, buf);
        }
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_Get_RSTACK_RSP
//
//  @brief: Get Radio STACK ReSPonse
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_Get_RSTACK_RSP (TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8] = { 0 };

    WiMOD_LoRaWAN_ShowResponse ("Get Radio STACK ReSPonse",
            WiMOD_LoRaWAN_StatusStrings,
            rxMessage->Payload[0]);

    USART_Transmit (&huart2, "Data Rate: ");
    num2str (rxMessage->Payload[1], str);
    USART_Transmit (&huart2, (const char *)str);
    USART_Transmit (&huart2, "\n\r");

    USART_Transmit (&huart2, "TX Power Level (EIRP): ");
    num2str (rxMessage->Payload[2], str);
    USART_Transmit (&huart2, (const char *)str);
    USART_Transmit (&huart2, "\n\r");

    USART_Transmit (&huart2, "Adaptive DR: ");
    if (rxMessage->Payload[3] & 0x1)
    {
        USART_Transmit (&huart2, "enable\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "disable\n\r");
    }
    USART_Transmit (&huart2, "Duty Cycle: ");
    if (rxMessage->Payload[3] & 0x2)
    {
        USART_Transmit (&huart2, "enable\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "disable\n\r");
    }
    USART_Transmit (&huart2, "Class : ");
    if (rxMessage->Payload[3] & 0x4)
    {
        USART_Transmit (&huart2, "C\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "A\n\r");
    }
    USART_Transmit (&huart2, "RF format : ");
    if (rxMessage->Payload[3] & 0x40)
    {
        USART_Transmit (&huart2, "extended\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "standard\n\r");
    }
    USART_Transmit (&huart2, "MAC forwarding : ");
    if (rxMessage->Payload[3] & 0x80)
    {
        USART_Transmit (&huart2, "enable\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "disable\n\r");
    }

    USART_Transmit (&huart2, "Power Saving Mode: ");
    if (rxMessage->Payload[4] & 0x1)
    {
        USART_Transmit (&huart2, "automatic\n\r");
    }
    else
    {
        USART_Transmit (&huart2, "off\n\r");
    }

    USART_Transmit (&huart2, "Retransmissions: ");
    num2str (rxMessage->Payload[5], str);
    USART_Transmit (&huart2, (const char *)str);
    USART_Transmit (&huart2, "\n\r");

    USART_Transmit (&huart2, "BandIdx: ");
    num2str (rxMessage->Payload[6], str);
    USART_Transmit (&huart2, (const char *)str);
    USART_Transmit (&huart2, "\n\r");

    USART_Transmit (&huart2, "MAC length: ");
    num2str (rxMessage->Payload[7], str);
    USART_Transmit (&huart2, (const char *)str);
    USART_Transmit (&huart2, "\n\r");
}
//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_MAC_CMDRxIndication
//
//  @brief: show received MAC cmd
//
//------------------------------------------------------------------------------
    static void
WiMOD_LoRaWAN_Process_MAC_CMDRxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    int payloadSize = rxMessage->Length - 1;
    //
    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
        payloadSize -= 5;

    if (payloadSize >= 1)
    {
        USART_Transmit (&huart2, "MAC rx event:\n\rmac-payload: 0x");
        for (int i = 0; i < payloadSize; i++)
            USART_Transmit (&huart2, (const char *)num2hex (
                        rxMessage->Payload[1 + i], BYTE_F));
        USART_Transmit (&huart2, "\n\r");
    }

    //    if (rxMessage->Payload[0] & 0x02)
    //        printf("ack for uplink packet:yes\n\r");
    //    else
    //        printf("ack for uplink packet:no\n\r");
    //
    //    if (rxMessage->Payload[0] & 0x04)
    //        printf("frame pending:yes\n\r");
    //    else
    //        printf("frame pending:no\n\r");

    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
    {
        UINT8 *rxInfo = &rxMessage->Payload[1 + payloadSize];
        sprintf (buf, "ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                (int)rxInfo[0], (int)rxInfo[1], (int)rxInfo[2], (int)rxInfo[3],
                (int)rxInfo[4]);
        USART_Transmit (&huart2, buf);
    }
}
//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_Process_C_DataRxIndication
//
//  @brief: show received C-Data
//
//------------------------------------------------------------------------------

    void
WiMOD_LoRaWAN_Process_C_DataRxIndication (TWiMOD_HCI_Message *rxMessage)
{
    char buf[350];
    int payloadSize = rxMessage->Length - 1;

    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
        payloadSize -= 5;

    if (payloadSize >= 1)
    {
        sprintf (buf, "C-Data rx event: port:0x%02X\n\rapp-payload:",
                rxMessage->Payload[1]);
        USART_Transmit (&huart2, buf);
        for (int i = 1; i < payloadSize;)
            USART_Transmit (&huart2, (const char *)num2hex (
                        rxMessage->Payload[1 + i], BYTE_F));
        USART_Transmit (&huart2, "\n\r");
    }

    if (rxMessage->Payload[0] & 0x02)
    {
        sprintf (buf, "ack for uplink packet:yes\n\r");
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "ack for uplink packet:no\n\r");
        USART_Transmit (&huart2, buf);
    }

    if (rxMessage->Payload[0] & 0x04)
    {
        sprintf (buf, "frame pending:yes\n\r");
        USART_Transmit (&huart2, buf);
    }
    else
    {
        sprintf (buf, "frame pending:no\n\r");
        USART_Transmit (&huart2, buf);
    }

    // rx channel info attached ?
    if (rxMessage->Payload[0] & 0x01)
    {
        UINT8 *rxInfo = &rxMessage->Payload[1 + payloadSize];
        sprintf (buf, "ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                (int)rxInfo[0], (int)rxInfo[1], (int)rxInfo[2], (int)rxInfo[3],
                (int)rxInfo[4]);
        USART_Transmit (&huart2, buf);
    }
}

//------------------------------------------------------------------------------
//
//  WiMOD_LoRaWAN_ShowResponse
//
//  @brief: show response status as human readable string
//
//------------------------------------------------------------------------------

    static void
WiMOD_LoRaWAN_ShowResponse (const char *string, const TIDString *statusTable,
        UINT8 statusID)
{
    while (statusTable->String)
    {
        if (statusTable->ID == statusID)
        {
            USART_Transmit (&huart2, string);
            USART_Transmit (&huart2, " - Status(0x");
            USART_Transmit (&huart2, (const char *)num2hex (statusID, BYTE_F));
            USART_Transmit (&huart2, ") ");
            USART_Transmit (&huart2, statusTable->String);
            USART_Transmit (&huart2, "\n\r");
            return;
        }

        statusTable++;
    }
}
//------------------------------------------------------------------------------
// end of file
//------------------------------------------------------------------------------
