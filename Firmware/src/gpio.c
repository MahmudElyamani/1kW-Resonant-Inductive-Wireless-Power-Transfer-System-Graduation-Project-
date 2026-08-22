/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   GPIO configuration for the 4 PWM outputs, the TIM1 break input,
 *          the ADC sense input and the fault-status LED.
 ******************************************************************************
 */

#include "stm32f103xx.h"
#include "gpio.h"


#define CNF_AF_PP           0x2U   
#define CNF_IN_PULL         0x2U   
#define CNF_IN_ANALOG       0x0U   
#define CNF_OUT_PP          0x0U   
#define MODE_OUT_50MHZ      0x3U

static void gpio_set_pin_config(GPIO_TypeDef *port, uint32_t pin,
                                 uint32_t mode, uint32_t cnf)
{
    volatile uint32_t *cr = (pin < 8U) ? &port->CRL : &port->CRH;
    uint32_t shift = (pin % 8U) * 4U;
    uint32_t value = *cr;

    value &= ~(0xFUL << shift);
    value |= ((cnf << 2U) | mode) << shift;
    *cr = value;
}

void GPIO_Init(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
                    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;


    gpio_set_pin_config(GPIOA, 8U,  MODE_OUT_50MHZ, CNF_AF_PP);
    gpio_set_pin_config(GPIOA, 9U,  MODE_OUT_50MHZ, CNF_AF_PP);
    gpio_set_pin_config(GPIOA, 10U, MODE_OUT_50MHZ, CNF_AF_PP);
    gpio_set_pin_config(GPIOA, 11U, MODE_OUT_50MHZ, CNF_AF_PP);


    gpio_set_pin_config(GPIOB, 12U, 0x0U, CNF_IN_PULL);
    GPIOB->ODR |= (1UL << 12);   

    gpio_set_pin_config(GPIOA, 5U, 0x0U, CNF_IN_ANALOG);

    gpio_set_pin_config(GPIOC, 13U, MODE_OUT_50MHZ, CNF_OUT_PP);
    GPIOC->BSRR = (1UL << 13); 
}

void GPIO_FaultLedOn(void)
{
    GPIOC->BRR = (1UL << 13);   
}

void GPIO_FaultLedOff(void)
{
    GPIOC->BSRR = (1UL << 13);  
}
