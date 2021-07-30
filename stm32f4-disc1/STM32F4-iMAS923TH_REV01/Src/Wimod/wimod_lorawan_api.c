/**
 ******************************************************************************
 * @file           : wimod_lorawan_api.c
 * @abstract       : API Layer of LoRaWAN Host Controller Interface
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
#include "wimod_lorawan_api.h"

/* HCI Message Receiver callback ---------------------------------------------*/
static TWiMOD_HCI_Message *WiMOD_LoRaWAN_Process_RxMessage
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_DevMgmt_Message
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_LoRaWAN_Message
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Reactivate_Rsp
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_U_DATA_TxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_C_DATA_TxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_JoinTxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_JoinNetworkIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_U_DataRxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_C_DataRxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_MAC_CMDRxIndication
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Set_RSTACK_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_RSTACK_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_Support_Band_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_Network_Status_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP
(TWiMOD_HCI_Message *rxMessage);

static void WiMOD_LoRaWAN_ShowResponse
(const char *string, const TIDString *statusTable, uint8_t statusID);

/* Private variables ---------------------------------------------------------*/
/* Reserve one Tx-Message ----------------------------------------------------*/
TWiMOD_HCI_Message TxMessage;

/* Reserve one Rx-Message ----------------------------------------------------*/
TWiMOD_HCI_Message RxMessage;

/* Status Codes for DeviceMgmt Response Messages -----------------------------*/
static const TIDString WiMOD_DeviceMgmt_StatusStrings[] = {
    { DEVMGMT_STATUS_OK, "ok"                                   },
    { DEVMGMT_STATUS_ERROR, "error"                             },
    { DEVMGMT_STATUS_CMD_NOT_SUPPORTED, "command not supported" },
    { DEVMGMT_STATUS_WRONG_PARAMETER, "wrong parameter"         },
    { DEVMGMT_STATUS_WRONG_DEVICE_MODE, "wrong device mode"     },
    { 0x00U, 0x00U                                              } 
    /* End of table */
};

/* Status Codes for LoRaWAN Response Messages --------------------------------*/
static const TIDString WiMOD_LoRaWAN_StatusStrings[] = {
    { LORAWAN_STATUS_OK, "ok"                                              },
    { LORAWAN_STATUS_ERROR, "error"                                        },
    { LORAWAN_STATUS_CMD_NOT_SUPPORTED, "command not supported"            },
    { LORAWAN_STATUS_WRONG_PARAMETER, "wrong parameter"                    },
    { LORAWAN_STATUS_WRONG_DEVICE_MODE, "wrong device mode"                },
    { LORAWAN_STATUS_DEVICE_NOT_ACTIVATED, "device not activated"          },
    { LORAWAN_STATUS_DEVICE_BUSY, "device busy - command rejected"         },
    { LORAWAN_STATUS_QUEUE_FULL, "message queue full - command rejected"   },
    { LORAWAN_STATUS_LENGTH_ERROR, "HCI message length error"              },
    { LORAWAN_STATUS_NO_FACTORY_SETTINGS, "no factory settings available"  },
    { LORAWAN_STATUS_CHANNEL_BLOCKED_BY_DC,
        "error: channel blocked due to duty cycle, please try later again" },
    { LORAWAN_STATUS_CHANNEL_NOT_AVAILABLE, "error: channel not available" },
    { 0x00U, 0x00U                                                         } 
    /* End of table */
};

/* Status Codes for DeviceMgmt Response Messages -----------------------------*/
static const TIDString WiMOD_DeviceMgmt_ModuleTypes[] = {
    { 0x90U, "iM880A (obsolete)"               },
    { 0x92U, "iM880A-L (128k)"                 },
    { 0x93U, "iU880A (128k)"                   },
    { 0x98U, "iM880B-L"                        },
    { 0x99U, "iU880B"                          },
    { 0x9AU, "iM980A (iMAS923TH for Thailand)" },
    { 0xA0U, "iM881A"                          },
    { 0x00U, 0x00U                             } 
    /* End of table */
};

/**
 * @brief  init complete interface
 * @param
 * @retval uint8_t
 */
uint8_t WiMOD_LoRaWAN_Init(UART_HandleTypeDef *comPort)
{
    /* Init HCI layer */
    return WiMOD_HCI_Init(comPort,           /* init serial port */
            WiMOD_LoRaWAN_Process_RxMessage, /* Receiver callback */
            &RxMessage);                     /* Receive message */
}

/**
 * @brief  Device management message request
 * @param
 * @retval int32_t
 */
int32_t WiMOD_DevMgmt_Msg_Req(uint8_t msg_id, uint8_t *val, uint16_t len)
{
    TxMessage.SapID = DEVMGMT_SAP_ID;
    TxMessage.MsgID = msg_id;

    memset(TxMessage.Payload, 0U, WIMOD_HCI_MSG_PAYLOAD_SIZE);

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
            TxMessage.Length = 0U;
            break;
        case DEVMGMT_MSG_SET_RTC_REQ:
        case DEVMGMT_MSG_SET_RTC_ALARM_REQ:
            TxMessage.Length = 4U;
            break;
        case DEVMGMT_MSG_SET_OPMODE_REQ:
            TxMessage.Length = 1U;
            break;
        default:
            return DEVMGMT_STATUS_CMD_NOT_SUPPORTED;
    }

    if (TxMessage.Length > 0U)
    {
        memcpy(TxMessage.Payload, val, TxMessage.Length);
    }

    return (WiMOD_HCI_SendMessage(&TxMessage));
}

