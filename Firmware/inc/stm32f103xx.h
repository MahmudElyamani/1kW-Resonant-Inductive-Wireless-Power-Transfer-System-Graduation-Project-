/**
 ******************************************************************************
 * @file    stm32f103xx.h
 * @brief   Minimal hand-written register map for STM32F103C6T6 (Low-Density).
 *          No CMSIS, no StdPeriph, no HAL. Only the peripherals used by this
 *          project are declared. Values taken from RM0008 (STM32F10xxx
 *          reference manual) and the STM32F103x4/x6 datasheet.
 ******************************************************************************
 */

#ifndef STM32F103XX_H
#define STM32F103XX_H

#include <stdint.h>

/* ---------------------------------------------------------------------- */
/*  Core / NVIC                                                            */
/* ---------------------------------------------------------------------- */
#define NVIC_ISER0      (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ICER0      (*(volatile uint32_t *)0xE000E180UL)
#define NVIC_IPR_BASE   ((volatile uint8_t  *)0xE000E400UL)

/* Cortex-M3 System Control Block (used for NMI/fault reporting only) */
#define SCB_ICSR        (*(volatile uint32_t *)0xE000ED04UL)

/* IRQ numbers as defined in the STM32F103 vector table (see startup file) */
typedef enum
{
    ADC1_2_IRQn        = 18,
    TIM1_BRK_IRQn      = 24,
    TIM1_UP_IRQn       = 25,
    TIM1_TRG_COM_IRQn  = 26,
    TIM1_CC_IRQn       = 27
} IRQn_Type;

static inline void NVIC_EnableIRQ(IRQn_Type irq)
{
    NVIC_ISER0 = (1UL << ((uint32_t)irq & 0x1FUL));
}

static inline void NVIC_DisableIRQ(IRQn_Type irq)
{
    NVIC_ICER0 = (1UL << ((uint32_t)irq & 0x1FUL));
}

static inline void NVIC_SetPriority(IRQn_Type irq, uint8_t prio)
{
    /* STM32F10x implements 4 priority bits -> shift into top nibble */
    NVIC_IPR_BASE[(uint32_t)irq] = (uint8_t)((prio << 4) & 0xF0U);
}

/* ---------------------------------------------------------------------- */
/*  Peripheral base addresses                                              */
/* ---------------------------------------------------------------------- */
#define FLASH_R_BASE    0x40022000UL
#define RCC_BASE        0x40021000UL
#define AFIO_BASE       0x40010000UL
#define GPIOA_BASE      0x40010800UL
#define GPIOB_BASE      0x40010C00UL
#define GPIOC_BASE      0x40011000UL
#define ADC1_BASE       0x40012400UL
#define TIM1_BASE       0x40012C00UL
#define IWDG_BASE       0x40003000UL

/* ---------------------------------------------------------------------- */
/*  FLASH interface (only ACR used, for wait-state / latency config)       */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t AR;
    volatile uint32_t RESERVED;
    volatile uint32_t OBR;
    volatile uint32_t WRPR;
} FLASH_TypeDef;
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)

#define FLASH_ACR_LATENCY_2WS   0x02UL
#define FLASH_ACR_PRFTBE        (1UL << 4)

/* ---------------------------------------------------------------------- */
/*  RCC - Reset and Clock Control                                          */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;
#define RCC     ((RCC_TypeDef *)RCC_BASE)

#define RCC_CR_HSEON            (1UL << 16)
#define RCC_CR_HSERDY           (1UL << 17)
#define RCC_CR_CSSON            (1UL << 19)
#define RCC_CR_PLLON            (1UL << 24)
#define RCC_CR_PLLRDY           (1UL << 25)

#define RCC_CFGR_SW_PLL         0x02UL
#define RCC_CFGR_SWS_Msk        0x0CUL
#define RCC_CFGR_SWS_PLL        0x08UL
#define RCC_CFGR_HPRE_DIV1      (0x0UL << 4)
#define RCC_CFGR_PPRE1_DIV2     (0x4UL << 8)
#define RCC_CFGR_PPRE2_DIV1     (0x0UL << 11)
#define RCC_CFGR_ADCPRE_DIV6    (0x2UL << 14)
#define RCC_CFGR_PLLSRC_HSE     (1UL << 16)
#define RCC_CFGR_PLLXTPRE_DIV1  (0UL << 17)
#define RCC_CFGR_PLLMUL_9       (0x7UL << 18)

