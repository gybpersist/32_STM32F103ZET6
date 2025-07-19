#include "Int_LED.h"

void Int_LED_Init(void)
{
    // 1 ʱ��ʹ�� (GPIOA)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2 GPIO ����
    // 2.1 ���� PA0 PA1 PA8 Ϊͨ��������� MODE:11 CNF:00
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
    GPIOA->CRH &= ~GPIO_CRH_CNF8;

    // 3 ���� ODR ��������Ĵ��� (Ĭ������ߵ�ƽ)
    GPIOA->ODR |= (GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR8);
}

void Int_LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led; //����͵�ƽ
}

void Int_LED_Off(uint16_t led)
{
    GPIOA->ODR |= led; //����ߵ�ƽ
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
