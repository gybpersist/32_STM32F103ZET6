#include "Dri_I2C2.h"

void Dri_I2C2_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIOB 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 I2C2 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2 配置 GPIOB 引脚
    // 2.1 配置 SCL(PB10) 为复用开漏输出 (MODE:11 CNF:11)
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH |= GPIO_CRH_CNF10;
    // 2.2 配置 SDA(PB11) 为复用开漏输出 (MODE:11 CNF:11)
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH |= GPIO_CRH_CNF11;

    // 3 配置 I2C2
    // 3.1 I2C2 软件复位
    I2C2->CR1 |= I2C_CR1_SWRST;
    Com_Delay_ms(10);
    I2C2->CR1 &= ~I2C_CR1_SWRST;
    Com_Delay_ms(10);
    // 3.2 I2C2 模块时钟频率 (APB1的时钟频率,36MHZ)  
    I2C2->CR2 &= ~I2C_CR2_FREQ;
    I2C2->CR2 |= 36;              // 100100
    // 3.3 I2C2 的主模式选项:标准模式的 I2C2  T_SCL=100KHZ 
    I2C2->CCR &= ~I2C_CCR_FS;
    // 3.4 I2C2 的预分频系数    CCR = TPCLK1 / (2 * T_SCL) --> CCR = 36MHZ / (2 * 100KHZ ) = 180
    I2C2->CCR &= ~I2C_CCR_CCR;
    I2C2->CCR = 180;
    // 3.5 I2C2 的最大上升沿时间        1us/(1/36MHZ) + 1= 36 + 1
    I2C2->TRISE &= ~I2C_TRISE_TRISE;
    I2C2->TRISE |= 36 + 1;
    // 3.6 I2C2 使能
    I2C2->CR1 |= I2C_CR1_PE;
}

void Dri_I2C2_Start(void)
{
    // 1 发送起始信号
    I2C2->CR1 |= I2C_CR1_START;

    // 2 等待起始信号发送完成 
    while((I2C2->SR1 & I2C_SR1_SB) == 0)
    ;

    // 3 清除SB标志位 (读取SR1后，后续发送地址清除该位)
    (void)I2C2->SR1;
}

void Dri_I2C2_TransmitSlaveAddr(uint8_t addr)
{
    // 1 发送从机地址
    I2C2->DR = addr;

    // 2 等待从机地址发送完成
    while((I2C2->SR1 & I2C_SR1_ADDR) == 0)
    ;

    // 3 清除 ADDR 标志位 (在软件读取SR1寄存器后，对SR2寄存器的读操作将清除该位)
    (void)I2C2->SR1;
    (void)I2C2->SR2;
}

void Dri_I2C2_TransmitByte(uint8_t byte)
{
    // // 1 发送数据
    // I2C2->DR = byte;

    // // 2 等待发送完成
    // while ((I2C2->SR1 & I2C_SR1_BTF) == 0)
    // {
    // }

    // // 3 清除 BTF 标志位 (在软件读取SR1寄存器后，对数据寄存器的读或写操作将清除该位)
    // (void)I2C2->SR1;
    // (void)I2C2->DR;

    // 1 等待发送数据寄存器位空
    while ((I2C2->SR1 & I2C_SR1_TXE) == 0)
    {
    }

    // 2 发送数据
    I2C2->DR = byte;
}

uint8_t Dri_I2C2_ReceiveACK(void)
{
   uint8_t ACK = (I2C2->SR1 & I2C_SR1_AF) ? 1 : 0;

   // 该位由软件写’0’清除
   I2C2->SR1 &= ~I2C_SR1_AF;

   return ACK;
}

uint8_t Dri_I2C2_ReceiveByte(void)
{
    uint8_t byte;

    // 1 等待接收完成
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0)
    {
    }

    // 2 接收数据
    byte = I2C2->DR;

    // 3 软件对数据寄存器的读写操作清除该位

    return byte;
}

void Dri_I2C2_TransmitACK(uint8_t ack)
{
    if (ack == 0)
    {
        I2C2->CR1 |= I2C_CR1_ACK;
    }
    else
    {
        I2C2->CR1 &= ~I2C_CR1_ACK;
    }
}

void Dri_I2C2_Stop(void)
{
    // 1 发送停止信号
    I2C2->CR1 |= I2C_CR1_STOP;
}
