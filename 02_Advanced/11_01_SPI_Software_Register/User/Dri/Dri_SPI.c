#include "Dri_SPI.h"

void Dri_SPI_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIO 使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2 GPIO 配置
    // 2.1 SS(PC13) 片选信号线 引脚模式:通用推挽输出 (MODE:11 CNF:00)
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    // 2.2 SCLK(PA5) 时钟信号线 引脚模式:通用推挽输出 (MODE:11 CNF:00)
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    // 2.3 MOSI(PA7) 主机输出从机输入信号线 引脚模式:通用推挽输出 (MODE:11 CNF:00)
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    // 2.4 MISO(PA6) 主机输入从机输出信号线 引脚模式:浮空输入模式 (MODE:00 CNF:01)
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3 初始化
    // 3.1 片选信号拉高
    SS_HIGH;
    // 3.2 时钟信号拉低
    SCLK_LOW;
    // 3.3 延时
    SCLK_DELAY;
}

void Dri_SPI_Start(void)
{
    // 1 片选信号拉低
    SS_LOW;
}

void Dri_SPI_Stop(void)
{
    // 1 片选信号拉高
    SS_HIGH;
}

uint8_t Dri_SPI_SwapByte(uint8_t byte)
{
    // 循环8次
    for (uint8_t i = 0; i < 8; i++)
    {
        // 先准备好数据
        if (byte & 0x80)
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        byte <<= 1;

        // 再拉高SCLK(上升沿)接收数据
        SCLK_HIGH;
        SCLK_DELAY;

        // 再读取MISO数据
        if (MISO_READ)
        {
            byte |= 0x01;
        }

        // 再拉低SCLK(下降沿)发送数据 ,并为下次数据做准备
        SCLK_LOW;
        SCLK_DELAY;
    }
    
    return byte;
}
