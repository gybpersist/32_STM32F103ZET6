#include "stm32f10x.h"                  // Device header
#include "Com_Util.h"
#include "Int_LED.h"

int main(void)
{
	Int_LED_Init();
	
	// ��������,����3��LED
	// uint16_t leds[3] = {LED1,LED2,LED3};

	// ��ֹ�ܷ�
	while (1)
	{
		// LED1��˸
		Int_LED_Toggle(LED1);
		Com_Delay_ms(500);

		// ����LED��˸
		// Int_LED_OnAll(leds,3);
		// Com_Delay_ms(500);
		// Int_LED_OffAll(leds,3);
		// Com_Delay_ms(500);

		// LED��ˮ��
		// for (uint8_t i = 0; i < 3; i++)
		// {
		// 	Int_LED_On(leds[i]); //����
		// 	Com_Delay_ms(500);
		// 	Int_LED_Off(leds[i]); //Ϩ��
		// }
	}
}
