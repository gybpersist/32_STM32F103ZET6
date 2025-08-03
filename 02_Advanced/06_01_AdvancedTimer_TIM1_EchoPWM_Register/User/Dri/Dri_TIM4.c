#include "Dri_TIM4.h"

void Dri_TIM4_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIOB 使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 TIM4 使能
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // 2 GPIO 配置
    // 2.1 TIM4_CH1(PB6) 为浮空输入模式 (MODE:00,CNF:01 默认)
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3 TIM4 时基单元配置
    // 3.1 设置 CNT 的计数模式:向上计数 (默认)
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // 3.2 设置 PSC 预分频器值 ,实现每 1us 计数一次 [可测量频率: f < 1MHZ 的PWM方波]
    TIM4->PSC = 71;
    // 3.3 设置 ARR 自动重装载值 ,设置为最大值 [可测量频率: f > 16HZ 的PWM方波]
    TIM4->ARR = 0xFFFF;

    // 4 TIM4_CH1 输入捕获设置
    // 4.1 设置 TIM4_CH1 为输入捕获模式,并设置 IC1映射在TI1上 (CCMR1.CC1S=01)
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    // 4.2 设置 TI1 选择 TIM4_CH1 引脚 (CR2.TI1S=0 默认)
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 4.3 设置不进行输入滤波 (CCMR1.IC1F=0000 默认)
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 4.4 设置为上升沿触发捕获事件 (CCER.CC1P=0 默认)
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 4.5 设置不进行 IC1 预分频 (CCMR1.IC1PSC=00 默认)
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 4.6 使能 TIM4_CH1 输入捕获 (CCER.CC1E=1)
    TIM4->CCER |= TIM_CCER_CC1E;

    // 5 TIM4_CH2 输入捕获设置
    // 5.1 设置 TIM4_CH2 为输入捕获模式,并设置 IC2映射在TI1上 (CCMR1.CC2S=10)
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    // 5.2 设置 TIM4_CH2 为下降沿触发捕获事件 (CCER.CC2P=1)
    TIM4->CCER |= TIM_CCER_CC2P;
    // 5.3 设置不进行 IC2 预分频 (CCMR1.IC2PSC=00 默认)
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    // 5.4 使能 TIM4_CH2 输入捕获 (CCER.CC2E=1)
    TIM4->CCER |= TIM_CCER_CC2E;

    // 6 TIM4_CH2 触发输入设置 (从模式)
    // 6.1 设置触发输入信号(TRGI)选择TI1FP1 (SMCR.TS=101)
    TIM4->SMCR |= TIM_SMCR_TS_2;
    TIM4->SMCR &= ~TIM_SMCR_TS_1;
    TIM4->SMCR |= TIM_SMCR_TS_0;
    // 6.2 设置 TIM4_CH2 接收到触发信号后,复位计数器 (从模式选择:SMCR.SMS=100)
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~TIM_SMCR_SMS_1;
    TIM4->SMCR &= ~TIM_SMCR_SMS_0;

}

void Dri_TIM4_Start(void)
{
    // 开启计数器使能
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Dri_TIM4_Stop(void)
{
    // 关闭计数器使能
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

double Dri_TIM4_CH1_GetPWMPeriod(void)
{
    // TIM4->CCR1 的值为 us ,换算为 ms (需要 / 1000.0)
    return ((TIM4->CCR1 + 1) / 1000.0);
}

double Dri_TIM4_CH1_GetPWMFreq(void)
{

    // TIM4->CCR1 的值为 us ,换算为 s (需要 / 1000000.0)
    return (1000.0 / Dri_TIM4_CH1_GetPWMPeriod());
}

double Dri_TIM4_CH2_GetPWMDuty(void)
{
    return ((TIM4->CCR2 + 1.0) / (TIM4->CCR1 + 1.0));
}
