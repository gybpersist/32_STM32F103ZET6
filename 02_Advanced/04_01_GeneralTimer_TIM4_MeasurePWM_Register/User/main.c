#include "Dri_USART1.h"
#include "Dri_TIM4.h"
#include "Dri_TIM5.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 GeneralYimer TIM4 T and f Example: \n");

	// 初始化 TIM4
	Dri_TIM4_Init();
	Dri_TIM4_Start();


	// 初始化 TIM5 ,默认是 1KHZ
	Dri_TIM5_Init();
	Dri_TIM5_Start();
	Dri_TIM5_CH2_SetPWMDuty(50); //设置初始占空比

	// 死循环
	while (1)
	{
		Com_Delay_s(2);
		printf("PWM T:%.2lfms,f:%.2lfHZ",Dri_TIM4_CH1_GetPWMPeriod(),Dri_TIM4_CH1_GetPWMFreq());

	}
		
}

