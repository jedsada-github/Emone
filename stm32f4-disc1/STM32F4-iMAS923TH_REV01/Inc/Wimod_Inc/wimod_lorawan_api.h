/**
 ******************************************************************************
 * @file           : wimod_lorawan_api.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIMOD_LORAWAN_API_H
#define __WIMOD_LORAWAN_API_H

/* Includes ------------------------------------------------------------------*/
#include "wimod_hci_layer.h"

/* Private define ------------------------------------------------------------*/
#define MAKEWORD(lo, hi)                        ((lo) | ((hi) << 8U))
#define MAKELONG(lo, hi)                        ((lo) | ((hi) << 16U))

/* Private HCI Message Structure ---------------------------------------------*/
/* Helper struct for ID -> string conversion ---------------------------------*/
typedef struct {
    int ID;
    const char *String;
} TIDString;

/* Endpoint (SAP) Identifier -------------------------------------------------*/
#define DEVMGMT_SAP_ID                          (0x01U)
#define LORAWAN_SAP_ID                          (0x10U)

/* Device Management SAP Message Identifier ----------------------------------*/
#define DEVMGMT_MSG_PING_REQ                    (0x01U)
#define DEVMGMT_MSG_PING_RSP                    (0x02U)

#define DEVMGMT_MSG_GET_DEVICE_INFO_REQ         (0x03U)
#define DEVMGMT_MSG_GET_DEVICE_INFO_RSP         (0x04U)

#define DEVMGMT_MSG_GET_FW_VERSION_REQ          (0x05U)
#define DEVMGMT_MSG_GET_FW_VERSION_RSP          (0x06U)

#define DEVMGMT_MSG_RESET_REQ                   (0x07U)
#define DEVMGMT_MSG_RESET_RSP                   (0x08U)

#define DEVMGMT_MSG_SET_OPMODE_REQ              (0x09U)
#define DEVMGMT_MSG_SET_OPMODE_RSP              (0x0AU)
#define DEVMGMT_MSG_GET_OPMODE_REQ              (0x0BU)
#define DEVMGMT_MSG_GET_OPMODE_RSP              (0x0CU)

#define DEVMGMT_MSG_SET_RTC_REQ                 (0x0DU)
#define DEVMGMT_MSG_SET_RTC_RSP                 (0x0EU)
#define DEVMGMT_MSG_GET_RTC_REQ                 (0x0FU)
#define DEVMGMT_MSG_GET_RTC_RSP                 (0x10U)

#define DEVMGMT_MSG_GET_DEVICE_STATUS_REQ       (0x17U)
#define DEVMGMT_MSG_GET_DEVICE_STATUS_RSP       (0x18U)

#define DEVMGMT_MSG_SET_RTC_ALARM_REQ           (0x31U)
#define DEVMGMT_MSG_SET_RTC_ALARM_RSP           (0x32U)
#define DEVMGMT_MSG_CLEAR_RTC_ALARM_REQ         (0x33U)
#define DEVMGMT_MSG_CLEAR_RTC_ALARM_RSP         (0x34U)
#define DEVMGMT_MSG_GET_RTC_ALARM_REQ           (0x35U)
#define DEVMGMT_MSG_GET_RTC_ALARM_RSP           (0x36U)
#define DEVMGMT_MSG_RTC_ALARM_IND               (0x38U)

/* Status Identifier ---------------------------------------------------------*/
#define DEVMGMT_STATUS_OK                                                      \
    (0x00U) /* Operation successful */
#define DEVMGMT_STATUS_ERROR                                                   \
    (0x01U) /* Operation failed */
#define DEVMGMT_STATUS_CMD_NOT_SUPPORTED                                       \
    (0x02U) /* Command is not supported */
#define DEVMGMT_STATUS_WRONG_PARAMETER                                         \
    (0x03U) /* HCI message contains wrong parameter */
#define DEVMGMT_STATUS_WRONG_DEVICE_MODE                                       \
    (0x04U) /* Stack is running in a wrong mode */

/* LoRaWAN SAP Message Identifier --------------------------------------------*/
#define LORAWAN_MSG_ACTIVATE_DEVICE_REQ         (0x01U)
#define LORAWAN_MSG_ACTIVATE_DEVICE_RSP         (0x02U)
#define LORAWAN_MSG_SET_JOIN_PARAM_REQ          (0x05U)
#define LORAWAN_MSG_SET_JOIN_PARAM_RSP          (0x06U)
                                                     
