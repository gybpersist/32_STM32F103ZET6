#include "Int_LED.h"

void Int_LED_Init(void)
{
    // 1 时钟使能 (GPIOA)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2 GPIO 配置
    // 2.1 设置 PA0 PA1 PA8 为通用推挽输出 MODE:11 CNF:00
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
    GPIOA->CRH &= ~GPIO_CRH_CNF8;

    // 3 设置 ODR 数据输出寄存器 (默认输出高电平)
    GPIOA->ODR |= (GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR8);
}

void Int_LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led; //输出低电平
}

void Int_LED_Off(uint16_t led)
{
    GPIOA->ODR |= led; //输出高电平
}

void Int_LED_Toggle(uint16_t led)
{
    if ((GPIOA->ODR & led) == 0)
    {
        Int_LED_Off(led);
    }else
    {
        Int_LED_On(led);
    }
}

void Int_LED_OnAll(uint16_t *leds, uint8_t leds_len)
{
    for (uint8_t i = 0; i < leds_len; i++)
    {
        Int_LED_On(leds[i]);
    }
}

void Int_LED_OffAll(uint16_t *leds, uint8_t leds_len)
{
    for (uint8_t i = 0; i < leds_len; i++)
    {
        Int_LED_On(leds[i]);
    }
}
