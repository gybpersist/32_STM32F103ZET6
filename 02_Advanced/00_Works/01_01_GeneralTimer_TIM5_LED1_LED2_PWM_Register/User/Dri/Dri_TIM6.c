#include "Dri_TIM6.h"

void Dri_TIM6_Init(uint16_t up_ms)
{
    // 1 时钟使能
    // 对 TIM6 使能
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2 TIM6 设置
    // 2.1 时基单元设置:设置预分频值 ,实现每 100us 计一个数 
    TIM6->PSC = 7200 - 1;       // f = 72M / (PSC + 1) = 1000000 / 7200 = 100us
    // 2.2 时基单元设置:设置自动重装载值
    TIM6->ARR = up_ms * 10- 1;      // 每 up_ms 毫秒产生一次更新事件
    // 2.3 打开更新中断使能
    TIM6->DIER |= TIM_DIER_UIE;

    // 3 NVIC 设置
    NVIC_SetPriorityGrouping(3);    // 设置抢占优先级为4位
    NVIC_SetPriority(TIM6_IRQn,5);  // 设置 TIM6 中断向量的优先级
    NVIC_EnableIRQ(TIM6_IRQn);      // 使能 TIM6 中断向量

}

void Dri_TIM6_Start(void)
{
    // 打开 TIM6 的计数使能
    TIM6->CR1 |= TIM_CR1_CEN;
}

void Dri_TIM6_Stop(void)
{
    // 关闭 TIM6 的计数使能
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

// 定义弱函数,产生更新中断会被调用
__weak void Dri_TIM6_UpdateCallback(void){
}

/**
 * @brief TIM6 的中断服务程序
 * 
 */
void TIM6_IRQHandler(void){

    // 清除更新事件中断标志位
    TIM6->SR &= ~TIM_SR_UIF;

    // 调用弱函数
    Dri_TIM6_UpdateCallback();
}