/**
 * @brief  Device management message request
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_Msg_Req(uint8_t msg_id, uint8_t *val, uint16_t len)
{
    TxMessage.SapID = LORAWAN_SAP_ID;
    TxMessage.MsgID = msg_id;

    memset(TxMessage.Payload, 0U, WIMOD_HCI_MSG_PAYLOAD_SIZE);

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
        memcpy(TxMessage.Payload, val, TxMessage.Length);
    }

    return (WiMOD_HCI_SendMessage(&TxMessage));
}

/**
 * @brief  Get custom config
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetCustomConfig(void) 
{ 
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_CUSTOM_CFG_REQ, NULL, 0U); 
}

/**
 * @brief  Set custom config
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetCustomConfig(int8_t val)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_CUSTOM_CFG_REQ, (uint8_t *)&val, 1U);
}

/**
 * @brief  Set OPMODE
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetOPMODE(uint8_t val)
{
    if (val > 1U)
    {
        return (-1);
    }

    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_SET_OPMODE_REQ, &val, 1U);
}

/**
 * @brief  Get OPMODE
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetOPMODE(void)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_OPMODE_REQ, NULL, 0U);
}

/**
 * @brief  Clear RTC Alarm
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_ClearRTCAlarm(void)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_CLEAR_RTC_ALARM_REQ, NULL, 0U);
}

/**
 * @brief  Get RTC Alarm
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetRTCAlarm(void)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_RTC_ALARM_REQ, NULL, 0U);
}

/**
 * @brief  Set RTC Alarm
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetRTCAlarm(uint8_t *alarm)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_SET_RTC_ALARM_REQ, alarm, 4U);
}

/**
 * @brief  Get RTC
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetRTC(void)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_RTC_REQ, NULL, 0U);
}

/**
 * @brief  Set RTC
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetRTC(uint32_t *time)
{ 
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_SET_RTC_REQ, (uint8_t *)time, 4U);
}

/**
 * @brief  Ping device
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SendPing(void)
{
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_PING_REQ, NULL, 0U);
}

/**
 * @brief  Get device information
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetDeviceInfo(void)
{
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_DEVICE_INFO_REQ, NULL, 0U);
}

/**
 * @brief  Get firmware version
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetFirmwareVersion(void)
{
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_FW_VERSION_REQ, NULL, 0U);
}

/**
 * @brief  Get network status
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetNkwStatus(void)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_NWK_STATUS_REQ, NULL, 0U);
}

/**
 * @brief  Factory reset
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_FactoryReset(void)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_FACTORY_RESET_REQ, NULL, 0U);
}

/**
 * @brief  GetLinkADR
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetLinkADR(void)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_GET_LINKADRREQ_CONFIG_REQ, NULL, 0U);
}

/**
 * @brief  SetLinkADR
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetLinkADR(uint8_t *payload)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_LINKADRREQ_CONFIG_REQ, payload, 1U);
}

/**
 * @brief  SetMAC_CMD 
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetMAC_CMD(uint8_t *payload)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SEND_MAC_CMD_REQ, payload, 16U);
}

/**
 * @brief  Reset device
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_Reset(void)
{
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_RESET_REQ, NULL, 0U);
}

/**
 * @brief  Get device status
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_GetDeviceStatus(void)
{
    return WiMOD_DevMgmt_Msg_Req(DEVMGMT_MSG_GET_DEVICE_STATUS_REQ, NULL, 0U);
}

/**
 * @brief  Send set join parameters
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SetJoinParameters(uint8_t *payload)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SET_JOIN_PARAM_REQ, payload, 24U);
}


/**
 * @brief  Send join radio message
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_JoinNetworkRequest(void)
{
    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_JOIN_NETWORK_REQ, NULL, 0U);
}

/**
 * @brief  Send unconfirmed radio message
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SendURadioData(
        uint8_t port,     /* LoRaWAN Port */
        uint8_t *srcData, /* Application payload */
        int srcLength)    /* Length of application payload */
{
    uint8_t payload[WIMOD_HCI_MSG_PAYLOAD_SIZE] = { 0U };

    /* 1. Check length */
    if (srcLength > (WIMOD_HCI_MSG_PAYLOAD_SIZE - 1U))
    {
        /* Overflow error */
        return (-1);
    }

    /* 
     * 2.  Init payload
     * 2.1 Init port 
     */
    payload[0U] = port;

    /* 2.2 init radio message payload */
    memcpy(&payload[1U], srcData, srcLength);

    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SEND_UDATA_REQ, payload, 1U + srcLength);
}

/**
 * @brief  Send confirmed radio message
 * @param
 * @retval int32_t
 */
