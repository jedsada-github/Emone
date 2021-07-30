/**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */

/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */


#include "stm32f4xx.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)25000000U) /*!< Default value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM as data memory  */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)\
 || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F40xxx || STM32F41xxx || STM32F42xxx || STM32F43xxx || STM32F469xx || STM32F479xx ||\
          STM32F412Zx || STM32F412Vx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
/* #define DATA_IN_ExtSDRAM */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx ||\
          STM32F479xx */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00U /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
uint32_t SystemCoreClock = 16000000U;
const uint8_t AHBPrescTable[16U] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U };
const uint8_t APBPrescTable[8U]  = { 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U };
/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_FunctionPrototypes
  * @{
  */

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  static void SystemInit_ExtMemCtl(void);
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3UL << 10UL * 2UL)|(3UL << 11UL * 2UL));  /* set CP10 and CP11 Full Access */
  #endif

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (its value
  *              depends on the application requirements), user has to ensure that HSE_VALUE
  *              is same as the real frequency of the crystal used. Otherwise, this function
  *              may have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0U, pllvco = 0U, pllp = 2U, pllsource = 0U, pllm = 2U;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00U:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04U:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08U:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22U;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

      if (pllsource != 0U)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16U) + 1U) * 2U;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4U)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}

#if defined (DATA_IN_ExtSRAM) && defined (DATA_IN_ExtSDRAM)
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx)
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  __IO uint32_t tmp = 0x00U;

  register uint32_t tmpreg = 0U, timeout = 0xFFFFU;
  register __IO uint32_t index;

  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface clock */
  RCC->AHB1ENR |= 0x000001F8U;

  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0U]  = 0x00CCC0CCU;
  GPIOD->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PDx pins in Alternate function mode */
  GPIOD->MODER   = 0xAAAA0A8AU;
  /* Configure PDx pins speed to 100 MHz */
  GPIOD->OSPEEDR = 0xFFFF0FCFU;
  /* Configure PDx pins Output type to push-pull */
  GPIOD->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PDx pins */
  GPIOD->PUPDR   = 0x00000000U;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0U]  = 0xC00CC0CCU;
  GPIOE->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PEx pins in Alternate function mode */
  GPIOE->MODER   = 0xAAAA828AU;
  /* Configure PEx pins speed to 100 MHz */
  GPIOE->OSPEEDR = 0xFFFFC3CFU;
  /* Configure PEx pins Output type to push-pull */
  GPIOE->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PEx pins */
  GPIOE->PUPDR   = 0x00000000U;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0U]  = 0xCCCCCCCCU;
  GPIOF->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PFx pins in Alternate function mode */
  GPIOF->MODER   = 0xAA800AAAU;
  /* Configure PFx pins speed to 50 MHz */
  GPIOF->OSPEEDR = 0xAA800AAAU;
  /* Configure PFx pins Output type to push-pull */
  GPIOF->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PFx pins */
  GPIOF->PUPDR   = 0x00000000U;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0U]  = 0xCCCCCCCCU;
  GPIOG->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PGx pins in Alternate function mode */
  GPIOG->MODER   = 0xAAAAAAAAU;
  /* Configure PGx pins speed to 50 MHz */
  GPIOG->OSPEEDR = 0xAAAAAAAAU;
  /* Configure PGx pins Output type to push-pull */
  GPIOG->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PGx pins */
  GPIOG->PUPDR   = 0x00000000U;

  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0U]  = 0x00C0CC00U;
  GPIOH->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PHx pins in Alternate function mode */
  GPIOH->MODER   = 0xAAAA08A0U;
  /* Configure PHx pins speed to 50 MHz */
  GPIOH->OSPEEDR = 0xAAAA08A0U;
  /* Configure PHx pins Output type to push-pull */
  GPIOH->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PHx pins */
  GPIOH->PUPDR   = 0x00000000U;

  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0U]  = 0xCCCCCCCCU;
  GPIOI->AFR[1U]  = 0x00000CC0U;
  /* Configure PIx pins in Alternate function mode */
  GPIOI->MODER   = 0x0028AAAAU;
  /* Configure PIx pins speed to 50 MHz */
  GPIOI->OSPEEDR = 0x0028AAAAU;
  /* Configure PIx pins Output type to push-pull */
  GPIOI->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PIx pins */
  GPIOI->PUPDR   = 0x00000000U;

/*-- FMC Configuration -------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001U;
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);

  FMC_Bank5_6->SDCR[0U] = 0x000019E4U;
  FMC_Bank5_6->SDTR[0U] = 0x01115351U;

  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011U;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Delay */
  for (index = 0U; index < 1000U; index++);

  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012U;
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Auto refresh command */
  FMC_Bank5_6->SDCMR = 0x00000073U;
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* MRD register program */
  FMC_Bank5_6->SDCMR = 0x00046014U;
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027CU << 1U));

  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0U];
  FMC_Bank5_6->SDCR[0U] = (tmpreg & 0xFFFFFDFFU);

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2U]  = 0x00001011U;
  FMC_Bank1->BTCR[3U]  = 0x00000201U;
  FMC_Bank1E->BWTR[2U] = 0x0FFFFFFFU;
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */
#if defined(STM32F469xx) || defined(STM32F479xx)
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2U]  = 0x00001091U;
  FMC_Bank1->BTCR[3U]  = 0x00110212U;
  FMC_Bank1E->BWTR[2U] = 0x0FFFFFFFU;
