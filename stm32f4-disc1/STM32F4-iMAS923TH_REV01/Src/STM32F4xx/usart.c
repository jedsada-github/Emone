/**
 ******************************************************************************
 * File Name          : USART.c
 * Description        : This file provides code for the configuration
 *                      of the USART instances.
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether 
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2018 STMicroelectronics International N.V. 
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other 
 *    contributors to this software may be used to endorse or promote products 
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this 
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under 
 *    this license is void and will automatically terminate your rights under 
 *    this license. 
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{
  huart2.Instance          = USART2;
  huart2.Init.BaudRate     = USART2_Baudrate_115200;
  huart2.Init.WordLength   = UART_WORDLENGTH_8B;
  huart2.Init.StopBits     = UART_STOPBITS_1;
  huart2.Init.Parity       = UART_PARITY_NONE;
  huart2.Init.Mode         = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART3_UART_Init(void)
{
    huart3.Instance          = USART3;
    huart3.Init.BaudRate     = USART3_Baudrate_115200;
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;
    huart3.Init.StopBits     = UART_STOPBITS_1;
    huart3.Init.Parity       = UART_PARITY_NONE;
    huart3.Init.Mode         = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief UART_Printf Function
 * @param None
 * @retval None
 */
void UART_Printf(const char* fmt, ...)
{
    char buff[USART2_TX_BUFF_MAX];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
    va_end(args);
}

/**
 * @brief  show main menu
 * @param  
 * @retval none
 */
void USART_ShowMenu(void)
{
    USART_Transmit(&huart2, "\n\r");
    USART_Transmit(&huart2, "------------------------------\n\r");
    USART_Transmit(&huart2, "[SPACE] : show this menu\n\r");
    USART_Transmit(&huart2, "[o]     : get OPMODE\n\r");
    USART_Transmit(&huart2, "[m]     : set OPMODE\n\r");
    USART_Transmit(&huart2, "[n]     : clear OPMODE\n\r");
    USART_Transmit(&huart2, "[E]     : get DEV EUI\n\r");
    USART_Transmit(&huart2, "[e]     : set DEV EUI (0x70B3D58FF101787C)\n\r");
    USART_Transmit(&huart2, "[A]     : activate ABP\n\r");
    USART_Transmit(&huart2, "[R]     : reactivate\n\r");
    USART_Transmit(&huart2, "[D]     : deactivate\n\r");
    USART_Transmit(&huart2, "[T]     : activate OTA\n\r");
    USART_Transmit(&huart2, "[j]     : join network request\n\r");
    USART_Transmit(&huart2, "[N]     : get network status\n\r");
    USART_Transmit(&huart2, "[K]     : get Radio Stack Configuration\n\r");
    USART_Transmit(&huart2, "[S]     : set Radio Stack Configuration\n\r");
    USART_Transmit(&huart2, "[Q]     : get link ADR\n\r");
    USART_Transmit(&huart2, "[W]     : set link ADR\n\r");
    USART_Transmit(&huart2, "[G]     : get support band\n\r");
    USART_Transmit(&huart2, "[C]     : get custom configuration\n\r");
    USART_Transmit(&huart2, "[O]     : set custom configuration\n\r");
    USART_Transmit(&huart2, "[t]     : get RTC\n\r");
    USART_Transmit(&huart2, "[*]     : set RTC\n\r");
    USART_Transmit(&huart2, "[a]     : get RTC alarm\n\r");
    USART_Transmit(&huart2, "[+]     : set RTC alarm\n\r");
    USART_Transmit(&huart2, "[-]     : clear RTC alarm\n\r");
    USART_Transmit(&huart2, "[r]     : reset device\n\r");
    USART_Transmit(&huart2, "[p]     : ping device\n\r");
    USART_Transmit(&huart2, "[f]     : get firmware version\n\r");
    USART_Transmit(&huart2, "[i]     : get device information\n\r");
    USART_Transmit(&huart2, "[s]     : get device status\n\r");
    USART_Transmit(&huart2, "[u]     : send unconfirmed radio message port 21 data 067104D2FB2E\n\r");
    USART_Transmit(&huart2, "[c]     : send confirmed radio message port 23 data 6701106700FF\n\r");
    USART_Transmit(&huart2, "[Z]     : factory reset\n\r\n\r");
    /* printf("[e|x]   : exit program\n\r"); */
    USART_Transmit(&huart2, "-> enter command: ");
}

/**
 * @brief  Transmit the initial message to the PC which is connected to the
 *         STM32 board via UART.
 * @param  
 * @retval none
 */
void USART_TxWelcomeMessage(void)
{
    /* Send information to PC via USART */
    UART_Printf("\n\r");
    UART_Printf(" LoRaWAN-iMAS923TH (iM980A) Demo\n\r");
    UART_Printf(" -------------------------------------------\n\r");
    UART_Printf(" iMAS923TH Expansion Board for STM32F4-DISC1\n\r\n\r");
    UART_Printf(" Copyright 2018 @ EmOne; The MIT License (MIT)\n\r\n\r");
    UART_Printf(" Welcome to eMOD HCL example (WiMOD compatible)!!!\n\r");
}

/**
 * @brief  Send string data via UART
 * @param  
 * @retval none
 */
void USART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *TextString)
{
    uint8_t TextStringLength = 0U;

    /* Calculate the length of the text string to be sent */
    while (TextString[TextStringLength++] != '\0');
    TextStringLength--;

    /* Use the HAL function to send the text string via USART */
    HAL_UART_Transmit(huart, TextString, TextStringLength, HAL_MAX_DELAY);
}

