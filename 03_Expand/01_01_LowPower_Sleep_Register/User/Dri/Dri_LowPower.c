#include "Dri_LowPower.h"

void Enable_LowPower_SleepMode(void)
{
    // // 开启 SLEEP-ON-EXIT 模式 (开启后:唤醒后会执行一次中断服务程序,然后再次进入睡眠模式)
    // SCB->SCR |= SCB_SCR_SLEEPONEXIT;

    // 进入低功耗的睡眠模式(使用中断唤醒)
    __WFI();
}
