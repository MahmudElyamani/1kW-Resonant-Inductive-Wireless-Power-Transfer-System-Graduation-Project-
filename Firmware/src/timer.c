/**
 ******************************************************************************
 * @file    timer.c
 * @brief   TIM1 configured as a 4-channel, 85kHz independent PWM generator
 *          (PA8..PA11) with the advanced-timer break function used as a
 *          hardware safety net: any fault on BKIN (PB12) or a software
 *          "break generate" instantly forces all 4 outputs to their
 *          inactive/idle state without any CPU/ISR latency involved,
 *          exactly the feature TIM1 exists for.
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "timer.h"

volatile uint32_t g_tim1_update_count = 0U;
volatile uint8_t  g_tim1_break_flag   = 0U;

void TIM1_PWM_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    TIM1->CR1 = 0U;
    TIM1->PSC = 0U;                 
    TIM1->ARR = TIM1_PWM_ARR;       
    TIM1->RCR = 0U;                 

    TIM1->CCR1 = (TIM1_PWM_ARR + 1U) / 2U;
    TIM1->CCR2 = (TIM1_PWM_ARR + 1U) / 2U;
    TIM1->CCR3 = (TIM1_PWM_ARR + 1U) / 2U;
    TIM1->CCR4 = (TIM1_PWM_ARR + 1U) / 2U;
    TIM1->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE |
                  TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;


    TIM1->CCMR2 = TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE |
                  TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;


    TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E |
                 TIM_CCER_CC3E | TIM_CCER_CC4E;

    
    TIM1->BDTR = TIM_BDTR_MOE | TIM_BDTR_BKE | TIM_BDTR_OSSI |
                 TIM_BDTR_LOCK_1;

    TIM1->CR1  = TIM_CR1_ARPE;     
    TIM1->DIER = TIM_DIER_UIE | TIM_DIER_BIE;
    TIM1->EGR  = TIM_EGR_UG;        

    NVIC_SetPriority(TIM1_BRK_IRQn, 0U); 
    NVIC_SetPriority(TIM1_UP_IRQn,  2U);
    NVIC_EnableIRQ(TIM1_BRK_IRQn);
    NVIC_EnableIRQ(TIM1_UP_IRQn);

    TIM1->CR1 |= TIM_CR1_CEN;      
}

void TIM1_PWM_SetDuty(uint8_t channel, uint8_t duty_pct)
{
    uint32_t ccr;

    if (duty_pct > 100U)
    {
        duty_pct = 100U;
    }

    ccr = ((uint32_t)(TIM1_PWM_ARR + 1U) * duty_pct) / 100U;

    switch (channel)
    {
        case 1U: TIM1->CCR1 = ccr; break;
        case 2U: TIM1->CCR2 = ccr; break;
        case 3U: TIM1->CCR3 = ccr; break;
        case 4U: TIM1->CCR4 = ccr; break;
        default: break; 
    }
}

void TIM1_PWM_EmergencyStop(void)
{
    TIM1->EGR = TIM_EGR_BG;
}

void TIM1_PWM_Resume(void)
{
    TIM1->SR &= ~TIM_SR_BIF;
    TIM1->BDTR |= TIM_BDTR_MOE;
}
