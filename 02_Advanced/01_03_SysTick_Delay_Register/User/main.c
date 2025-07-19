#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_SysTick.h"
#include "Int_LED.h"

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 SysTick Example: \n");

	// ��ʼ��LED
	Int_LED_Init();

	// ��ʼ��SysTick
	Dri_SysTick_Init();

	// ��ѭ��
	while (1)
	{
		// ���� LED ��˸����
		Int_LED_Toggle(LED3);
		// ��ʱ1000ms
		Dri_SysTick_Delay_ms(1000);
	}
		
}

