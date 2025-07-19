#include "Com_Util.h"

void Com_Delay_us(uint16_t us)
{
    // 1 设置 SysTick 的时钟源
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;    // 使用 内核时钟（HCLK），即系统时钟
    // 2 设置 SysTick 自动重装载值  
    SysTick->LOAD = 72 * us;    // f=72MHZ T=1/72M  每执行 72*us 次,相当于 1us
    // 3 设置 SysTick 开始计数 (使能)
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    // 4 等待 SysTick 计数完成
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0 && SysTick->CTRL & SysTick_CTRL_ENABLE)
    {}
    
    // 5 停止 SysTick 计数
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Com_Delay_ms(uint16_t ms)
{
    while (ms)
    {
        Com_Delay_us(1000);
        ms--;
    }
}
