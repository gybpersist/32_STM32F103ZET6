#include "Com_Util.h"

// 延迟多少微秒
void Com_Delay_us(uint16_t count){
	// 设置滴答定时器的重装载值
	SysTick->LOAD = 72 * count;

	// 设置滴答定时器的时钟源是处理器时钟
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

	// 滴答定时器开始计数
	SysTick->CTRL |= SysTick_CTRL_ENABLE;

	// 等待滴答定时器计数到0 COUNTFLAG标志位0:没计完
	while (((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0) && SysTick->CTRL & SysTick_CTRL_ENABLE)
	{
		
	}
	
	// 滴答定时器停止计数
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

// 延迟多少毫秒
void Com_Delay_ms(uint16_t count){
	while (count)
	{
		Com_Delay_us(1000);
		count--;
	}
}
