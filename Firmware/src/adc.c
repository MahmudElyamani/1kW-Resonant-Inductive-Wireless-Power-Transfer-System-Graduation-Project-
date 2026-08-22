/**
 ******************************************************************************
 * @file    adc.c
 * @brief   ADC1 continuous conversion on the sense input (PA5/IN5) with the
 *          analog watchdog armed as a hardware-level over-current /
 *          over-voltage detector. On trip, ADC1_2_IRQHandler (interrupt.c)
 *          calls TIM1_PWM_EmergencyStop() directly from the ISR so the
 *          reaction time does not depend on the main loop's schedule.
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "adc.h"

static volatile uint16_t s_last_sample = 0U;

static void adc_delay(volatile uint32_t cycles)
{
    while (cycles--)
    {
        __asm__ volatile("nop");
    }
}

void ADC_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->SMPR2 = (0x3UL << (ADC_SENSE_CHANNEL * 3U));

    ADC1->SQR1 = 0U;
    ADC1->SQR3 = ADC_SENSE_CHANNEL;

    ADC1->HTR = ADC_AWD_HIGH_THRESHOLD;
    ADC1->LTR = ADC_AWD_LOW_THRESHOLD;
    ADC1->CR1 = ADC_CR1_AWDEN | ADC_CR1_AWDIE | ADC_CR1_AWDSGL |
                (ADC_SENSE_CHANNEL << ADC_CR1_AWDCH_Pos);

    NVIC_SetPriority(ADC1_2_IRQn, 1U);
    NVIC_EnableIRQ(ADC1_2_IRQn);

    /* Power up, calibrate (mandatory ST-recommended sequence) */
    ADC1->CR2 = ADC_CR2_ADON;
    adc_delay(1000U);

    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
    {
        /* wait for calibration to complete */
    }

    ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL_SWSTART;
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t ADC_GetLastSample(void)
{
    return s_last_sample;
}


void ADC_OnEndOfConversion(void)
{
    s_last_sample = (uint16_t)ADC1->DR;
}
