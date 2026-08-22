/**
 ******************************************************************************
 * @file    startup_stm32f103c6.s
 * @brief   Cortex-M3 vector table and reset handler for STM32F103C6T6.
 *          GNU assembler syntax (arm-none-eabi-as / gcc).
 ******************************************************************************
 */

    .syntax unified
    .cpu cortex-m3
    .thumb

/* Symbols provided by the linker script (see linker/stm32f103c6.ld) */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss
.word _estack

/* ------------------------------------------------------------------------ */
/*  Reset handler: copy .data from FLASH to RAM, zero .bss, call C startup  */
/* ------------------------------------------------------------------------ */
    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr   sp, =_estack

    /* Copy initialized data from FLASH (_sidata) into RAM (.data) */
    ldr   r0, =_sdata
    ldr   r1, =_edata
    ldr   r2, =_sidata
    movs  r3, #0
    b     CopyDataLoopCheck
CopyDataLoop:
    ldr   r4, [r2, r3]
    str   r4, [r0, r3]
    adds  r3, r3, #4
CopyDataLoopCheck:
    adds  r4, r0, r3
    cmp   r4, r1
    bcc   CopyDataLoop

    /* Zero-fill .bss */
    ldr   r0, =_sbss
    ldr   r1, =_ebss
    movs  r2, #0
    b     ZeroBssLoopCheck
ZeroBssLoop:
    str   r2, [r0]
    adds  r0, r0, #4
ZeroBssLoopCheck:
    cmp   r0, r1
    bcc   ZeroBssLoop

    bl    Clock_Init_Placeholder   /* no-op, real clock setup runs in main() */
    bl    main

InfiniteLoop:
    b     InfiniteLoop
    .size Reset_Handler, .-Reset_Handler

    .thumb_func
    .weak Clock_Init_Placeholder
    .type Clock_Init_Placeholder, %function
Clock_Init_Placeholder:
    bx lr

/* ------------------------------------------------------------------------ */
/*  Default handler: infinite loop, used as the weak fallback for every     */
/*  interrupt that this project does not explicitly implement.              */
/* ------------------------------------------------------------------------ */
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
    b Default_Handler
    .size Default_Handler, .-Default_Handler

/* ------------------------------------------------------------------------ */
/*  Weak aliases: any handler NOT defined in interrupt.c falls back to      */
/*  Default_Handler. TIM1_BRK_IRQHandler, TIM1_UP_IRQHandler,               */
/*  ADC1_2_IRQHandler, NMI_Handler and HardFault_Handler are overridden by  */
/*  the strong definitions in src/interrupt.c.                              */
/* ------------------------------------------------------------------------ */
    .macro WEAK_HANDLER name
    .weak \name
    .thumb_set \name, Default_Handler
    .endm

    WEAK_HANDLER NMI_Handler
    WEAK_HANDLER HardFault_Handler
    WEAK_HANDLER MemManage_Handler
    WEAK_HANDLER BusFault_Handler
    WEAK_HANDLER UsageFault_Handler
    WEAK_HANDLER SVC_Handler
    WEAK_HANDLER DebugMon_Handler
    WEAK_HANDLER PendSV_Handler
    WEAK_HANDLER SysTick_Handler

    WEAK_HANDLER WWDG_IRQHandler
    WEAK_HANDLER PVD_IRQHandler
    WEAK_HANDLER TAMPER_IRQHandler
    WEAK_HANDLER RTC_IRQHandler
    WEAK_HANDLER FLASH_IRQHandler
    WEAK_HANDLER RCC_IRQHandler
    WEAK_HANDLER EXTI0_IRQHandler
    WEAK_HANDLER EXTI1_IRQHandler
    WEAK_HANDLER EXTI2_IRQHandler
    WEAK_HANDLER EXTI3_IRQHandler
    WEAK_HANDLER EXTI4_IRQHandler
    WEAK_HANDLER DMA1_Channel1_IRQHandler
    WEAK_HANDLER DMA1_Channel2_IRQHandler
    WEAK_HANDLER DMA1_Channel3_IRQHandler
    WEAK_HANDLER DMA1_Channel4_IRQHandler
    WEAK_HANDLER DMA1_Channel5_IRQHandler
    WEAK_HANDLER DMA1_Channel6_IRQHandler
    WEAK_HANDLER DMA1_Channel7_IRQHandler
    WEAK_HANDLER ADC1_2_IRQHandler
    WEAK_HANDLER USB_HP_CAN1_TX_IRQHandler
    WEAK_HANDLER USB_LP_CAN1_RX0_IRQHandler
    WEAK_HANDLER CAN1_RX1_IRQHandler
    WEAK_HANDLER CAN1_SCE_IRQHandler
    WEAK_HANDLER EXTI9_5_IRQHandler
    WEAK_HANDLER TIM1_BRK_IRQHandler
    WEAK_HANDLER TIM1_UP_IRQHandler
    WEAK_HANDLER TIM1_TRG_COM_IRQHandler
    WEAK_HANDLER TIM1_CC_IRQHandler
    WEAK_HANDLER TIM2_IRQHandler
    WEAK_HANDLER TIM3_IRQHandler
    WEAK_HANDLER TIM4_IRQHandler
    WEAK_HANDLER I2C1_EV_IRQHandler
    WEAK_HANDLER I2C1_ER_IRQHandler
    WEAK_HANDLER I2C2_EV_IRQHandler
    WEAK_HANDLER I2C2_ER_IRQHandler
    WEAK_HANDLER SPI1_IRQHandler
    WEAK_HANDLER SPI2_IRQHandler
    WEAK_HANDLER USART1_IRQHandler
    WEAK_HANDLER USART2_IRQHandler
    WEAK_HANDLER USART3_IRQHandler
    WEAK_HANDLER EXTI15_10_IRQHandler
    WEAK_HANDLER RTCAlarm_IRQHandler
    WEAK_HANDLER USBWakeUp_IRQHandler

