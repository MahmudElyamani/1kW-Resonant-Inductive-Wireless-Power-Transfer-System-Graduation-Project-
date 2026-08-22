#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* TIM1 clock = APB2 clock = 72MHz. With PSC=0, ARR=845 the update event
 * (and therefore the PWM output) frequency is:
 *      f = 72,000,000 / (ARR + 1) = 72,000,000 / 846 = 85,106 Hz  (~85kHz)
 * Adjust TIM1_PWM_ARR below if you need a different exact frequency. */
#define TIM1_PWM_ARR            845U

/**
 * @brief Initialise TIM1 to generate 4 independent 85kHz PWM signals on
 *        CH1..CH4 (PA8..PA11), with the break function (BDTR) armed so
 *        the outputs are force-disabled by hardware the instant PB12
 *        (BKIN) goes low or a software break is requested - no CPU
 *        intervention required to guarantee outputs go safe.
 */
void TIM1_PWM_Init(void);

/**
 * @brief Update the duty cycle of one PWM channel.
 * @param channel   1..4
 * @param duty_pct  0..100
 */
void TIM1_PWM_SetDuty(uint8_t channel, uint8_t duty_pct);

/**
 * @brief Immediately force all 4 PWM outputs to their inactive state
 *        via the timer's software break-generate bit. Safe to call
 *        from an ISR.
 */
void TIM1_PWM_EmergencyStop(void);

/**
 * @brief Re-arm the main output enable after a fault has been cleared.
 *        Only call this once the monitoring layer confirms the fault
 *        condition is gone.
 */
void TIM1_PWM_Resume(void);

/* Incremented once per timer update event (~85,000 times/sec). Used by
 * the protection layer as a lightweight "the timer is alive" heartbeat. */
extern volatile uint32_t g_tim1_update_count;

/* Set by TIM1_BRK_IRQHandler when a hardware/software break occurred. */
extern volatile uint8_t g_tim1_break_flag;

#endif /* TIMER_H */
