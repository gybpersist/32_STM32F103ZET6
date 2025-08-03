#include "Dri_TIM3.h"

void Dri_TIM3_Init(void)
{
    // 1 时钟使能
    // 1.1 对 TIM3 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // 1.2 对 GPIOB 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2 GPIO 设置
    // 2.1 将 TIM3_CH3(PB0) 设置为复用推挽输出 (MODE:11,CNF:10)
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL |= GPIO_CRL_CNF0_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    // 3 TIM3 时基单元设置
    // 3.1 设置 CNT 计数模式 (默认:向上计数模式)
    TIM3->CR1 &=~ TIM_CR1_CMS;          // 关闭中央对齐计数模式
    TIM3->CR1 &= ~TIM_CR1_DIR;          // 向上计数模式
    // 3.2 设置 PSC 预分频值 ,设置 10us 一个数
    TIM3->PSC = 719;
    // 3.3 设置 ARR 自动重装载值 ,设置每 1ms (计100个数) 产生一次更新事件
    TIM3->ARR = 99;

    // 4 TIM3 输出比较设置
    // 4.1 设置 TIM3_CH3  为输出比较模式 (默认:CCMR1.CC2S==00)
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    // 4.2 设置 TIM3_CH3  输出比较模式为 PWM模式1 (CCMR1.OC2M=110)
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    // 4.3 设置 TIM3_CH3 输出极性为高电平有效 (默认:CCER.CC2P=0)
    TIM3->CCER &= ~TIM_CCER_CC3P;
    // 4.4 设置 TIM3_CH3 输出使能 (CCER.CC2E=1)
    TIM3->CCER |= TIM_CCER_CC3E;
}

void Dri_TIM3_Start(void)
{
    //设置 TIM3 计数使能
    TIM3->CR1 |= TIM_CR1_CEN; 
}

void Dri_TIM3_Stop(void)
{
    //关闭 TIM3 计数使能
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

// 设置 TIM3_CH3 PWM 的占空比
void Dri_TIM3_CH3_SetPWMDuty(uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    // 给 CCR3 赋值 ,设置占空比
    TIM3->CCR3 = duty;
}
