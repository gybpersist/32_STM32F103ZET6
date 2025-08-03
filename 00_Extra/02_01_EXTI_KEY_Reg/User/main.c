#include "Com_Util.h"
#include "Int_LED.h"
#include "Int_Key.h"

uint16_t leds[3] = {LED1,LED2,LED3};	//2 打开 leds

int main(void)
{	
	// LED 初始化
	 Int_LED_Init();

	// 按键初始化
	Int_Key_Init();

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

// 重定义 SW3 按下弱函数 ,触发中断后将会被调用
void Int_Key_IsSW3Callback(void){
	Int_LED_Toggle(LED2);
}
// 重定义 SW4 按下弱函数 ,触发中断后将会被调用
void Int_Key_IsSW4Callback(void){
	Int_LED_Toggle(LED2);
}
// 重定义 SW5 按下弱函数 ,触发中断后将会被调用
void Int_Key_IsSW5Callback(void){
	Int_LED_Toggle(LED2);
}
// 重定义 SW6 按下弱函数 ,触发中断后将会被调用
void Int_Key_IsSW6Callback(void){
	Int_LED_Toggle(LED2);
}
