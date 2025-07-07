#include "Int_LED.h"

void Int_LED_On(uint16_t led)
{
    // GPIOA->ODR &= ~led; //点亮
    HAL_GPIO_WritePin(GPIOA,led,GPIO_PIN_RESET);
}

void Int_LED_Off(uint16_t led)
{
    // GPIOA->ODR |= led;
    HAL_GPIO_WritePin(GPIOA,led,GPIO_PIN_SET);
}

void Int_LED_Toggle(uint16_t led)
{
    // if ((GPIOA->ODR & led) == 1)  // 如果当前位为1（LED灭）
    // {
    //     Int_LED_On(led);   // 打开LED（输出0）
    // }
    // else                        // 如果当前位为0（LED亮）
    // {
    //     Int_LED_Off(led);  // 关闭LED（输出1）
    // }
    HAL_GPIO_TogglePin(GPIOA,led);
}

void Int_LED_OnAll(uint16_t *leds, uint8_t leds_len)
{
    for (uint8_t i = 0; i < leds_len; i++)
    {
        Int_LED_On(leds[i]);
    }
}

void Int_LED_OffAll(uint16_t *leds,uint8_t leds_len)
{
    for (uint8_t i = 0; i < leds_len; i++)
    {
        Int_LED_Off(leds[i]);
    }
}
