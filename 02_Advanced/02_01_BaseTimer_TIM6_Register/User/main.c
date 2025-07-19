#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_TIM6.h"

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 SysTick Example: \n");

	// ��ʼ��LED
	Int_LED_Init();

	// ��ʼ�� TIM6 ,����ʱʱ��Ϊ 1000ms
	Dri_TIM6_Init(1000);

	// ���� ��ʱ
	Dri_TIM6_Start();

	// ��ѭ��
	while (1)
	{
	}
		
}

// �ض���������,���������жϻᱻ����
void Dri_TIM6_UpdateCallback(void){
	// ���� LED ��˸����
	Int_LED_Toggle(LED3);
}
