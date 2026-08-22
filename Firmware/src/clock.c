/**
 ******************************************************************************
 * @file    clock.c
 * @brief   HSE/PLL clock tree setup for STM32F103C6 @72MHz + Clock Security
 *          System (CSS) enable. All register-level, no library.
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "clock.h"

#define HSE_STARTUP_TIMEOUT   0x0000FFFFUL
#define PLL_LOCK_TIMEOUT      0x0000FFFFUL

int Clock_Init(void)
{
    volatile uint32_t timeout;


    RCC->CR |= RCC_CR_HSEON;
    timeout = HSE_STARTUP_TIMEOUT;
    while (!(RCC->CR & RCC_CR_HSERDY))
    {
        if (--timeout == 0U)
        {
            
            return -1;
        }
    }


    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2WS;


    RCC->CFGR = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 |
                RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_ADCPRE_DIV6;


    RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_DIV1 |
                 RCC_CFGR_PLLMUL_9;


    RCC->CR |= RCC_CR_PLLON;
    timeout = PLL_LOCK_TIMEOUT;
    while (!(RCC->CR & RCC_CR_PLLRDY))
    {
        if (--timeout == 0U)
        {
            return -1; 
        }
    }

    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SWS_Msk) | RCC_CFGR_SW_PLL;
    timeout = PLL_LOCK_TIMEOUT;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
    {
        if (--timeout == 0U)
        {
            return -1;
        }
    }

    RCC->CIR |= RCC_CIR_CSSC;   /* clear any stale CSS flag first  */
    RCC->CR  |= RCC_CR_CSSON;

    return 0;
}
