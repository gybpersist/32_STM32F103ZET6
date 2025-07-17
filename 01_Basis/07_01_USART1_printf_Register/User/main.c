#include <stdint.h>
#include "stm32f10x.h"                  // Device header
#include "Com_Util.h"
#include "Dri_USART1.h"


int main()
{
	uint8_t *name = "TOM";
	// 初始化 USART1
	Dri_USART1_Init();

	// 开启 USART1 接收的中断形式
	Dri_USART1_Receive_IT_Start();

	// 发送一个字节
	printf("Hello SB:\n");
	
	printf("%s is SB",name);

	while (1)
	{

	}
}

// 重写回调函数
void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
	Dri_USART1_TransmitBytes(rx_bytes,rx_len);
}
