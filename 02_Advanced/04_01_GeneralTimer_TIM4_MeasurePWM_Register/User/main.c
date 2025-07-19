#include "Dri_USART1.h"
#include "Dri_TIM4.h"
#include "Dri_TIM5.h"

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 GeneralYimer TIM4 T and f Example: \n");

	// ��ʼ�� TIM4
	Dri_TIM4_Init();
	Dri_TIM4_Start();


	// ��ʼ�� TIM5 ,Ĭ���� 1KHZ
	Dri_TIM5_Init();
	Dri_TIM5_Start();
	Dri_TIM5_CH2_SetPWMDuty(50); //���ó�ʼռ�ձ�

	// ��ѭ��
	while (1)
	{
		Com_Delay_s(2);
		printf("PWM T:%.2lfms,f:%.2lfHZ",Dri_TIM4_CH1_GetPWMPeriod(),Dri_TIM4_CH1_GetPWMFreq());

	}
		
}

