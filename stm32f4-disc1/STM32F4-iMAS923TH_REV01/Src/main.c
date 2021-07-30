/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

uint8_t ABP_ACTIVATE_PAYLOAD[] = {
        /* Device address (LSB): E01BD038 */
        0x38U, 0xD0U, 0x1BU, 0xE0U,
        /* Network key (MSB): 00112233445566778899AABBCCDDEEFF */
        0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U,
        0x88U, 0x99U, 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU,
        /* App key (MSB) : FFEEDDCCBBAA99887766554433221100 */
        0xFFU, 0xEEU, 0xDDU, 0xCCU, 0xBBU, 0xAAU, 0x99U, 0x88U,
        0x77U, 0x66U, 0x55U, 0x44U, 0x33U, 0x22U, 0x11U, 0x00U
};

uint8_t OTA_ACTIVATE_PAYLOAD[] = {
        /* App EUI (MSB): AABBCCDDEEFF1122 */
        0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU, 0x11U, 0x22U,
        /* App key (MSB) */
        0xFFU, 0xEEU, 0xDDU, 0xCCU, 0xBBU, 0xAAU, 0x99U, 0x88U,
        0x77U, 0x66U, 0x55U, 0x44U, 0x33U, 0x22U, 0x11U, 0x00U
}; 

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* STM32F4 Board Configuration -----------------------------------------------*/
    STM32F4xx_Device_Init();
    
    /* Infinite loop */
    while (1U)
    {
        /* Handle receiver process */
        WiMOD_LoRaWAN_Process();

        /* Serial Device read command */
        SerialDevice_CheckAppCmd();
    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
