#include <stdint.h>
#include "stm32f10x.h"
#include "Com_Util.h"
#include "Dri_USART1.h"
#include "Dri_SysTick.h"
#include "Int_LED.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 SysTick Example: \n");

	// 初始化LED
	Int_LED_Init();

	// 初始化SysTick
	Dri_SysTick_Init();

	// 死循环
	while (1)
		;
}

// 重定义 SysTick 回调函数
// 1s 中断一次 (调用1000次,LED1闪烁一次)
void Dri_SysTick_Callback(void)
{
	static uint16_t count = 1000;

	count--;

	if(count == 0)
	{
		count = 1000;
		// 调用 LED 闪烁函数
		Int_LED_Toggle(LED1);
	}
}
