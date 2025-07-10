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
	// 定义数组,保存3个LED
	// uint16_t leds[3] = {LED1,LED2,LED3};
	
	// 防止跑飞
	while (1)
	{
		// LED1闪烁
		// Int_LED_Toggle(LED1);
		// Com_Delay_ms(500);

		// 所有LED闪烁
		// Int_LED_OnAll(leds,3);
		// Com_Delay_ms(500);
		// Int_LED_OffAll(leds,3);
		// Com_Delay_ms(500);

		// LED流水灯
		// for (uint8_t i = 0; i < 3; i++)
		// {
		// 	Int_LED_On(leds[i]); //点亮
		// 	Com_Delay_ms(500);
		// 	Int_LED_Off(leds[i]); //熄灭
		// }
	}
}

// 重写弱函数 
void Int_Key_SW3Callback(void){
	// 改变LED3的状态
	Int_LED_Toggle(LED1);
}
// 重写弱函数 
void Int_Key_SW4Callback(void){
	// 改变LED3的状态
	Int_LED_Toggle(LED2);
}

// 重写弱函数 
void Int_Key_SW5Callback(void){
	// 改变LED3的状态
	Int_LED_Toggle(LED3);
}

// 重写弱函数 
void Int_Key_SW6Callback(void){
	// 改变LED3的状态
	Int_LED_OffAll(leds,3);
}

