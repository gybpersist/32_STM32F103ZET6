#include "Dri_SPI1.h"

void Dri_SPI1_Init(void)
{
    // 1 时钟使能
    // 1.1 使能 GPIOA 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 使能 GPIOC 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 1.3 使能 SPI1 时钟
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2 GPIO 配置
    // 2.1 配置 SS(PC13),SCK(PA5) 为通用推挽输出 (CNF=00 , MODE=11)
    GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    // 2.2 配置 SCK(PA5),MOSI(PA7) 为复用推挽输出 (CNF=10 , MODE=11)
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF7_0);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    // 2.3 配置 MISO(PA6) 为浮空输入模式 (CNF=01, MODE=00)
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE6;

    // 3 拉高片选信号
    SS_HIGH;

    // 4 初始化 SPI1
    // 4.1 设置 SPI1 的时钟波特率 为 18MHz (4分频)
    SPI1->CR1 &= ~SPI_CR1_BR_2;
    SPI1->CR1 &= ~SPI_CR1_BR_1;
    SPI1->CR1 |= SPI_CR1_BR_0;
    // 4.2 选择时钟极性 CPOL=0 时钟极性为低电平
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    // 4.3 选择时钟相位 CPHA=0 时钟相位为第一个时钟沿
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    // 4.4 选择 8 位数据模式
    SPI1->CR1 &= ~SPI_CR1_DFF;
    // 4.5 设置帧格式位先 MSB
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // 4.6 设置 SSM SSI
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;
    // 4.7 设置为主模式
    SPI1->CR1 |= SPI_CR1_MSTR;
    // 4.8 使能 SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}

void Dri_SPI1_Start(void)
{
    // 拉低片选信号
    SS_LOW;
}

void Dri_SPI1_Stop(void)
{
    // 拉高片选信号
    SS_HIGH;
}

uint8_t Dri_SPI1_SwapByte(uint8_t byte)
{
    // 1 发送数据
    SPI1->DR = byte;
    // 2 等待发送完成
    while ((SPI1->SR & SPI_SR_TXE) == 0);
    // 3 等待接收完成
    while ((SPI1->SR & SPI_SR_RXNE) == 0);
    byte = SPI1->DR;
    // 4 返回接收数据
    return byte;
}
