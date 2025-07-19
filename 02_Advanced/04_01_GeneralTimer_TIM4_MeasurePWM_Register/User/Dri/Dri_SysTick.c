#include "Dri_SysTick.h"

// ����ȫ�ֱ��� ������ʱ���� ÿ��1ms��һ��
static uint16_t s_delay_ms = 0;

void Dri_SysTick_Init(void)
{
    // 1 SysTick ����
    // 1.1 ���� SysTick ʱ��ԴΪ������ʱ�� HCLK 
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    // 1.2 ���� SysTick ��װ��ֵΪ 72M/8 - 1
    SysTick->LOAD = (72 * 1000 - 1);
    // 1.3 ���� SysTick �ж�
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    // 1.4 ʹ�� SysTick �ж�
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    // 2 NVIC ����
    // 2/1 ����ȫ����ռ���ȼ�
    NVIC_SetPriorityGrouping(3);
    // 2.2 �������ȼ�Ϊ 0
    NVIC_SetPriority(SysTick_IRQn, 0);
    // 2.3 ʹ�� SysTick �ж�  --- NVIC ����ʹ�� SysTick �ں��ж�
    // NVIC_EnableIRQ(SysTick_IRQn);
}

void Dri_SysTick_Delay_ms(uint16_t ms)
{
    s_delay_ms = ms;
    while (s_delay_ms);
}

// ���� SysTick ������Ļص�����
__weak void Dri_SysTick_Callback(void)
{
}

// SysTick �жϴ�����
void SysTick_Handler(void)
{
    if (s_delay_ms)
    {
        s_delay_ms--;
    }
    
    // ���� SysTick �ص�����
    Dri_SysTick_Callback();
}
