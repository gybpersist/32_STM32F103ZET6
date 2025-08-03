#include "Dri_IWDG.h"

void Dri_IWDG_Init(void)
{
    // 1 写入 Key,允许写入 IWDG_PR 和 IWDG_RLR 寄存器
    IWDG->KR = 0x5555;

    // 2 设置 IWDG 预分频系数为 64 ,实现每 1.6ms 计一个数 (IWDG.PR=100)
    IWDG->PR = 0x0004;

    // 3 设置 IWDG 重装载寄存器为 2500 -1, 实现 4s 之后复位
    IWDG->RLR = 2500 - 1;

    // 4 写入 Key, 使能启动 IWDG 独立看门狗
    IWDG->KR = 0xCCCC;
}

void Dri_IWDG_Refresh(void)
{
    // 写入 Key, 刷新 IWDG 独立看门狗
    IWDG->KR = 0xAAAA;
}
