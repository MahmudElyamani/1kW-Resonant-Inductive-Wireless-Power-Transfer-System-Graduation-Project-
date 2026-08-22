## STM32F103C6 Code, There's a separate folder for Atmega328 (Arduino) firmware testing for prototype

![ArduinoFirmware](/codesinglephase)

## PWM generation

TIM1 clock = APB2 clock = 72MHz (PSC = 0). `ARR = 845` gives
`72,000,000 / 846 ≈ 85.1kHz`. All 4 channels share the same period
register but have independent `CCRx` duty registers, so 4 synchronized
85kHz PWM signals with independent duty cycles come out of a *single*
timer — this is exactly what TIM1's 4 compare channels are for.

- PWM frequency: change `TIM1_PWM_ARR` in `inc/timer.h`.