/**
 * @brief  Send a text string via USART.
 * @param  huart       pointer to a UART_HandleTypeDef structure that contains
 *                     the configuration information for the specified UART module.
 * @param  TextString  The text string to be sent.
 * @note It use the HAL_UART_Transmit function.
 *
 * @retval none
 */
void USART_Transmit(UART_HandleTypeDef *huart, const char *data)
{ 
    USART_Transmit_Data(huart, (uint8_t *)data); 
}

/**
 * @brief  Handle text character just received.
 * @param  huart pointer to a UART_HandleTypeDef structure that contains
 *               the configuration information for the specified UART module.
 * @note To use inside USARx_IRQHandler function.
 *
 * @retval none
 */
void USART_ITCharManager(UART_HandleTypeDef *huart)
{
    uint8_t UART_Receive_IT_Char;

    UART_Receive_IT_Char = (uint8_t)(huart->Instance->USART_DATA_REGISTER);
    /* Checks the buffer full or return carriage  */
    if ((huart->RxXferCount == 1U) || (UART_Receive_IT_Char == '\r'))
    {
        huart->RxXferCount += 1U;
        huart->pRxBuffPtr -= 1U;
        *(huart->pRxBuffPtr) = '\0';

        USART_Transmit(huart, "\n\r");
        while (HAL_IS_BIT_SET(huart->Instance->USART_STATUS_REGISTER, UART_FLAG_RXNE))
        {
        }

        __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

        /* Check if a transmit process is ongoing or not */
        if (huart->gState == HAL_UART_STATE_BUSY_TX_RX)
        {
            huart->gState = HAL_UART_STATE_BUSY_TX;
        }
        else
        {
            /* Disable the UART Parity Error Interrupt */
            __HAL_UART_DISABLE_IT(huart, UART_IT_PE);

            /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
            __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

            huart->gState = HAL_UART_STATE_READY;
        }
    }
}

/**
 * @brief  This function converts a text string into a number.
 * @param  str       The pointer to the text string to convert.
 * @param  pnum      The pointer to the numerical variable.
 * @retval FlagStatus  SET or RESET related to the conversion.
 *
 * @retval enum 
 */
FlagStatus str2num(uint8_t *str, uint32_t *pnum)
{
    uint8_t  TxtStr_digit = 0U;
    uint8_t  digit        = 0U;
    uint32_t tenpwr       = 0U;
    uint32_t number       = 0U;

    while (*(str + digit) != '\0')
    {
        if (((*(str + digit) >= '0') && (*(str + digit) <= '9')))
        {
            digit++;
        }
        else
        {
            *pnum = 0U;
            return RESET;
        }
    }

    tenpwr = 1U;
    number = 0U;

    do
    {
        TxtStr_digit = (*(str + (--digit)));
        number += ((TxtStr_digit - '0') * tenpwr);
        tenpwr *= 10U;
    } while (digit);

    *pnum = number;
    return SET;
}

/**
 * @brief  Convert a number nbr into a string str with 7 characters.
 * @param  nbr The number to be converted.
 * @param  str The container of the converted number into a text in decimal
 *         format.
 * @note   The decimal digits of the number must be maximum 7 so str has to be
 *         able to store at least 7 characters plus '\0'.
 *
 * @retval none
 */
void num2str(uint32_t nbr, uint8_t *str)
{
    uint8_t i = 0U;
    uint32_t divisor = 0U;
    uint8_t *pstrbuff;

    pstrbuff = str;

    /* Reset the text string */
    for (i = 0U; i < 7U; i++)
    {
        *(pstrbuff + i) = '\0';
    }

    divisor = 1000000U;

    if (nbr) /* If nbr is different from zero then it is processed  */
    {
        while (!(nbr / divisor))
        {
            divisor /= 10U;
        }

        while (divisor >= 10U)
        {
            i = nbr / divisor;
            *pstrbuff++ = '0' + i;
            nbr = nbr - (i * divisor);
            divisor /= 10U;
        }
    }

    *pstrbuff++ = '0' + nbr;
    *pstrbuff++ = '\0';
}

/**
 * @brief  Convert an integer number into hexadecimal format.
 *
 * @param  num         The integer number to convert.
 * @param  HexFormat   The output format about hexadecimal number.
 *
 * @retval uint8_t*    The address of the string text for the converted hexadecimal number.
 */
uint8_t *num2hex(uint32_t num, eHexFormat HexFormat)
{
    static uint8_t HexValue[9U];
    uint8_t i         = 0U;
    uint8_t dummy     = 0U;
    uint8_t HexDigits = 0U;

    switch (HexFormat)
    {
    case HALFBYTE_F:
        HexDigits = 1U;
        break;
    case BYTE_F:
        HexDigits = 2U;
        break;
    case WORD_F:
        HexDigits = 4U;
        break;
    case DOUBLEWORD_F:
        HexDigits = 8U;
        break;
    default:
        HexDigits = 2U;
        break;
    }

    for (i = 0U; i < HexDigits; i++)
    {
        HexValue[i] = '\0';
        dummy = (num & (0x0F << (((HexDigits - 1U) - i) * 4U))) >> (((HexDigits - 1U) - i) * 4U);
        if (dummy < 0x0AU)
        {
            HexValue[i] = dummy + '0';
        }
        else
        {
            HexValue[i] = (dummy - 0x0AU) + 'A';
        }
    }
    HexValue[i] = '\0';

    return HexValue;
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