#endif /* STM32F469xx || STM32F479xx */

  (void)(tmp);
}
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */
#elif defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  __IO uint32_t tmp = 0x00U;
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
#if defined (DATA_IN_ExtSDRAM)
  register uint32_t tmpreg = 0U, timeout = 0xFFFFU;
  register __IO uint32_t index;

#if defined(STM32F446xx)
  /* Enable GPIOA, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG interface
      clock */
  RCC->AHB1ENR |= 0x0000007DU;
#else
  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface
      clock */
  RCC->AHB1ENR |= 0x000001F8U;
#endif /* STM32F446xx */
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

#if defined(STM32F446xx)
  /* Connect PAx pins to FMC Alternate function */
  GPIOA->AFR[0U]  |= 0xC0000000U;
  GPIOA->AFR[1U]  |= 0x00000000U;
  /* Configure PDx pins in Alternate function mode */
  GPIOA->MODER   |= 0x00008000U;
  /* Configure PDx pins speed to 50 MHz */
  GPIOA->OSPEEDR |= 0x00008000U;
  /* Configure PDx pins Output type to push-pull */
  GPIOA->OTYPER  |= 0x00000000U;
  /* No pull-up, pull-down for PDx pins */
  GPIOA->PUPDR   |= 0x00000000U;

  /* Connect PCx pins to FMC Alternate function */
  GPIOC->AFR[0U]  |= 0x00CC0000U;
  GPIOC->AFR[1U]  |= 0x00000000U;
  /* Configure PDx pins in Alternate function mode */
  GPIOC->MODER   |= 0x00000A00U;
  /* Configure PDx pins speed to 50 MHz */
  GPIOC->OSPEEDR |= 0x00000A00U;
  /* Configure PDx pins Output type to push-pull */
  GPIOC->OTYPER  |= 0x00000000U;
  /* No pull-up, pull-down for PDx pins */
  GPIOC->PUPDR   |= 0x00000000U;
#endif /* STM32F446xx */

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0U]  = 0x000000CCU;
  GPIOD->AFR[1U]  = 0xCC000CCCU;
  /* Configure PDx pins in Alternate function mode */
  GPIOD->MODER   = 0xA02A000AU;
  /* Configure PDx pins speed to 50 MHz */
  GPIOD->OSPEEDR = 0xA02A000AU;
  /* Configure PDx pins Output type to push-pull */
  GPIOD->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PDx pins */
  GPIOD->PUPDR   = 0x00000000U;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0U]  = 0xC00000CCU;
  GPIOE->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PEx pins in Alternate function mode */
  GPIOE->MODER   = 0xAAAA800AU;
  /* Configure PEx pins speed to 50 MHz */
  GPIOE->OSPEEDR = 0xAAAA800AU;
  /* Configure PEx pins Output type to push-pull */
  GPIOE->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PEx pins */
  GPIOE->PUPDR   = 0x00000000U;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0U]  = 0xCCCCCCCCU;
  GPIOF->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PFx pins in Alternate function mode */
  GPIOF->MODER   = 0xAA800AAAU;
  /* Configure PFx pins speed to 50 MHz */
  GPIOF->OSPEEDR = 0xAA800AAAU;
  /* Configure PFx pins Output type to push-pull */
  GPIOF->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PFx pins */
  GPIOF->PUPDR   = 0x00000000U;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0U]  = 0xCCCCCCCCU;
  GPIOG->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PGx pins in Alternate function mode */
  GPIOG->MODER   = 0xAAAAAAAAU;
  /* Configure PGx pins speed to 50 MHz */
  GPIOG->OSPEEDR = 0xAAAAAAAAU;
  /* Configure PGx pins Output type to push-pull */
  GPIOG->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PGx pins */
  GPIOG->PUPDR   = 0x00000000U;

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx)
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0U]  = 0x00C0CC00U;
  GPIOH->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PHx pins in Alternate function mode */
  GPIOH->MODER   = 0xAAAA08A0U;
  /* Configure PHx pins speed to 50 MHz */
  GPIOH->OSPEEDR = 0xAAAA08A0U;
  /* Configure PHx pins Output type to push-pull */
  GPIOH->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PHx pins */
  GPIOH->PUPDR   = 0x00000000U;

  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0U]  = 0xCCCCCCCCU;
  GPIOI->AFR[1U]  = 0x00000CC0U;
  /* Configure PIx pins in Alternate function mode */
  GPIOI->MODER   = 0x0028AAAAU;
  /* Configure PIx pins speed to 50 MHz */
  GPIOI->OSPEEDR = 0x0028AAAAU;
  /* Configure PIx pins Output type to push-pull */
  GPIOI->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PIx pins */
  GPIOI->PUPDR   = 0x00000000U;
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */

