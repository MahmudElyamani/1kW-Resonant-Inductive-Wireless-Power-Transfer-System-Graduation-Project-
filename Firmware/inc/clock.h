#ifndef CLOCK_H
#define CLOCK_H

/* System core clock after SystemClock_Config() runs, in Hz.
 * HSE(8MHz) x PLL(9) = 72MHz SYSCLK/HCLK, APB2 = 72MHz, APB1 = 36MHz. */
#define SYSTEM_CORE_CLOCK_HZ   72000000UL
#define APB2_CLOCK_HZ          72000000UL

/**
 * @brief Configure HSE + PLL for 72MHz SYSCLK and enable the Clock
 *        Security System (CSS) so an HSE failure is caught in hardware
 *        and reported through the NMI handler instead of silently
 *        running the timers off a drifting/failed clock.
 * @retval 0 on success, -1 if HSE or PLL failed to lock (fallback to
 *         HSI @8MHz is left active in that case so the MCU stays alive).
 */
int Clock_Init(void);

#endif /* CLOCK_H */
