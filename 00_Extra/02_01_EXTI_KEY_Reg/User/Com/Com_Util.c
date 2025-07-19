#include "Com_Util.h"

void Com_Delay_us(uint16_t us)
{
    // 1 ���� SysTick ��ʱ��Դ
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;    // ʹ�� �ں�ʱ�ӣ�HCLK������ϵͳʱ��
    // 2 ���� SysTick �Զ���װ��ֵ  
    SysTick->LOAD = 72 * us;    // f=72MHZ T=1/72M  ÿִ�� 72*us ��,�൱�� 1us
    // 3 ���� SysTick ��ʼ���� (ʹ��)
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    // 4 �ȴ� SysTick �������
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0 && SysTick->CTRL & SysTick_CTRL_ENABLE)
    {}
    
    // 5 ֹͣ SysTick ����
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Com_Delay_ms(uint16_t ms)
{
    while (ms)
    {
        Com_Delay_us(1000);
        ms--;
    }
}
