#include "Dri_TIM5.h"

void Dri_TIM5_Init(void)
{
    // 1 时钟使能
    // 1.1 对 TIM5 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    // 1.2 对 GPIOA 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2 GPIO 设置
    // 2.1 将 TIM5_CH2(PA1) TIM5_CH1(PA0) 设置为复用推挽输出 (MODE:11,CNF:10)
    GPIOA->CRL |= GPIO_CRL_MODE1 | GPIO_CRL_MODE0;
    GPIOA->CRL |= GPIO_CRL_CNF1_1 | GPIO_CRL_CNF0_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0 | GPIO_CRL_CNF0_0;

    // 3 TIM5 时基单元设置
    // 3.1 设置 CNT 计数模式 (默认:向上计数模式)
    TIM5->CR1 &=~ TIM_CR1_CMS;          // 关闭中央对齐计数模式
    TIM5->CR1 &= ~TIM_CR1_DIR;          // 向上计数模式
    // 3.2 设置 PSC 预分频值 ,设置 10us 一个数
    TIM5->PSC = 719;
    // 3.3 设置 ARR 自动重装载值 ,设置每 1ms (计100个数) 产生一次更新事件
    TIM5->ARR = 99;

    // 4 TIM5 输出比较设置
    // 4.1 设置 TIM5_CH2 TIM5_CH1 为输出比较模式 (默认:CCMR1.CC2S==00)
    TIM5->CCMR1 &= ~(TIM_CCMR1_CC2S | TIM_CCMR1_CC1S);
    // 4.2 设置 TIM5_CH2 TIM5_CH1 输出比较模式为 PWM模式1 (CCMR1.OC2M=110)
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC1M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1M_1;
    TIM5->CCMR1 &= ~(TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC1M_0);
    // 4.3 设置 TIM5_CH2 输出极性为高电平有效 (默认:CCER.CC2P=0)
    TIM5->CCER &= ~TIM_CCER_CC2P;
    TIM5->CCER |= TIM_CCER_CC1P;        // TIM5_CH1 输出极性为低电平有效
    // 4.4 设置 TIM5_CH2 输出使能 (CCER.CC2E=1)
    TIM5->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
}

void Dri_TIM5_Start(void)
{
    //设置 TIM5 计数使能
    TIM5->CR1 |= TIM_CR1_CEN; 
}

void Dri_TIM5_Stop(void)
{
    //关闭 TIM5 计数使能
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void Dri_TIM5_CH2_SetPWMDuty(uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    // 给 CCR2 赋值 ,设置占空比
    TIM5->CCR2 = duty;
}

void Dri_TIM5_CH1_SetPWMDuty(uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    // 给 CCR1 赋值 ,设置占空比
    TIM5->CCR1 = duty;
}