#define RCC_APB2ENR_AFIOEN      (1UL << 0)
#define RCC_APB2ENR_IOPAEN      (1UL << 2)
#define RCC_APB2ENR_IOPBEN      (1UL << 3)
#define RCC_APB2ENR_IOPCEN      (1UL << 4)
#define RCC_APB2ENR_ADC1EN      (1UL << 9)
#define RCC_APB2ENR_TIM1EN      (1UL << 11)

#define RCC_CIR_CSSF             (1UL << 7)   /* clock security flag       */
#define RCC_CIR_CSSC             (1UL << 23)  /* clock security clear bit  */

/* ---------------------------------------------------------------------- */
/*  GPIO                                                                    */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)

typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    volatile uint32_t RESERVED;
    volatile uint32_t MAPR2;
} AFIO_TypeDef;
#define AFIO    ((AFIO_TypeDef *)AFIO_BASE)

/* ---------------------------------------------------------------------- */
/*  TIM1 - Advanced-control timer                                          */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM1_TypeDef;
#define TIM1    ((TIM1_TypeDef *)TIM1_BASE)

#define TIM_CR1_CEN         (1UL << 0)
#define TIM_CR1_ARPE        (1UL << 7)

#define TIM_DIER_UIE        (1UL << 0)
#define TIM_DIER_BIE        (1UL << 7)

#define TIM_SR_UIF          (1UL << 0)
#define TIM_SR_BIF          (1UL << 7)

#define TIM_EGR_UG          (1UL << 0)
#define TIM_EGR_BG          (1UL << 7)  /* software-forced break event */

#define TIM_CCMR1_OC1M_PWM1 (0x6UL << 4)
#define TIM_CCMR1_OC1PE     (1UL << 3)
#define TIM_CCMR1_OC2M_PWM1 (0x6UL << 12)
#define TIM_CCMR1_OC2PE     (1UL << 11)

#define TIM_CCMR2_OC3M_PWM1 (0x6UL << 4)
#define TIM_CCMR2_OC3PE     (1UL << 3)
#define TIM_CCMR2_OC4M_PWM1 (0x6UL << 12)
#define TIM_CCMR2_OC4PE     (1UL << 11)

#define TIM_CCER_CC1E       (1UL << 0)
#define TIM_CCER_CC2E       (1UL << 4)
#define TIM_CCER_CC3E       (1UL << 8)
#define TIM_CCER_CC4E       (1UL << 12)

#define TIM_BDTR_LOCK_1     (0x1UL << 8)
#define TIM_BDTR_OSSI       (1UL << 10)
#define TIM_BDTR_BKE        (1UL << 12)
#define TIM_BDTR_BKP        (1UL << 13)
#define TIM_BDTR_AOE        (1UL << 14)
#define TIM_BDTR_MOE        (1UL << 15)

/* ---------------------------------------------------------------------- */
/*  ADC1                                                                    */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_TypeDef;
#define ADC1    ((ADC_TypeDef *)ADC1_BASE)

#define ADC_SR_AWD          (1UL << 0)
#define ADC_SR_EOC          (1UL << 1)

#define ADC_CR1_AWDCH_Pos   0
#define ADC_CR1_AWDEN       (1UL << 23)
#define ADC_CR1_AWDIE       (1UL << 6)
#define ADC_CR1_AWDSGL      (1UL << 9)

#define ADC_CR2_ADON        (1UL << 0)
#define ADC_CR2_CAL         (1UL << 2)
#define ADC_CR2_CONT        (1UL << 1)
#define ADC_CR2_EXTSEL_SWSTART (0x7UL << 17)
#define ADC_CR2_EXTTRIG     (1UL << 20)
#define ADC_CR2_SWSTART     (1UL << 22)

/* ---------------------------------------------------------------------- */
/*  IWDG - Independent watchdog                                            */
/* ---------------------------------------------------------------------- */
typedef struct
{
    volatile uint32_t KR;
    volatile uint32_t PR;
    volatile uint32_t RLR;
    volatile uint32_t SR;
} IWDG_TypeDef;
#define IWDG    ((IWDG_TypeDef *)IWDG_BASE)

#define IWDG_KR_RELOAD      0xAAAAUL
#define IWDG_KR_UNLOCK      0x5555UL
#define IWDG_KR_START       0xCCCCUL

#endif /* STM32F103XX_H */