int32_t WiMOD_LoRaWAN_SendCRadioData(
        uint8_t port,     /* LoRaWAN Port */
        uint8_t *srcData, /* Application data */
        int srcLength)    /* Length of application data */
{
    uint8_t payload[WIMOD_HCI_MSG_PAYLOAD_SIZE] = { 0U };

    /* 1. Check length */
    if (srcLength > (WIMOD_HCI_MSG_PAYLOAD_SIZE - 1U))
    {
        /* Overflow error */
        return (-1);
    }

    /* 
     * 2.  Init payload
     * 2.1 Init port 
     */
    payload[0U] = port;

    /* 2.2 Init radio message payload */
    memcpy(&payload[1U], srcData, srcLength);

    return WiMOD_LoRaWAN_Msg_Req(LORAWAN_MSG_SEND_CDATA_REQ, payload, 1U + srcLength);
}

/**
 * @brief  Handle receiver process
 * @param
 * @retval void
 */
void WiMOD_LoRaWAN_Process(void)
{
    /* Call HCI process */
    WiMOD_HCI_Process();
}

/**
 * @brief  Handle receiver message 
 * @param
 * @retval None
 */
static TWiMOD_HCI_Message *WiMOD_LoRaWAN_Process_RxMessage(TWiMOD_HCI_Message *rxMessage)
{
    switch (rxMessage->SapID)
    {
        case DEVMGMT_SAP_ID:
            WiMOD_LoRaWAN_Process_DevMgmt_Message(rxMessage);
            break;
        case LORAWAN_SAP_ID:
            WiMOD_LoRaWAN_Process_LoRaWAN_Message(rxMessage);
            break;
    }

    return (&RxMessage);
}

/**
 * @brief  handle received Device Management SAP messages 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_DevMgmt_Message(TWiMOD_HCI_Message *rxMessage)
{
    switch (rxMessage->MsgID)
    {
        case DEVMGMT_MSG_PING_RSP:
            WiMOD_LoRaWAN_ShowResponse("ping response",
                    WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);
            break;
        case DEVMGMT_MSG_GET_DEVICE_INFO_RSP:
            WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp(rxMessage);
            break;
        case DEVMGMT_MSG_GET_FW_VERSION_RSP:
            WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp(rxMessage);
            break;
        case DEVMGMT_MSG_RESET_RSP:
            WiMOD_LoRaWAN_ShowResponse("reset response",
                    WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);
            break;
        case DEVMGMT_MSG_GET_DEVICE_STATUS_RSP:
            WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp(rxMessage);
            break;
        case DEVMGMT_MSG_GET_RTC_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp(rxMessage);
            break;
        case DEVMGMT_MSG_SET_RTC_RSP:
            WiMOD_LoRaWAN_ShowResponse("set RTC response",
                    WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);
            break;
        case DEVMGMT_MSG_GET_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp(rxMessage);
            break;
        case DEVMGMT_MSG_SET_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_ShowResponse("set RTC Alarm response",
                    WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);
            break;
        case DEVMGMT_MSG_CLEAR_RTC_ALARM_RSP:
            WiMOD_LoRaWAN_ShowResponse("clear RTC Alarm response",
                    WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);
            break;
        case DEVMGMT_MSG_GET_OPMODE_RSP:
            WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp(rxMessage);
            break;

        default:
            SerialDevice_Printf("Unhandled DeviceMgmt message received - MsgID: 0x""%02X\n\r",
                    (uint8_t)rxMessage->MsgID);
            break;
    }
}

/**
 * @brief  Get OPMODE 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_Get_OPMODE_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse("get OPMODE response", WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {
        SerialDevice_Printf("OPMODE Set: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)rxMessage->Payload[1], BYTE_F));
        SerialDevice_Printf("\n\r");
    }
}

/**
 * @brief  Get RTC alarm 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_Get_RTC_ALARM_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;
    WiMOD_LoRaWAN_ShowResponse("get RTC ALARM response", 
            WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {
        SerialDevice_Printf("RTC ALARM Set: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)rxMessage->Payload[1U], BYTE_F));
        SerialDevice_Printf("\n\r");

        SerialDevice_Printf("RTC ALARM Daily: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)rxMessage->Payload[2U], BYTE_F));
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[3U], 3U);
        SerialDevice_Printf("RTC ALARM Time: [");
        num2str((help & 0xFFU), str); /* Hour */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str((help & 0xFF00U) >> 8U, str); /* Minute */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str((help & 0xFF0000U) >> 16U, str); /* Second */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("]\n\r");
    }
}

/**
 * @brief  Get RTC 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_Get_RTC_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;
    WiMOD_LoRaWAN_ShowResponse("get RTC response",
            WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {
        memcpy((uint8_t *)&help, &rxMessage->Payload[1U], 4U);
        SerialDevice_Printf("RTC Time: ");
        num2str(((help & 0xFC000000U) >> 26U) + 2000U, str); /* Year */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("-");
        num2str(((help & 0xF000U) >> 12U), str); /* Months */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("-");
        num2str(((help & 0x3E00000U) >> 21U), str); /* Day */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(" [");
        num2str(((help & 0x1F0000U) >> 16U), str); /* Hour */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str(((help & 0xFC0U) >> 6U), str); /* Minutes */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str(((help & 0x3FU)), str); /* Seconds */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("]\n\r");
    }
}

