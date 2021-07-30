/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define USART_STATUS_REGISTER   SR /* HAL USART status register name adapter */
#define USART_DATA_REGISTER     DR /* HAL UART data register name adapter */
#define USART2_TX_BUFF_MAX      (256U)
#define USART2_Baudrate_115200  (115200U)
#define USART3_Baudrate_115200  (115200U)

/* Private enum --------------------------------------------------------------*/
/**
 * @brief The hexdecimal format to be used
 */
typedef enum
{
    HALFBYTE_F,  /* 1 hex digit */
    BYTE_F,      /* 2 hex digits */
    WORD_F,      /* 4 hex digits */
    DOUBLEWORD_F /* 8 hex digits */
} eHexFormat;

/* Exported global variables -------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USART2 init function ------------------------------------------------------*/
void MX_USART2_UART_Init(void);

/* USART3 init function ------------------------------------------------------*/
void MX_USART3_UART_Init(void);

/* USART printf function -----------------------------------------------------*/
void UART_Printf(const char* fmt, ...);

void USART_ShowMenu(void);
void USART_TxWelcomeMessage(void);
void USART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *data);
void USART_Transmit(UART_HandleTypeDef *huart, const char *TextString);
void USART_ITCharManager(UART_HandleTypeDef *huart);
FlagStatus str2num(uint8_t *str, uint32_t *pnum);
void num2str(uint32_t nbr, uint8_t *str);
uint8_t *num2hex(uint32_t num, eHexFormat HexFormat);

#ifdef __cplusplus
}
#endif
#endif /* __USART_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
