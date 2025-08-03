#include "Dri_SPI1.h"

void Dri_SPI1_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIO 使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 2.2 SPI1 使能
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2 GPIO 配置
    // 2.1 SS(PC13) 片选信号线 引脚模式:通用推挽输出 (MODE:11 CNF:00)
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    // 2.2 SCLK(PA5) 时钟信号线 引脚模式:复用推挽输出 (MODE:11 CNF:10)
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    // 2.3 MOSI(PA7) 主机输出从机输入信号线 引脚模式:复用推挽输出 (MODE:11 CNF:10)
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
    // 2.4 MISO(PA6) 主机输入从机输出信号线 引脚模式:浮空输入模式 (MODE:00 CNF:01)
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3 初始化
    // 3.1 片选信号拉高
    SS_HIGH;

    // 4 SPI1 配置
    // 4.1 SPI1 时钟频率 :18MHZ 需要4分频（CR1.BR=001）
    SPI1->CR1 &= ~SPI_CR1_BR_2;
    SPI1->CR1 &= ~SPI_CR1_BR_1;
    SPI1->CR1 |= SPI_CR1_BR_0;
    // 4.2 SPI1 时钟极性:0 (CPOL=0,默认) 时钟相位:0 (CPHA=0,默认)
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    // 4.3 设置 SPI1 的数据格式为8位 (CR1.DFF=0,默认)
    SPI1->CR1 &= ~SPI_CR1_DFF;
    // 4.4 设置 SPI1 的高位先行 (CR1.LSBFIRST=0,默认)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // 4.5 强制设置 NSS 为高电平 不受外部控制 (CR1.SSM=1,CR1.SSI=1)
    SPI1->CR1 |= SPI_CR1_SSM; // 可以通过SSI控制位决定NSS的高低电平
    SPI1->CR1 |= SPI_CR1_SSI; // 强制设置为高电平
    // 4.6 设置 SPI1 为主模式 (CR1.MSTR=1)
    SPI1->CR1 |= SPI_CR1_MSTR;
    // 4.7 设置 SPI1 使能 (CR1.SPE=1)
    SPI1->CR1 |= SPI_CR1_SPE;
}

void Dri_SPI1_Start(void)
{
    // 1 片选信号拉低
    SS_LOW;
}

void Dri_SPI1_Stop(void)
{
    // 1 片选信号拉高
    SS_HIGH;
}

uint8_t Dri_SPI1_SwapByte(uint8_t byte)
{
    // 1 等待发送缓冲寄存器为空
    while ((SPI1->SR & SPI_SR_TXE) == 0)
        ;

    // 2 发送数据
    SPI1->DR = byte;

    // 3 等待接收缓冲寄存器非空
    while ((SPI1->SR & SPI_SR_RXNE) == 0)
        ;

    // 4 接收数据
    byte = SPI1->DR;

    return byte;
}
