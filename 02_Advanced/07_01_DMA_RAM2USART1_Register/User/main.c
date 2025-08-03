#include "Dri_USART1.h"
#include "Dri_TIM1.h"
#include "Dri_DMA.h"

uint8_t TX_DMA[] = "HELLO HUI!\n"; // 存放在RAM中

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("DMA RAM->DMA->USART1_TX: \n");

	// DMA 初始化
	Dri_DMA_Init();
	// DMA 开启并发送 RAM 数据 RAM->DMA->USART1_TX
	Dri_DMA_Transfer((uint32_t)TX_DMA,(uint32_t)(&USART1->DR),sizeof(TX_DMA)-1);

	// 死循环
	while (1)
	{
	}
		
}

// 重定义 DMA1_Channel4 TC 中断触发后调用的弱函数
void Dri_DMA_TC_Callback(void)
{
	printf("Finished!!");
}
