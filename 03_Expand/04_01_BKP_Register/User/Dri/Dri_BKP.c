#include "Dri_BKP.h"

void Dri_BKP_Init(void)
{
    // 1 时钟使能
    // 1.1 使能 PWR 时钟
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    // 1.2 使能 BKP 时钟
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;

    // 2 使能 BKP 访问 (允许向RTC寄存器和后备寄存器写入数据)
    PWR->CR |= PWR_CR_DBP;

    // // 3 复位备份域
    // RCC->BDCR |= RCC_BDCR_BDRST;
}
