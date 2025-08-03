#include "Dri_LowPower.h"

void Enable_LowPower_SleepMode(void)
{
    // // 开启 SLEEP-ON-EXIT 模式 (开启后:唤醒后会执行一次中断服务程序,然后再次进入睡眠模式)
    // SCB->SCR |= SCB_SCR_SLEEPONEXIT;

    // 进入低功耗的睡眠模式 (使用中断唤醒)
    __WFI();
}

void Enable_LowPower_StopMode(void)
{
    // 1 对 PWR 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 2 低功耗配置
    // 2.1 开启深度睡眠模式 (SCB_SCR.SLEEPDEER=1,该寄存器位于内核)
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    // 2.2 设置此深度睡眠模式位 STOP 模式 ,(PWR_CR.PDDS=0,默认)
    PWR->CR &= ~PWR_CR_PDDS;
    // 2.3 设置进入 STOP 模式时,1.8V 供电区使用低功耗模式（PWR_CR.LPDS=1）
    PWR->CR |= PWR_CR_LPDS;

    // 3 进入 STOP 低功耗模式 (使用 EXTI 唤醒)
    __WFI();
}

void Enable_LowPower_StandbyMode(void)
{
   // 1 对 PWR 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 2 低功耗配置
    // 2.1 开启深度睡眠模式 (SCB_SCR.SLEEPDEER=1,该寄存器位于内核)
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    // 2.2 设置此深度睡眠模式位 STANDBY 模式 ,(PWR_CR.PDDS=1)
    PWR->CR |= PWR_CR_PDDS;
    
    // 3 使能 WKUP 引脚
    // PWR->CSR |= PWR_CSR_EWUP;

    // 4 清除 WUF 标志位 (每次被唤醒WUF都会置1) (需要通过置1,才能清除) 
    PWR->CR |= PWR_CR_CWUF;

    // 5 进入 STANDBY 模式
    __WFI();


}
