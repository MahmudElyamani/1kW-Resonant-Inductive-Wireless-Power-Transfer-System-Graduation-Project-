#ifndef GPIO_H
#define GPIO_H

/*
 * Pin map used by this project
 * -----------------------------------------------------------------------
 *  PA8  (TIM1_CH1)  -> PWM output 1     (85 kHz)
 *  PA9  (TIM1_CH2)  -> PWM output 2     (85 kHz)
 *  PA10 (TIM1_CH3)  -> PWM output 3     (85 kHz)
 *  PA11 (TIM1_CH4)  -> PWM output 4     (85 kHz)
 *  PB12 (TIM1_BKIN) -> external fault/break input (active LOW, pulled up)
 *  PA5  (ADC1_IN5)  -> analog current/voltage sense feedback
 *  PC13             -> onboard fault/status LED (active LOW on Blue Pill)
 * -----------------------------------------------------------------------
 */

void GPIO_Init(void);

/* Fault LED helpers, used by the protection layer */
void GPIO_FaultLedOn(void);
void GPIO_FaultLedOff(void);

#endif /* GPIO_H */
