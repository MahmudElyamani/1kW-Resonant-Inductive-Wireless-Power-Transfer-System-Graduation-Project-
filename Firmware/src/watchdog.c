/**
 ******************************************************************************
 * @file    watchdog.c
 * @brief   IWDG configuration: ~100ms timeout, LSI-clocked, independent of
 *          the main clock tree and immune to a stuck main loop.
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "watchdog.h"

/* LSI ~= 40kHz. Prescaler /32 -> 1.25kHz tick. RLR=124 -> (124+1)/1250 = 100ms */
#define IWDG_PRESCALER_DIV32    0x03U
#define IWDG_RELOAD_100MS       124U

void Watchdog_Init(void)
{
    IWDG->KR  = IWDG_KR_UNLOCK;         
    IWDG->PR  = IWDG_PRESCALER_DIV32;
    IWDG->RLR = IWDG_RELOAD_100MS;
    IWDG->KR  = IWDG_KR_RELOAD;         
    IWDG->KR  = IWDG_KR_START;         
}

void Watchdog_Refresh(void)
{
    IWDG->KR = IWDG_KR_RELOAD;
}
