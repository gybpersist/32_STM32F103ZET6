#include "Dri_USART1.h"
#include "Dri_TIM1.h"

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 AdvancedTimer TIM1 Example: \n");

	// TIM1 初始化
	Dri_TIM1_Init();
	// 启动 TIM1 计数
	Dri_TIM1_Start();

	// 死循环
	while (1)
	{
	}
		
}

