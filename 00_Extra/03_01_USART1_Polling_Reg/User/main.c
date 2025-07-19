#include "Com_Util.h"
#include "Dri_USART1.h"

uint8_t rx_bytes[100];
uint16_t rx_len = 0;

int main(void)
{	
	// USART1 ≥ı ºªØ
	Dri_USART1_Init();

	printf("5201314\n",8);
	printf("HUI",3);

	// ∑¿÷π≈‹∑…
	while (1)
	{
		Dri_USART1_ReceiveBytes(rx_bytes,&rx_len);
		Dri_USART1_TransmitBytes(rx_bytes,rx_len);

	}
}
