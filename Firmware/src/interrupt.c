/**
 ******************************************************************************
 * @file    interrupt.c
 * @brief   
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "interrupt.h"
#include "timer.h"
#include "adc.h"
#include "gpio.h"

volatile uint8_t g_fault_break_pin   = 0U;
volatile uint8_t g_fault_overcurrent = 0U;
volatile uint8_t g_fault_clock       = 0U;

uint8_t Fault_IsActive(void)
{
    return (uint8_t)(g_fault_break_pin || g_fault_overcurrent || g_fault_clock);
}

void Fault_ClearAll(void)
{
    g_fault_break_pin   = 0U;
    g_fault_overcurrent = 0U;
    g_fault_clock       = 0U;
    g_tim1_break_flag   = 0U;
    GPIO_FaultLedOff();
}

/* ---------------------------------------------------------------------- */
/*  TIM1 break: fires when BKIN (PB12) goes low OR a software break was   */
/*  requested. The timer has ALREADY forced the outputs off in hardware   */
/*  by the time this ISR runs - this handler only has to record why.      */
/* ---------------------------------------------------------------------- */
void TIM1_BRK_IRQHandler(void)
{
    if (TIM1->SR & TIM_SR_BIF)
    {
        TIM1->SR &= ~TIM_SR_BIF;
        g_fault_break_pin = 1U;
        GPIO_FaultLedOn();
    }
}

/* ---------------------------------------------------------------------- */
/*  TIM1 update: fires every PWM period (~85kHz). Used purely as a        */
/*  lightweight liveness counter for the protection layer - it does NOT   */
/*  do any heavy work here, an 85kHz ISR budget is tiny.                  */
/* ---------------------------------------------------------------------- */
void TIM1_UP_IRQHandler(void)
{
    if (TIM1->SR & TIM_SR_UIF)
    {
        TIM1->SR &= ~TIM_SR_UIF;
        g_tim1_update_count++;
    }
}

/* ---------------------------------------------------------------------- */
/*  ADC1/2: analog watchdog trip (over-current / over-voltage on the      */
/*  sense channel) or a normal end-of-conversion.                         */
/* ---------------------------------------------------------------------- */
void ADC1_2_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_AWD)
    {
        ADC1->SR &= ~ADC_SR_AWD;
        g_fault_overcurrent = 1U;
        GPIO_FaultLedOn();
        TIM1_PWM_EmergencyStop();  /* belt-and-braces: also force via SW */
    }

    if (ADC1->SR & ADC_SR_EOC)
    {
        ADC_OnEndOfConversion();   /* reads DR, which also clears EOC   */
    }
}

/* ---------------------------------------------------------------------- */
/*  NMI: raised by the Clock Security System if HSE dies while running.   */
/*  Hardware has already auto-switched SYSCLK to HSI; we just need to     */
/*  make sure the (now mistimed) PWM outputs are disabled.                */
/* ---------------------------------------------------------------------- */
void NMI_Handler(void)
{
    if (RCC->CIR & RCC_CIR_CSSF)
    {
        RCC->CIR |= RCC_CIR_CSSC;  /* clear the CSS flag              */
        g_fault_clock = 1U;
        GPIO_FaultLedOn();
        TIM1_PWM_EmergencyStop();
    }
}

/* ---------------------------------------------------------------------- */
/*  HardFault: last line of defence. We cannot know why we got here, so   */
/*  the only safe move is to kill the PWM outputs and halt instead of     */
/*  letting a corrupted CPU state keep driving external hardware.         */
/* ---------------------------------------------------------------------- */
void HardFault_Handler(void)
{
    TIM1_PWM_EmergencyStop();
    GPIO_FaultLedOn();
    for (;;)
    {

    }
}