/*-- FMC Configuration -------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001U;
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);

  /* Configure and enable SDRAM bank1 */
#if defined(STM32F446xx)
  FMC_Bank5_6->SDCR[0U] = 0x00001954U;
#else
  FMC_Bank5_6->SDCR[0U] = 0x000019E4U;
#endif /* STM32F446xx */
  FMC_Bank5_6->SDTR[0U] = 0x01115351U;

  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011U;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Delay */
  for (index = 0U; index < 1000U; index++);

  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012U;
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Auto refresh command */
#if defined(STM32F446xx)
  FMC_Bank5_6->SDCMR = 0x000000F3U;
#else
  FMC_Bank5_6->SDCMR = 0x00000073U;
#endif /* STM32F446xx */
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* MRD register program */
#if defined(STM32F446xx)
  FMC_Bank5_6->SDCMR = 0x00044014U;
#else
  FMC_Bank5_6->SDCMR = 0x00046014U;
#endif /* STM32F446xx */
  timeout = 0xFFFFU;
  while((tmpreg != 0U) && (timeout-- > 0U))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020U;
  }

  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;
#if defined(STM32F446xx)
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000050CU << 1U));
#else
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027CU << 1U));
#endif /* STM32F446xx */

  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0U];
  FMC_Bank5_6->SDCR[0U] = (tmpreg & 0xFFFFFDFFU);
#endif /* DATA_IN_ExtSDRAM */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)\
 || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)

#if defined(DATA_IN_ExtSRAM)
/*-- GPIOs Configuration -----------------------------------------------------*/
   /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
  RCC->AHB1ENR   |= 0x00000078U;
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0U]  = 0x00CCC0CCU;
  GPIOD->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PDx pins in Alternate function mode */
  GPIOD->MODER   = 0xAAAA0A8AU;
  /* Configure PDx pins speed to 100 MHz */
  GPIOD->OSPEEDR = 0xFFFF0FCFU;
  /* Configure PDx pins Output type to push-pull */
  GPIOD->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PDx pins */
  GPIOD->PUPDR   = 0x00000000U;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0U]  = 0xC00CC0CCU;
  GPIOE->AFR[1U]  = 0xCCCCCCCCU;
  /* Configure PEx pins in Alternate function mode */
  GPIOE->MODER   = 0xAAAA828AU;
  /* Configure PEx pins speed to 100 MHz */
  GPIOE->OSPEEDR = 0xFFFFC3CFU;
  /* Configure PEx pins Output type to push-pull */
  GPIOE->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PEx pins */
  GPIOE->PUPDR   = 0x00000000U;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0U]  = 0x00CCCCCCU;
  GPIOF->AFR[1U]  = 0xCCCC0000U;
  /* Configure PFx pins in Alternate function mode */
  GPIOF->MODER   = 0xAA000AAAU;
  /* Configure PFx pins speed to 100 MHz */
  GPIOF->OSPEEDR = 0xFF000FFFU;
  /* Configure PFx pins Output type to push-pull */
  GPIOF->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PFx pins */
  GPIOF->PUPDR   = 0x00000000U;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0U]  = 0x00CCCCCCU;
  GPIOG->AFR[1U]  = 0x000000C0U;
  /* Configure PGx pins in Alternate function mode */
  GPIOG->MODER   = 0x00085AAAU;
  /* Configure PGx pins speed to 100 MHz */
  GPIOG->OSPEEDR = 0x000CAFFFU;
  /* Configure PGx pins Output type to push-pull */
  GPIOG->OTYPER  = 0x00000000U;
  /* No pull-up, pull-down for PGx pins */
  GPIOG->PUPDR   = 0x00000000U;

/*-- FMC/FSMC Configuration --------------------------------------------------*/
  /* Enable the FMC/FSMC interface clock */
  RCC->AHB3ENR         |= 0x00000001U;

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2U]  = 0x00001011U;
  FMC_Bank1->BTCR[3U]  = 0x00000201U;
  FMC_Bank1E->BWTR[2U] = 0x0fffffffU;
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */
#if defined(STM32F469xx) || defined(STM32F479xx)
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2U]  = 0x00001091U;
  FMC_Bank1->BTCR[3U]  = 0x00110212U;
  FMC_Bank1E->BWTR[2U] = 0x0fffffffU;
#endif /* STM32F469xx || STM32F479xx */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx)\
   || defined(STM32F412Zx) || defined(STM32F412Vx)
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FSMCEN);
  /* Configure and enable Bank1_SRAM2 */
  FSMC_Bank1->BTCR[2U]  = 0x00001011U;
  FSMC_Bank1->BTCR[3U]  = 0x00000201U;
  FSMC_Bank1E->BWTR[2U] = 0x0FFFFFFFU;
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F412Zx || STM32F412Vx */

#endif /* DATA_IN_ExtSRAM */
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||\
          STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx  */
  (void)(tmp);
}
#endif /* DATA_IN_ExtSRAM && DATA_IN_ExtSDRAM */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