/**
 * @brief  Show device status 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_DeviceStatus_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;
    WiMOD_LoRaWAN_ShowResponse("device status response",
            WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {
        memcpy((uint8_t *)&help, &rxMessage->Payload[1U], 1U);
        num2str(help & 0xFFU, str);
        SerialDevice_Printf("System Tick Resolution: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(" ms\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[2U], 4U);
        num2str(help, str);
        SerialDevice_Printf("System Tick: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[6U], 4U);
        SerialDevice_Printf("Target Time: ");
        num2str(((help & 0xFC000000U) >> 26U) + 2000U, str); /* Year */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("-");
        num2str(((help & 0xF000U) >> 12U), str); /* Months */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("-");
        num2str(((help & 0x3E00000U) >> 21U), str); /* Day */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(" [");
        num2str(((help & 0x1F0000U) >> 16U), str); /* Hour */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str(((help & 0xFC0) >> 6U), str); /* Minutes */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(":");
        num2str(((help & 0x3F)), str); /* Seconds */
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("]\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[10U], 2U);
        num2str(help & 0xFFFFU, str);
        SerialDevice_Printf("NVM Status: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[12U], 2U);
        num2str(help & 0xFFFFU, str);
        SerialDevice_Printf("Battery level: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(" mV\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[16U], 4U);
        num2str(help, str);
        SerialDevice_Printf("TX U-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[20U], 4U);
        num2str(help, str);
        SerialDevice_Printf("TX C-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[24U], 4U);
        num2str(help, str);
        SerialDevice_Printf("TX Error: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[28U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX1 U-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[32U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX1 C-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[36U], 4);
        num2str(help, str);
        SerialDevice_Printf("RX1 MIC-Error: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[40U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX2 U-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[44U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX2 C-Data: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[48U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX2 MIC-Error: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[52U], 4U);
        num2str(help, str);
        SerialDevice_Printf("TX Join: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[56U], 4U);
        num2str(help, str);
        SerialDevice_Printf("RX Accept: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");
    }
}

/**
 * @brief  Show device information 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_DeviceInfo_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;
    WiMOD_LoRaWAN_ShowResponse("device information response",
            WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {
        WiMOD_LoRaWAN_ShowResponse("Module type",
                WiMOD_DeviceMgmt_ModuleTypes, rxMessage->Payload[1U]);

        memcpy((uint8_t *)&help, &rxMessage->Payload[2U], 4U);
        SerialDevice_Printf("Device address: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)help, DOUBLEWORD_F));
        SerialDevice_Printf("\n\r");

        memcpy((uint8_t *)&help, &rxMessage->Payload[6U], 4U);
        SerialDevice_Printf("Device ID: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)help, DOUBLEWORD_F));
        SerialDevice_Printf("(");
        num2str(help, str);
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(")\n\r");
    }
}

/**
 * @brief  Show firmware version 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_DevMgmt_FirmwareVersion_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    char help[80U];

    WiMOD_LoRaWAN_ShowResponse("firmware version response",
            WiMOD_DeviceMgmt_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == DEVMGMT_STATUS_OK)
    {

        SerialDevice_Printf("version: V%d.%d\n\r",
                (int32_t)rxMessage->Payload[2U], (int32_t)rxMessage->Payload[1U]);
        SerialDevice_Printf("build-count: %d\n\r",
                (int32_t)MAKEWORD(rxMessage->Payload[3U], rxMessage->Payload[4U]));

        memcpy(help, &rxMessage->Payload[5U], 10U);
        help[10U] = 0U;
        SerialDevice_Printf("build-date: %s\n\r", help);

        /* More information attached ? */
        if (rxMessage->Length > 15U)
        {
            /* Add string termination */
            rxMessage->Payload[rxMessage->Length] = 0U;
            SerialDevice_Printf("firmware-content: %s\n\r", &rxMessage->Payload[15U]);
        }
    }
}

/**
 * @brief  Handle received LoRaWAN SAP messages 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_LoRaWAN_Message(TWiMOD_HCI_Message *rxMessage)
{
    switch (rxMessage->MsgID)
    {
        case LORAWAN_MSG_ACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_ShowResponse("activate device response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_REACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_Process_Reactivate_Rsp(rxMessage);
            break;
        case LORAWAN_MSG_DEACTIVATE_DEVICE_RSP:
            WiMOD_LoRaWAN_ShowResponse("deactivate device response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_SET_JOIN_PARAM_RSP:
            WiMOD_LoRaWAN_ShowResponse("set join param response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_JOIN_NETWORK_RSP:
            WiMOD_LoRaWAN_ShowResponse("join network response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_SEND_UDATA_RSP:
            WiMOD_LoRaWAN_ShowResponse("send U-Data response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_SEND_UDATA_IND:
            WiMOD_LoRaWAN_Process_U_DATA_TxIndication(rxMessage);
            break;
        case LORAWAN_MSG_SEND_CDATA_RSP:
            WiMOD_LoRaWAN_ShowResponse("send C-Data response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_SEND_CDATA_IND:
            WiMOD_LoRaWAN_Process_C_DATA_TxIndication(rxMessage);
            break;
        case LORAWAN_MSG_JOIN_TRANSMIT_IND:
            WiMOD_LoRaWAN_Process_JoinTxIndication(rxMessage);
            break;
        case LORAWAN_MSG_JOIN_NETWORK_IND:
            WiMOD_LoRaWAN_Process_JoinNetworkIndication(rxMessage);
            break;
        case LORAWAN_MSG_RECV_UDATA_IND:
            WiMOD_LoRaWAN_Process_U_DataRxIndication(rxMessage);
            break;
        case LORAWAN_MSG_RECV_CDATA_IND:
            WiMOD_LoRaWAN_Process_C_DataRxIndication(rxMessage);
            break;
        case LORAWAN_MSG_SEND_MAC_CMD_RSP:
            WiMOD_LoRaWAN_ShowResponse("send MAC CMD response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_RECV_MAC_CMD_IND:
            WiMOD_LoRaWAN_Process_MAC_CMDRxIndication(rxMessage);
            break;
        case LORAWAN_MSG_RECV_NODATA_IND:
            SerialDevice_Printf("no data received indication\n\r");
            break;
        case LORAWAN_MSG_SET_RSTACK_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Set_RSTACK_RSP(rxMessage);
            break;
        case LORAWAN_MSG_GET_RSTACK_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Get_RSTACK_RSP(rxMessage);
            break;
        case LORAWAN_MSG_GET_SUPPORTED_BANDS_RSP:
            WiMOD_LoRaWAN_Process_Get_Support_Band_RSP(rxMessage);
            break;
        case LORAWAN_MSG_GET_DEVICE_EUI_RSP:
            WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP(rxMessage);
            break;
        case LORAWAN_MSG_SET_DEVICE_EUI_RSP:
            WiMOD_LoRaWAN_ShowResponse("set device eui response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_GET_CUSTOM_CFG_RSP:
            WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP(rxMessage);
            break;
        case LORAWAN_MSG_SET_CUSTOM_CFG_RSP:
            WiMOD_LoRaWAN_ShowResponse("set custom cfg response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_FACTORY_RESET_RSP:
            WiMOD_LoRaWAN_ShowResponse("factory reset response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        case LORAWAN_MSG_GET_NWK_STATUS_RSP:
            WiMOD_LoRaWAN_Process_Get_Network_Status_RSP(rxMessage);
            break;
        case LORAWAN_MSG_GET_LINKADRREQ_CONFIG_RSP:
            WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP(rxMessage);
            break;
        case LORAWAN_MSG_SET_LINKADRREQ_CONFIG_RSP:
            WiMOD_LoRaWAN_ShowResponse("set link adr response",
                    WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);
            break;
        default:
            SerialDevice_Printf("Unhandled LoRaWAN SAP message ""received - MsgID : 0x");
            SerialDevice_Printf((const char *)num2hex(rxMessage->MsgID, BYTE_F));
            SerialDevice_Printf("\n\r");
            break;
    }
}

/**
 * @brief  Reactivate device 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Reactivate_Rsp(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;
    WiMOD_LoRaWAN_ShowResponse("reactivate device response",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        memcpy((uint8_t *)&help, &rxMessage->Payload[1U], 4U);
        num2str(help, str); /* Second */
        SerialDevice_Printf("Device address: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)help, DOUBLEWORD_F));
        SerialDevice_Printf("(");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf(")\n\r");
    }
}

/**
 * @brief  Unconfirm data transmit indicaton 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_C_DATA_TxIndication(TWiMOD_HCI_Message *rxMessage)
{

    if (rxMessage->Payload[0U] == 0x00U)
    {
        SerialDevice_Printf("c data tx event : radio packet sent\n\r");
    }
    else if (rxMessage->Payload[0U] == 0x01U) /* Channel info attached ? */
    {
        SerialDevice_Printf("c data tx event : radio packet sent with tx channel info attached\n\r");
        SerialDevice_Printf("c data tx event ChIdx %d\n\r", (int)rxMessage->Payload[1U]);
        SerialDevice_Printf("\tDR: %ld\n\r", (int32_t)rxMessage->Payload[2U]);
        SerialDevice_Printf("\tNum: %ld\n\r", (int32_t)rxMessage->Payload[3U]);
        SerialDevice_Printf("\tPower(dBm): %ld\n\r", (int32_t)rxMessage->Payload[4U]);
        SerialDevice_Printf("\tAirtime (ms): %ln\n\r", (uint32_t *)&rxMessage->Payload[5U]);
    }
    else if (rxMessage->Payload[0U] & 0x02U)
    {
        SerialDevice_Printf("error, maximum number of retransmissions reached\n\r");
    }
    else if (rxMessage->Payload[0U] & 0x04U)
    {
        SerialDevice_Printf("maximum payload size exceeded for current data rate\n\r");
    }
    else
    {
        SerialDevice_Printf("c data tx event - Status : error\n\r");
    }
}

/**
 * @brief  Unconfirm data transmit indicaton 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_U_DATA_TxIndication(TWiMOD_HCI_Message *rxMessage)
{
    if (rxMessage->Payload[0U] == 0x00U)
    {
        SerialDevice_Printf("u data tx event : radio packet sent\n\r");
    }
    else if (rxMessage->Payload[0U] == 0x01U) /* Channel info attached ? */
    {
        SerialDevice_Printf("u data tx event : radio packet sent with "
                "tx channel info ""attached\n\r");
        SerialDevice_Printf("u data tx event ChIdx %ld\n\r", (int32_t)rxMessage->Payload[1U]);
        SerialDevice_Printf("\tDR: %ld\n\r", (int32_t)rxMessage->Payload[2U]);
        SerialDevice_Printf("\tNum: %ld\n\r", (int32_t)rxMessage->Payload[3U]);
        SerialDevice_Printf("\tPower(dBm): %ld\n\r", (int32_t)rxMessage->Payload[4U]);
        SerialDevice_Printf("\tAirtime (ms): %ln\n\r", (uint32_t *)&rxMessage->Payload[5U]);
    }
    else
    {
        SerialDevice_Printf("u data tx event - Status : error\n\r");
    }
}

