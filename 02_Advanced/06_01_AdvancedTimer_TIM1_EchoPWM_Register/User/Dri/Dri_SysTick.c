#include "Dri_SysTick.h"

// 定义全局变量 用于延时计数 每隔1ms减一次
static uint16_t s_delay_ms = 0;

void Dri_SysTick_Init(void)
{
    // 1 SysTick 设置
    // 1.1 设置 SysTick 时钟源为处理器时钟 HCLK 
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    // 1.2 设置 SysTick 重装载值为 72M/8 - 1
    SysTick->LOAD = (72 * 1000 - 1);
    // 1.3 开启 SysTick 中断
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    // 1.4 使能 SysTick 中断
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    // 2 NVIC 设置
    // 2/1 设置全是抢占优先级
    NVIC_SetPriorityGrouping(3);
    // 2.2 设置优先级为 0
    NVIC_SetPriority(SysTick_IRQn, 0);
    // 2.3 使能 SysTick 中断  --- NVIC 不能使能 SysTick 内核中断
    // NVIC_EnableIRQ(SysTick_IRQn);
}

void Dri_SysTick_Delay_ms(uint16_t ms)
{
    s_delay_ms = ms;
    while (s_delay_ms);
}

// 定义 SysTick 触发后的回调函数
__weak void Dri_SysTick_Callback(void)
{
}

// SysTick 中断处理函数
void SysTick_Handler(void)
{
    if (s_delay_ms)
    {
        s_delay_ms--;
    }
    
    // 调用 SysTick 回调函数
    Dri_SysTick_Callback();
}
