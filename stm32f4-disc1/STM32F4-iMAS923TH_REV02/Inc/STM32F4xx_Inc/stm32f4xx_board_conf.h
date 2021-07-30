/**
 ******************************************************************************
 * @file           : stm32f4xx_board_conf.h
 * @brief          : Header for stm32f4xx_board_conf.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_BOARD_CONF_H
#define __STM32F4XX_BOARD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions prototypes ---------------------------------------------*/
void STM32F4xx_Device_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#endif /* USE_FULL_ASSERT */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

#endif /* __STM32F4XX_BOARD_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
