#include <stdint.h>
#include "stm32f10x.h"                  // Device header
#include "Com_Util.h"
#include "Int_LED.h"
#include "Int_Key.h"

uint16_t leds[3] = {LED1,LED2,LED3};

int main()
{
	Int_LED_Init();
	Int_Key_Init();
	// ��������,����3��LED
	// uint16_t leds[3] = {LED1,LED2,LED3};
	
	// ��ֹ�ܷ�
	while (1)
	{
		// LED1��˸
		// Int_LED_Toggle(LED1);
		// Com_Delay_ms(500);

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

// ��д������ 
void Int_Key_SW3Callback(void){
	// �ı�LED3��״̬
	Int_LED_Toggle(LED1);
}
// ��д������ 
void Int_Key_SW4Callback(void){
	// �ı�LED3��״̬
	Int_LED_Toggle(LED2);
}

// ��д������ 
void Int_Key_SW5Callback(void){
	// �ı�LED3��״̬
	Int_LED_Toggle(LED3);
}

// ��д������ 
void Int_Key_SW6Callback(void){
	// �ı�LED3��״̬
	Int_LED_OffAll(leds,3);
}