#define LORAWAN_MSG_JOIN_NETWORK_REQ            (0x09U)
#define LORAWAN_MSG_JOIN_NETWORK_RSP            (0x0AU)
#define LORAWAN_MSG_JOIN_TRANSMIT_IND           (0x0BU)
#define LORAWAN_MSG_JOIN_NETWORK_IND            (0x0CU)
                                                     
#define LORAWAN_MSG_SEND_UDATA_REQ              (0x0DU)
#define LORAWAN_MSG_SEND_UDATA_RSP              (0x0EU)
#define LORAWAN_MSG_SEND_UDATA_IND              (0x0FU)
#define LORAWAN_MSG_RECV_UDATA_IND              (0x10U)
                                                     
#define LORAWAN_MSG_SEND_CDATA_REQ              (0x11U)
#define LORAWAN_MSG_SEND_CDATA_RSP              (0x12U)
#define LORAWAN_MSG_SEND_CDATA_IND              (0x13U)
#define LORAWAN_MSG_RECV_CDATA_IND              (0x14U)
                                                     
#define LORAWAN_MSG_RECV_ACK_IND                (0x15U)
#define LORAWAN_MSG_RECV_NODATA_IND             (0x16U)
                                                     
#define LORAWAN_MSG_SET_RSTACK_CONFIG_REQ       (0x19U)
#define LORAWAN_MSG_SET_RSTACK_CONFIG_RSP       (0x1AU)
#define LORAWAN_MSG_GET_RSTACK_CONFIG_REQ       (0x1BU)
#define LORAWAN_MSG_GET_RSTACK_CONFIG_RSP       (0x1CU)
                                                     
#define LORAWAN_MSG_REACTIVATE_DEVICE_REQ       (0x1DU)
#define LORAWAN_MSG_REACTIVATE_DEVICE_RSP       (0x1EU)
                                                     
#define LORAWAN_MSG_DEACTIVATE_DEVICE_REQ       (0x21U)
#define LORAWAN_MSG_DEACTIVATE_DEVICE_RSP       (0x22U)

#define LORAWAN_MSG_FACTORY_RESET_REQ           (0x23U)
#define LORAWAN_MSG_FACTORY_RESET_RSP           (0x24U)

#define LORAWAN_MSG_SET_DEVICE_EUI_REQ          (0x25U)
#define LORAWAN_MSG_SET_DEVICE_EUI_RSP          (0x26U)
#define LORAWAN_MSG_GET_DEVICE_EUI_REQ          (0x27U)
#define LORAWAN_MSG_GET_DEVICE_EUI_RSP          (0x28U)
                                                     
#define LORAWAN_MSG_GET_NWK_STATUS_REQ          (0x29U)
#define LORAWAN_MSG_GET_NWK_STATUS_RSP          (0x2AU)

#define LORAWAN_MSG_SEND_MAC_CMD_REQ            (0x2BU)
#define LORAWAN_MSG_SEND_MAC_CMD_RSP            (0x2CU)
#define LORAWAN_MSG_RECV_MAC_CMD_IND            (0x2DU)

#define LORAWAN_MSG_SET_CUSTOM_CFG_REQ          (0x31U)
#define LORAWAN_MSG_SET_CUSTOM_CFG_RSP          (0x32U)
#define LORAWAN_MSG_GET_CUSTOM_CFG_REQ          (0x33U)
#define LORAWAN_MSG_GET_CUSTOM_CFG_RSP          (0x34U)

#define LORAWAN_MSG_GET_SUPPORTED_BANDS_REQ     (0x35U)
#define LORAWAN_MSG_GET_SUPPORTED_BANDS_RSP     (0x36U)

#define LORAWAN_MSG_SET_LINKADRREQ_CONFIG_REQ   (0x3BU)
#define LORAWAN_MSG_SET_LINKADRREQ_CONFIG_RSP   (0x3CU)
#define LORAWAN_MSG_GET_LINKADRREQ_CONFIG_REQ   (0x3DU)
#define LORAWAN_MSG_GET_LINKADRREQ_CONFIG_RSP   (0x3EU)

