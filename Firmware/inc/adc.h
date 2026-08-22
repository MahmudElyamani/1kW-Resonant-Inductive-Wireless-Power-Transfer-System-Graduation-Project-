#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Analog watchdog thresholds (12-bit ADC counts, 0..4095).
 * Tune these to your actual current/voltage sense divider/shunt+amp gain.
 * Example given assumes a sense signal centered mid-scale under normal
 * load, tripping if it swings outside a safe band. */
#define ADC_AWD_LOW_THRESHOLD    200U
#define ADC_AWD_HIGH_THRESHOLD   3800U
#define ADC_SENSE_CHANNEL        5U   /* PA5 = ADC1_IN5 */

/**
 * @brief Initialise ADC1 on the sense channel, run self-calibration,
 *        and arm the analog watchdog so an out-of-range reading raises
 *        ADC1_2_IRQn in hardware without needing to poll in software.
 */
void ADC_Init(void);

/** @brief Latest converted sense value (updated on every EOC). */
uint16_t ADC_GetLastSample(void);

#endif /* ADC_H */
