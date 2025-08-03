#include "Com_Util.h"
#include "Int_LED.h"

uint16_t leds[3] = {LED1,LED2,LED3};	//2 打开 leds

int main(void)
{	
	// LED 初始化
	 Int_LED_Init();

	// // 1 打开 LED1
	// Int_LED_On(LED1);
	
	// // 2 打开 leds
	// Int_LED_OnAll(leds,3);

	// 防止跑飞
	while (1)
	{
		// // 3 闪烁 LED2
		// Int_LED_Toggle(LED2);
		// Com_Delay_ms(500);

		// // 4 LED 流水灯 1 2 3 2  1 2 3 2  1 2 3 2...
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
