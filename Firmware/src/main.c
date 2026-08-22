/**
 ******************************************************************************
 * @file    main.c
 * @brief   
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "clock.h"
#include "gpio.h"
#include "timer.h"
#include "adc.h"
#include "watchdog.h"
#include "interrupt.h"


static uint32_t s_last_seen_tick;

static void Protection_Poll(void)
{

    static uint32_t stall_polls = 0U;

    if (g_tim1_update_count == s_last_seen_tick)
    {
        stall_polls++;
        if (stall_polls > 3U)
        {
            TIM1_PWM_EmergencyStop();
            g_fault_clock = 1U; 
            GPIO_FaultLedOn();
        }
    }
    else
    {
        stall_polls = 0U;
    }
    s_last_seen_tick = g_tim1_update_count;
}

int main(void)
{

    (void)Clock_Init();

    GPIO_Init();
    TIM1_PWM_Init();
    ADC_Init();
    Watchdog_Init();


    TIM1_PWM_SetDuty(1U, 25U);
    TIM1_PWM_SetDuty(2U, 50U);
    TIM1_PWM_SetDuty(3U, 75U);
    TIM1_PWM_SetDuty(4U, 90U);

    s_last_seen_tick = g_tim1_update_count;

    for (;;)
    {
        Protection_Poll();

        if (!Fault_IsActive())
        {

            Watchdog_Refresh();
        }

        
    }
}