/* Status Identifier ---------------------------------------------------------*/
#define LORAWAN_STATUS_OK                                                      \
    (0x00U) /* Operation successful */
#define LORAWAN_STATUS_ERROR                                                   \
    (0x01U) /* Operation failed */
#define LORAWAN_STATUS_CMD_NOT_SUPPORTED                                       \
    (0x02U) /* Command is not supported */
#define LORAWAN_STATUS_WRONG_PARAMETER                                         \
    (0x03U) /* HCI message contains wrong parameter */
#define LORAWAN_STATUS_WRONG_DEVICE_MODE                                       \
    (0x04U) /* Stack is running in a wrong mode */
#define LORAWAN_STATUS_DEVICE_NOT_ACTIVATED                                    \
    (0x05U) /* Device is not activated */
#define LORAWAN_STATUS_DEVICE_BUSY                                             \
    (0x06U) /* Device is busy, command rejected */
#define LORAWAN_STATUS_QUEUE_FULL                                              \
    (0x07U) /* Message queue is full, command rejected */
#define LORAWAN_STATUS_LENGTH_ERROR                                            \
    (0x08U) /* HCI message length is invalid or radio 
               payload size is too large */
#define LORAWAN_STATUS_NO_FACTORY_SETTINGS                                     \
    (0x09U) /* Factory Settings EEPROM block missing */
#define LORAWAN_STATUS_CHANNEL_BLOCKED_BY_DC                                   \
    (0x0AU) /* Channel blocked by Duty Cycle */
#define LORAWAN_STATUS_CHANNEL_NOT_AVAILABLE                                   \
    (0x0BU) /* No channel available (e.g. no channel 
               defined for the configured spreading factor) */

/* Function Prototypes -------------------------------------------------------*/
uint8_t WiMOD_LoRaWAN_Init (UART_HandleTypeDef *comPort);
int32_t WiMOD_DevMgmt_Msg_Req (uint8_t msg_id, uint8_t *val, uint16_t len);
int32_t WiMOD_LoRaWAN_Msg_Req (uint8_t msg_id, uint8_t *val, uint16_t len);
int32_t WiMOD_LoRaWAN_GetCustomConfig (void);
int32_t WiMOD_LoRaWAN_SetCustomConfig (int8_t val);
int32_t WiMOD_LoRaWAN_SetOPMODE (uint8_t val);
int32_t WiMOD_LoRaWAN_GetOPMODE (void);
int32_t WiMOD_LoRaWAN_ClearRTCAlarm (void);
int32_t WiMOD_LoRaWAN_GetRTCAlarm (void);
int32_t WiMOD_LoRaWAN_SetRTCAlarm (uint8_t *alarm);
int32_t WiMOD_LoRaWAN_GetRTC (void);
int32_t WiMOD_LoRaWAN_SetRTC (uint32_t *time);
int32_t WiMOD_LoRaWAN_SendPing (void);
int32_t WiMOD_LoRaWAN_GetDeviceInfo (void);
int32_t WiMOD_LoRaWAN_GetFirmwareVersion (void);
int32_t WiMOD_LoRaWAN_Reset (void);
int32_t WiMOD_LoRaWAN_GetDeviceStatus (void);
int32_t WiMOD_LoRaWAN_GetNkwStatus (void);
int32_t WiMOD_LoRaWAN_FactoryReset (void);
int32_t WiMOD_LoRaWAN_SetJoinParameters(uint8_t *payload);
int32_t WiMOD_LoRaWAN_JoinNetworkRequest (void);
int32_t WiMOD_LoRaWAN_SendURadioData (uint8_t port, uint8_t *data, int length);
int32_t WiMOD_LoRaWAN_SendCRadioData (uint8_t port, uint8_t *data, int length);
int32_t WiMOD_LoRaWAN_GetLinkADR (void);
int32_t WiMOD_LoRaWAN_SetLinkADR (uint8_t *payload);
int32_t WiMOD_LoRaWAN_SetMAC_CMD (uint8_t *payload);
void WiMOD_LoRaWAN_Process (void);

#endif /* __WIMOD_LORAWAN_API_H */

/************************ (C) COPYRIGHT www.emone.co.th *******END OF FILE****/
