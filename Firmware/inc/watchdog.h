#ifndef WATCHDOG_H
#define WATCHDOG_H

/**
 * @brief Configure the Independent Watchdog (IWDG). It runs off the
 *        internal ~40kHz LSI, independent from the main clock tree, so
 *        it keeps working even if HSE/PLL have failed. Timeout is set
 *        to approximately 100ms: if the main loop stops refreshing it
 *        (firmware hang, stuck ISR, etc.) the MCU is forced to reset
 *        rather than keep driving 4 unmonitored PWM outputs forever.
 */
void Watchdog_Init(void);

/** @brief Feed the watchdog. Call periodically from a healthy main loop. */
void Watchdog_Refresh(void);

#endif /* WATCHDOG_H */
