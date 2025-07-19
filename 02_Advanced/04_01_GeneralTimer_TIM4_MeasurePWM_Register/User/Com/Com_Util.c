#include "Com_Util.h"

// �ӳٶ���΢��
void Com_Delay_us(uint16_t count){
	// ���õδ�ʱ������װ��ֵ
	SysTick->LOAD = 72 * count;

	// ���õδ�ʱ����ʱ��Դ�Ǵ�����ʱ��
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

	// �δ�ʱ����ʼ����
	SysTick->CTRL |= SysTick_CTRL_ENABLE;

	// // �ȴ��δ�ʱ��������0 COUNTFLAG��־λ0:û����
	// while (((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0) && SysTick->CTRL & SysTick_CTRL_ENABLE)
	// {
	// }
	
	// 4. �ȴ��δ�ʱ���Ƶ�0�����û�Ƶ�0��һֱѭ��, ѭ������: COUNTFLAG��־λ��0��
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0)
		;

	// �δ�ʱ��ֹͣ����
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

// �ӳٶ��ٺ���
void Com_Delay_ms(uint16_t count){
	while (count--)
	{
		Com_Delay_us(1000);

	}
}

// �ӳٶ�����
void Com_Delay_s(uint16_t count){
	while (count--)
	{
		Com_Delay_ms(1000);
		
	}
}
