#include "Dri_SPI.h"

void Dri_SPI_Init(void)
{
    // 1 时钟使能
    // 1.1 使能 GPIOA 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 使能 GPIOC 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2 GPIO 配置
    // 2.1 配置 SS(PC13),SCK(PA5),MOSI(PA7) 为通用推挽输出 (CNF=00 , MODE=11)
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    // 2.2 配置 MISO(PA6) 为输入模式 (CNF=01, MODE=00)
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE6;

    // 3 初始化 SPI
    // 3.1 片选信号拉高
    SS_HIGH;
    // 3.2 时钟信号拉低
    SCK_LOW;
    // 3.3 延时
    SCL_DELAY;
}

void Dri_SPI_Start(void)
{
    // 拉低片选信号
    SS_LOW;
}

void Dri_SPI_Stop(void)
{
    // 拉高片选信号
    SS_HIGH;
}

uint8_t Dri_SPI_SwapByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 主设备输出数据信号写入 (先准备好数据,再拉高SCL发送)
        if (byte & 0x80)
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        // 主设备输出数据信号左移
        byte <<= 1; 

        // 时钟信号拉高
        SCK_HIGH;
        // 延时
        SCL_DELAY;

        // 主设备输入数据信号读取
        if (MISO_READ)
        {
            byte |= 0x01;
        }

        // 时钟信号拉低
        SCK_LOW;
        // 延时
        SCL_DELAY;
    }
    return byte;
}