/**
 * @brief  Show join transmit indicaton 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_JoinTxIndication(TWiMOD_HCI_Message *rxMessage)
{
    if (rxMessage->Payload[0U] == 0x00U)
    {
        SerialDevice_Printf("join tx event - Status : ok\n\r");
    }
    else if (rxMessage->Payload[0U] == 0x01U) /* Channel info attached ? */
    {
        SerialDevice_Printf("join tx event:%d, ChnIdx:%d, DR:%d - Status : ok\n\r",
                (uint8_t)rxMessage->Payload[3U],
                (uint8_t)rxMessage->Payload[1U],
                (uint8_t)rxMessage->Payload[2U]);
    }
    else
    {
        SerialDevice_Printf("join tx event - Status : error\n\r");
    }
}

/**
 * @brief  Show join network indicaton 
 * @param
 * @retval None
 */
void WiMOD_LoRaWAN_Process_JoinNetworkIndication(TWiMOD_HCI_Message *rxMessage)
{
    uint32_t address = 0U; 

    if (rxMessage->Payload[0U] == 0x00U)
    {
        address = MAKELONG(MAKEWORD(rxMessage->Payload[1U],
                           rxMessage->Payload[2U]),
                           MAKEWORD(rxMessage->Payload[3U],
                           rxMessage->Payload[4U]));
        SerialDevice_Printf("join network accept event - DeviceAddress:0x%08lX\n\r", address);
    }
    else if (rxMessage->Payload[0U] == 0x01U)
    {
        address = MAKELONG(MAKEWORD(rxMessage->Payload[1U],
                           rxMessage->Payload[2U]),
                           MAKEWORD(rxMessage->Payload[3U],
                           rxMessage->Payload[4U]));
        SerialDevice_Printf("join network accept event - "
                "DeviceAddress:0x%08lX, ChnIdx:%d, "
                "DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                address, (int)rxMessage->Payload[5U],
                (uint8_t)rxMessage->Payload[6U], (uint8_t)rxMessage->Payload[7U],
                (uint8_t)rxMessage->Payload[8U], (uint8_t)rxMessage->Payload[9U]);
    }
    else
    {
        SerialDevice_Printf("join network timeout event\n\r");
    }
}

