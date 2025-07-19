#include "Com_Util.h"
#include "Int_LED.h"
#include "Int_Key.h"

uint16_t leds[3] = {LED1,LED2,LED3};	//2 �� leds

int main(void)
{	
	// LED ��ʼ��
	 Int_LED_Init();

	// ������ʼ��
	Int_Key_Init();

	// // 1 �� LED1
	// Int_LED_On(LED1);
	
	// // 2 �� leds
	// Int_LED_OnAll(leds,3);

	// ��ֹ�ܷ�
	while (1)
	{
		// // 3 ��˸ LED2
		// Int_LED_Toggle(LED2);
		// Com_Delay_ms(500);

		// // 4 LED ��ˮ�� 1 2 3 2  1 2 3 2  1 2 3 2...
		// for (uint8_t i = 0; i < 3 ; i++)
		// {
		// 	Int_LED_Toggle(leds[i]);
		// 	Com_Delay_ms(500);
		// 	Int_LED_Toggle(leds[i]);
		// }
		// Int_LED_Toggle(LED2);
		// Com_Delay_ms(500);
		// Int_LED_Toggle(LED2);
	}
}

// �ض��� SW3 ���������� ,�����жϺ󽫻ᱻ����
void Int_Key_IsSW3Callback(void){
	Int_LED_Toggle(LED2);
}
// �ض��� SW4 ���������� ,�����жϺ󽫻ᱻ����
void Int_Key_IsSW4Callback(void){
	Int_LED_Toggle(LED2);
}
// �ض��� SW5 ���������� ,�����жϺ󽫻ᱻ����
void Int_Key_IsSW5Callback(void){
	Int_LED_Toggle(LED2);
}
// �ض��� SW6 ���������� ,�����жϺ󽫻ᱻ����
void Int_Key_IsSW6Callback(void){
	Int_LED_Toggle(LED2);
}
