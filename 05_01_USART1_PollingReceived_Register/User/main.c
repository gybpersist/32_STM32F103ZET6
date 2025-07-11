#include <stdint.h>
#include "stm32f10x.h"                  // Device header
#include "Com_Util.h"
#include "Dri_USART1.h"


int main()
{
	// ��ʼ�� USART1
	Dri_USART1_Init();

	uint8_t rx_bytes[100];
	uint16_t rx_len = 0;

	// ����һ���ֽ�
	Dri_USART1_TransmitBytes("��� SB\n",7);
	
	while (1)
	{
		Dri_USART1_ReceiveBytes(rx_bytes,&rx_len);

		Dri_USART1_TransmitBytes(rx_bytes,rx_len);
	}
}
