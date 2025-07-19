#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_TIM6.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 SysTick Example: \n");

	// 初始化LED
	Int_LED_Init();

	// 初始化 TIM6 ,并计时时间为 1000ms
	Dri_TIM6_Init(1000);

	// 开启 计时
	Dri_TIM6_Start();

	// 死循环
	while (1)
	{
	}
		
}

// 重定义弱函数,产生更新中断会被调用
void Dri_TIM6_UpdateCallback(void){
	// 调用 LED 闪烁函数
	Int_LED_Toggle(LED3);
}
