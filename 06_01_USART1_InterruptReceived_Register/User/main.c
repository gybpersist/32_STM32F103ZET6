#include <stdint.h>
#include "stm32f10x.h"                  // Device header
#include "Com_Util.h"
#include "Dri_USART1.h"


int main()
{
	// ��ʼ�� USART1
	Dri_USART1_Init();

	// ���� USART1 ���յ��ж���ʽ
	Dri_USART1_Receive_IT_Start();

	// ����һ���ֽ�
	Dri_USART1_TransmitBytes("Hello SB:\n",10);
	
	while (1)
	{

	}
}

// ��д�ص�����
void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
	Dri_USART1_TransmitBytes("Receive Data IT:",16);
	Dri_USART1_TransmitBytes(rx_bytes,rx_len);
}