/**
 * @brief  Show received U-Data 
 * @param
 * @retval None
 */
void WiMOD_LoRaWAN_Process_U_DataRxIndication(TWiMOD_HCI_Message *rxMessage)
{
    uint16_t i           = 0U;
    uint16_t payloadSize = rxMessage->Length - 1U;
    uint8_t *rxInfo      = NULL;

    /* RX channel info attached ? */
    if (rxMessage->Payload[0U] & 0x01U)
        payloadSize -= 5U;

    if (payloadSize >= 0x01U)
    {
        SerialDevice_Printf("U-Data rx event: port:0x");
        SerialDevice_Printf((const char *)num2hex(rxMessage->Payload[1U], BYTE_F));
        SerialDevice_Printf("\n\rapp-payload: 0x");
        for (i = 1U; i < payloadSize; i++)
        {
            SerialDevice_Printf((const char *)num2hex(rxMessage->Payload[1U + i], BYTE_F));
        }
        SerialDevice_Printf("\n\r");
    }

    if (rxMessage->Payload[0U] & 0x02U)
    {
        SerialDevice_Printf("ack for uplink packet:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("ack for uplink packet:no\n\r");
    }

    if (rxMessage->Payload[0U] & 0x04U)
    {
        SerialDevice_Printf("frame pending:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("frame pending:no\n\r");
    }

    /* RX channel info attached ? */
    if (rxMessage->Payload[0U] & 0x01U)
    {
        rxInfo = &rxMessage->Payload[1U + payloadSize];
        SerialDevice_Printf("ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                   (uint8_t)rxInfo[0U],
                   (uint8_t)rxInfo[1U],
                   (uint8_t)rxInfo[2U],
                   (uint8_t)rxInfo[3U],
                   (uint8_t)rxInfo[4U]);
    }
}

/**
 * @brief  show received C-Data 
 * @param
 * @retval None
 */
void WiMOD_LoRaWAN_Process_C_DataRxIndication(TWiMOD_HCI_Message *rxMessage)
{
    uint16_t i           = 0U;
    uint16_t payloadSize = rxMessage->Length - 1U;
    uint8_t *rxInfo      = NULL;

    /* RX channel info attached ? */
    if (rxMessage->Payload[0U] & 0x01U)
    {
        payloadSize -= 5U;
    }

    if (payloadSize >= 0x01)
    {
        SerialDevice_Printf("C-Data rx event: port:0x%02X\n\rapp-payload:", rxMessage->Payload[1U]);
        for (i = 1U; i < payloadSize; i++)
        {
            SerialDevice_Printf((const char *)num2hex(rxMessage->Payload[1U + i], BYTE_F));
        }
        SerialDevice_Printf("\n\r");
    }

    if (rxMessage->Payload[0U] & 0x02U)
    {
        SerialDevice_Printf("ack for uplink packet:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("ack for uplink packet:no\n\r");
    }

    if (rxMessage->Payload[0U] & 0x04U)
    {
        SerialDevice_Printf("frame pending:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("frame pending:no\n\r");
    }

    /* RX channel info attached ? */
    if (rxMessage->Payload[0U] & 0x01U)
    {
        rxInfo = (&rxMessage->Payload[1U + payloadSize]);

        SerialDevice_Printf("ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                   (uint8_t)rxInfo[0U],
                   (uint8_t)rxInfo[1U],
                   (uint8_t)rxInfo[2U],
                   (uint8_t)rxInfo[3U],
                   (uint8_t)rxInfo[4U]);
    }
}

/**
 * @brief  Get LINK ADR ReSPonse 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_LINK_ADR_RSP(TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse("Get Network Status ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        SerialDevice_Printf("LinkADRReq Option: ");
        switch (rxMessage->Payload[1U])
        {
            case 0x00U:
                SerialDevice_Printf("LoRaWAN v1.0.2");
                break;
            case 0x01U:
                SerialDevice_Printf("Semtech proposal");
                break;
            case 0x02U:
                SerialDevice_Printf("KPN/Actility proposal");
                break;
            default:
                SerialDevice_Printf("Unknown proposal");
                break;
        }
        SerialDevice_Printf("\n\r");
    }
}

/**
 * @brief  Get network status response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_Network_Status_RSP(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };
    uint32_t help  = 0U;

    WiMOD_LoRaWAN_ShowResponse("Get Network Status ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        SerialDevice_Printf("Network Status: 0x%02X\n\r", rxMessage->Payload[1U]);

        switch (rxMessage->Payload[1])
        {
            case 0x01U:
                SerialDevice_Printf("active (ABP)\n\r");
                break;
            case 0x02U:
                SerialDevice_Printf("active (OTAA)\n\r");
                break;
            case 0x03U:
                SerialDevice_Printf("joining (OTAA)\n\r");
                break;
            default:
                SerialDevice_Printf("device inactive\n\r");
                return;
        }

        memcpy((uint8_t *)&help, &rxMessage->Payload[2U], 4U);
        SerialDevice_Printf("\tDevice address: 0x");
        SerialDevice_Printf((const char *)num2hex((uint32_t)help, DOUBLEWORD_F));
        SerialDevice_Printf("\n\r");

        num2str(rxMessage->Payload[6U], str);
        SerialDevice_Printf("\tDR: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");

        num2str(rxMessage->Payload[7U], str);
        SerialDevice_Printf("\tPower: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("dBm\n\r");

        num2str(rxMessage->Payload[8U], str);
        SerialDevice_Printf("\tMax. payload size: ");
        SerialDevice_Printf((const char *)str);
        SerialDevice_Printf("\n\r");
    }
}

/**
 * @brief  Get custom config response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_Custom_CFG_RSP(TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse(" Get custom cfg ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        SerialDevice_Printf("RF Gain: %d dBd\n\r", (int)rxMessage->Payload[1U]);
    }
}

/**
 * @brief  Get device eui response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_Device_EUI_RSP(TWiMOD_HCI_Message *rxMessage)
{
    uint16_t i = 0U;

    WiMOD_LoRaWAN_ShowResponse(" Get device eui ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        SerialDevice_Printf("Device EUI: \n\r");
        for (i = 1U; i < rxMessage->Length; ++i)
        {
            SerialDevice_Printf(" %02X", rxMessage->Payload[i]);
        }
        SerialDevice_Printf("\n\r");
    }
}

/**
 * @brief  Get support band response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_Support_Band_RSP(TWiMOD_HCI_Message *rxMessage)
{
    uint16_t i = 0U;

    WiMOD_LoRaWAN_ShowResponse("Get support band ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_OK)
    {
        for (i = 1U; i < rxMessage->Length; i += 2U)
        {
            SerialDevice_Printf("Band Idx: %d\n\r", rxMessage->Payload[i]);
            SerialDevice_Printf("Max. EIRP: %d dBm\n\r", rxMessage->Payload[1 + i]);
        }
    }
}

/**
 * @brief  Set radio stack response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Set_RSTACK_RSP(TWiMOD_HCI_Message *rxMessage)
{
    WiMOD_LoRaWAN_ShowResponse("Set Radio STACK ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    if (rxMessage->Payload[0U] == LORAWAN_STATUS_WRONG_PARAMETER)
    {
        if (rxMessage->Payload[1U] & 0x01U)
        {

            SerialDevice_Printf("Wrong Data Rate\n\r");
        }
        if (rxMessage->Payload[1U] & 0x02U)
        {
            SerialDevice_Printf("Wrong TX Power Level\n\r");
        }
        if (rxMessage->Payload[1U] & 0x20U)
        {
            SerialDevice_Printf("Wrong Band Index\n\r");
        }
    }
}

/**
 * @brief  Get radio stack response 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_Get_RSTACK_RSP(TWiMOD_HCI_Message *rxMessage)
{
    uint8_t str[8U] = { 0U };

    WiMOD_LoRaWAN_ShowResponse("Get Radio STACK ReSPonse",
            WiMOD_LoRaWAN_StatusStrings, rxMessage->Payload[0U]);

    SerialDevice_Printf("Data Rate: ");
    num2str(rxMessage->Payload[1U], str);
    SerialDevice_Printf((const char *)str);
    SerialDevice_Printf("\n\r");

    SerialDevice_Printf("TX Power Level (EIRP): ");
    num2str(rxMessage->Payload[2U], str);
    SerialDevice_Printf((const char *)str);
    SerialDevice_Printf("\n\r");

    SerialDevice_Printf("Adaptive DR: ");
    if (rxMessage->Payload[3U] & 0x01U)
    {
        SerialDevice_Printf("enable\n\r");
    }
    else
    {
        SerialDevice_Printf("disable\n\r");
    }
    SerialDevice_Printf("Duty Cycle: ");
    if (rxMessage->Payload[3U] & 0x02U)
    {
        SerialDevice_Printf("enable\n\r");
    }
    else
    {
        SerialDevice_Printf("disable\n\r");
    }
    SerialDevice_Printf("Class : ");
    if (rxMessage->Payload[3U] & 0x04U)
    {
        SerialDevice_Printf("C\n\r");
    }
    else
    {
        SerialDevice_Printf("A\n\r");
    }
    SerialDevice_Printf("RF format : ");
    if (rxMessage->Payload[3U] & 0x40U)
    {
        SerialDevice_Printf("extended\n\r");
    }
    else
    {
        SerialDevice_Printf("standard\n\r");
    }
    SerialDevice_Printf("MAC forwarding : ");
    if (rxMessage->Payload[3U] & 0x80U)
    {
        SerialDevice_Printf("enable\n\r");
    }
    else
    {
        SerialDevice_Printf("disable\n\r");
    }

    SerialDevice_Printf("Power Saving Mode: ");
    if (rxMessage->Payload[4U] & 0x01U)
    {
        SerialDevice_Printf("automatic\n\r");
    }
    else
    {
        SerialDevice_Printf("off\n\r");
    }

    SerialDevice_Printf("Retransmissions: ");
    num2str(rxMessage->Payload[5U], str);
    SerialDevice_Printf((const char *)str);
    SerialDevice_Printf("\n\r");

    SerialDevice_Printf("BandIdx: ");
    num2str(rxMessage->Payload[6U], str);
    SerialDevice_Printf((const char *)str);
    SerialDevice_Printf("\n\r");

    SerialDevice_Printf("MAC length: ");
    num2str(rxMessage->Payload[7U], str);
    SerialDevice_Printf((const char *)str);
    SerialDevice_Printf("\n\r");
}

/**
 * @brief  Show received MAC cmd 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_Process_MAC_CMDRxIndication(TWiMOD_HCI_Message *rxMessage)
{
    uint16_t i = 0U;
    uint16_t payloadSize = rxMessage->Length - 1U;
    uint8_t *rxInfo = NULL;

    /* RX channel info attached ? */
    if (rxMessage->Payload[0] & 0x01U)
    {
        payloadSize -= 5U;
    }

    if (payloadSize >= 0x01U)
    {
        SerialDevice_Printf("MAC rx event:\n\rmac-payload: 0x");
        for (i = 0U; i < payloadSize; i++)
        {
            SerialDevice_Printf((const char *)num2hex(rxMessage->Payload[1U + i], BYTE_F));
        }
        SerialDevice_Printf("\n\r");
    }

    if (rxMessage->Payload[0U] & 0x02U)
    {
        SerialDevice_Printf("Ack for uplink packet:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("Ack for uplink packet:no\n\r");
    }

    if (rxMessage->Payload[0U] & 0x04U)
    {
        SerialDevice_Printf("Frame pending:yes\n\r");
    }
    else
    {
        SerialDevice_Printf("Frame pending:no\n\r");
    }

    /* RX channel info attached ? */
    if (rxMessage->Payload[0U] & 0x01U)
    {
        rxInfo = &rxMessage->Payload[1U + payloadSize];
        SerialDevice_Printf("ChnIdx:%d, DR:%d, RSSI:%d, SNR:%d, RxSlot:%d\n\r",
                (uint8_t)rxInfo[0U],
                (uint8_t)rxInfo[1U],
                (uint8_t)rxInfo[2U],
                (uint8_t)rxInfo[3U],
                (uint8_t)rxInfo[4U]);
    }
}

/**
 * @brief  Show response status as human readable string 
 * @param
 * @retval None
 */
static void WiMOD_LoRaWAN_ShowResponse(const char *string,
        const TIDString *statusTable, uint8_t statusID)
{
    while (statusTable->String)
    {
        if (statusTable->ID == statusID)
        {
            SerialDevice_Printf(string);
            SerialDevice_Printf(" - Status(0x");
            SerialDevice_Printf((const char *)num2hex(statusID, BYTE_F));
            SerialDevice_Printf(") ");
            SerialDevice_Printf(statusTable->String);
            SerialDevice_Printf("\n\r");
            return;
        }

        statusTable++;
    }
}

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