/* ------------------------------------------------------------------------ */
/*  Vector table                                                            */
/* ------------------------------------------------------------------------ */
    .section .isr_vector,"a",%progbits
    .type g_pfnVectorTable, %object
g_pfnVectorTable:
    .word _estack                       /* 0  Initial stack pointer        */
    .word Reset_Handler                 /* 1  Reset                        */
    .word NMI_Handler                   /* 2  NMI (also: CSS clock fault)  */
    .word HardFault_Handler             /* 3  Hard fault                   */
    .word MemManage_Handler             /* 4  MPU fault                    */
    .word BusFault_Handler              /* 5  Bus fault                    */
    .word UsageFault_Handler            /* 6  Usage fault                  */
    .word 0                             /* 7  Reserved                     */
    .word 0                             /* 8  Reserved                     */
    .word 0                             /* 9  Reserved                     */
    .word 0                             /* 10 Reserved                     */
    .word SVC_Handler                   /* 11 SVCall                       */
    .word DebugMon_Handler              /* 12 Debug monitor                */
    .word 0                             /* 13 Reserved                     */
    .word PendSV_Handler                /* 14 PendSV                       */
    .word SysTick_Handler               /* 15 SysTick                      */
    /* ---- STM32-specific interrupts ---- */
    .word WWDG_IRQHandler                /* 0:  Window watchdog            */
    .word PVD_IRQHandler                 /* 1:  PVD through EXTI           */
    .word TAMPER_IRQHandler              /* 2:  Tamper                     */
    .word RTC_IRQHandler                 /* 3:  RTC                        */
    .word FLASH_IRQHandler               /* 4:  Flash                      */
    .word RCC_IRQHandler                 /* 5:  RCC                        */
    .word EXTI0_IRQHandler               /* 6:  EXTI Line0                 */
    .word EXTI1_IRQHandler               /* 7:  EXTI Line1                 */
    .word EXTI2_IRQHandler               /* 8:  EXTI Line2                 */
    .word EXTI3_IRQHandler               /* 9:  EXTI Line3                 */
    .word EXTI4_IRQHandler               /* 10: EXTI Line4                 */
    .word DMA1_Channel1_IRQHandler       /* 11                             */
    .word DMA1_Channel2_IRQHandler       /* 12                             */
    .word DMA1_Channel3_IRQHandler       /* 13                             */
    .word DMA1_Channel4_IRQHandler       /* 14                             */
    .word DMA1_Channel5_IRQHandler       /* 15                             */
    .word DMA1_Channel6_IRQHandler       /* 16                             */
    .word DMA1_Channel7_IRQHandler       /* 17                             */
    .word ADC1_2_IRQHandler              /* 18: ADC1 & ADC2 (AWD trip)     */
    .word USB_HP_CAN1_TX_IRQHandler      /* 19                             */
    .word USB_LP_CAN1_RX0_IRQHandler     /* 20                             */
    .word CAN1_RX1_IRQHandler            /* 21                             */
    .word CAN1_SCE_IRQHandler            /* 22                             */
    .word EXTI9_5_IRQHandler             /* 23                             */
    .word TIM1_BRK_IRQHandler            /* 24: TIM1 Break (fault input)   */
    .word TIM1_UP_IRQHandler             /* 25: TIM1 Update                */
    .word TIM1_TRG_COM_IRQHandler        /* 26                             */
    .word TIM1_CC_IRQHandler             /* 27                             */
    .word TIM2_IRQHandler                /* 28                             */
    .word TIM3_IRQHandler                /* 29                             */
    .word TIM4_IRQHandler                /* 30                             */
    .word I2C1_EV_IRQHandler             /* 31                             */
    .word I2C1_ER_IRQHandler             /* 32                             */
    .word I2C2_EV_IRQHandler             /* 33                             */
    .word I2C2_ER_IRQHandler             /* 34                             */
    .word SPI1_IRQHandler                /* 35                             */
    .word SPI2_IRQHandler                /* 36                             */
    .word USART1_IRQHandler              /* 37                             */
    .word USART2_IRQHandler              /* 38                             */
    .word USART3_IRQHandler              /* 39                             */
    .word EXTI15_10_IRQHandler           /* 40                             */
    .word RTCAlarm_IRQHandler            /* 41                             */
    .word USBWakeUp_IRQHandler           /* 42                             */

    .size g_pfnVectorTable, .-g_pfnVectorTable
