#include "Dri_USART1.h"
#include "Dri_TIM1.h"
#include "Dri_DMA.h"

uint8_t RX_DMA[128] = {0}; // 存放在RAM中

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 DMA USART1_RX->DMA->RAM Example: \n");

	// DMA 初始化
	Dri_DMA_Init();
	// DMA 开启并接收 RAM 数据 USART1_RX->DMA->RAM (此次只能接收定长数据)
	Dri_DMA_Transfer((uint32_t)RX_DMA,(uint32_t)(&USART1->DR),10);

	// 死循环
	while (1)
	{
	}
		
}

// 重定义 DMA1_Channel4 TC 中断触发后调用的弱函数
void Dri_DMA_TC_Callback(void)
{
	printf("RX_DMA:%s",RX_DMA);
}
