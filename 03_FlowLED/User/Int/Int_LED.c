#include "Int_LED.h"

void Int_LED_Init(void)
{
    //1 �� GPIOA ʱ��ʹ�� 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//2 ���� PA0 PA1 PA8 ������ģʽ   CNF 00  MODE 11
	// ���� PA0 PA1
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
	GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
	// ���� PA8
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 3 ���� PA0 PA1 PA8 ����ߵ�ƽ
	GPIOA->ODR |= (LED1 | LED2 | LED3); 
}

void Int_LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led; //����
}

void Int_LED_Off(uint16_t led)
{
    GPIOA->ODR |= led;
}

void Int_LED_Toggle(uint16_t led)
{
    if ((GPIOA->ODR & led) == 1)  // �����ǰλΪ1��LED��
    {
        Int_LED_On(led);   // ��LED�����0��
    }
    else                        // �����ǰλΪ0��LED����
    {
        Int_LED_Off(led);  // �ر�LED�����1��
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
