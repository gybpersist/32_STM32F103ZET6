#include "Int_LED.h"

void Int_LED_Init(void)
{
    //1 对 GPIOA 时钟使能 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//2 设置 PA0 PA1 PA8 的引脚模式   CNF 00  MODE 11
	// 设置 PA0 PA1
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
	GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
	// 设置 PA8
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 3 设置 PA0 PA1 PA8 输出高电平
	GPIOA->ODR |= (LED1 | LED2 | LED3); 
}

void Int_LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led; //点亮
}

void Int_LED_Off(uint16_t led)
{
    GPIOA->ODR |= led;
}

void Int_LED_Toggle(uint16_t led)
{
    if ((GPIOA->ODR & led) == 1)  // 如果当前位为1（LED灭）
    {
        Int_LED_On(led);   // 打开LED（输出0）
    }
    else                        // 如果当前位为0（LED亮）
    {
        Int_LED_Off(led);  // 关闭LED（输出1）
    }
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
