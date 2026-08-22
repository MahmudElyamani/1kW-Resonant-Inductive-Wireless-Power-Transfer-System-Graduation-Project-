#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

/* System-wide fault flags, set from ISR context and consumed by the
 * main loop / protection layer. Kept as plain volatile flags (not a
 * single bitfield struct) so each source can be set/cleared
 * independently without a read-modify-write race between ISRs. */
extern volatile uint8_t g_fault_break_pin;   /* external BKIN fault      */
extern volatile uint8_t g_fault_overcurrent; /* ADC analog watchdog trip */
extern volatile uint8_t g_fault_clock;       /* HSE failure (CSS/NMI)    */

/**
 * @brief True if any latched fault is active. PWM must stay disabled
 *        (or be actively re-disabled) while this is true.
 */
uint8_t Fault_IsActive(void);

/** @brief Clear all latched fault flags after the hazard has been
 *         physically resolved. Does NOT re-enable the timer outputs by
 *         itself; call TIM1_PWM_Resume() explicitly afterward so
 *         resuming is always a deliberate, observable action. */
void Fault_ClearAll(void);

#endif /* INTERRUPT_H */
