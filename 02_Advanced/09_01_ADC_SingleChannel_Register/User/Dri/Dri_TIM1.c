#include "Dri_TIM1.h"
#include "Dri_USART1.h"

void Dri_TIM1_Init(void)
{ 
    // 1 时钟配置
    // 1.1 GPIOA 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 TIM1 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // 2 GPIO 配置
    // 2.1 设置 TIM1_CH1(PA8) 为复用推挽输出 (MODE:11,CNF:10)
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
    
    // 3 TIM1 时基单元配置
    // 3.1 设置 CNT 的计数模式:向上计数 (默认)
    TIM1->CR1 &= ~TIM_CR1_DIR;
    // 3.2 设置 PSC 预分频器值 ,实现每 100us 计数一次
    TIM1->PSC = 7199;
    // 3.3 设置 ARR 自动重装载值 ,实现每 500ms 溢出一次
    TIM1->ARR = 4999;

    // 4 TIM1_CH1 输出比较设置
    // 4.1 设置 TIM1_CH1 为输出比较模式 (CCMR1.CC1S=00 默认)
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
    // 4.2 设置 TIM1_CH1 输出比较模式是:PWM模式1 (CCMR1.OC1M=110)
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    // 4.3 设置 TIM1_CH1 输出极性是:高电平为有效电平 (CCER.CC1P=0 默认)
    TIM1->CCER &= ~TIM_CCER_CC1P;
    // 4.4 设置 TIM1_CH1 输出使能 (CCER.CC1E=1)
    TIM1->CCER |= TIM_CCER_CC1E;
    // 4.5 设置 TIM1_CH1 输出 PWM 方波的占空比为: ( TIM1_CH1_PWM_DUTY * (TIM1->ARR + 1) )/ 100
    TIM1->CCR1 = ( TIM1_CH1_PWM_DUTY * (TIM1->ARR + 1) )/ 100;
    // 4.6 设置开启 TIM1_CH1 的主输出使能 (BDTR.MOE=1)
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 5 TIM1 重复计数器设置
    // 5.1 设置 CNT 溢出5次才产生一次更新事件
    TIM1->RCR = 4;

    // 6 TIM1 中断和事件设置
    // 6.1 开启 TIM1 的更新中断使能
    TIM1->DIER |= TIM_DIER_UIE;
    // 6.2 软件方式主动产生一次更新事件, 产生更新事件后，PSC、RCR 的值会存入影子寄存器
    TIM1->EGR |= TIM_EGR_UG;

    // 7 NVIC 设置
    NVIC_SetPriorityGrouping(3);        // 4为全是抢占优先级
    NVIC_SetPriority(TIM1_UP_IRQn, 5);  // 中断优先级5
    NVIC_EnableIRQ(TIM1_UP_IRQn);       // 使能中断向量
}

void Dri_TIM1_Start(void)
{
    // 启动 TIM1 计数
    TIM1->CR1 |= TIM_CR1_CEN;
}

void Dri_TIM1_Stop(void)
{
    // 停止 TIM1 计数
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief TIM1 的中断服务程序
 *
 */
void TIM1_UP_IRQHandler(void)
{
    // 1 清除 UIF 更新中断标志位
    TIM1->SR &= ~TIM_SR_UIF;
    // 2 停止 TIM1 的计数使能
    Dri_TIM1_Stop();

    printf("TIM1_UP_IRQHandler !!!");
}
