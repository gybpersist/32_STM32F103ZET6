#include "Dri_I2C2.h"

void Dri_I2C2_Init(void)
{
    // 1 时钟使能 -----------------------------------------
    // 1.1 对 GPIOB 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 对 I2C2 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2 GPIO 引脚设置 ------------------------------------
    // 2.1 PB10(SCL) 复用开漏输出 MODE:11  ,CNF:11
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH |= GPIO_CRH_CNF10;
    // 2.2 PB11(SDA) 复用开漏输出 MODE:11  ,CNF:11
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH |= GPIO_CRH_CNF11;

    // 3 I2C2 配置 ----------------------------------------
    // 3.1 I2C2 软复位(可以省略)
    I2C2->CR1 |= I2C_CR1_SWRST;     // 置1进入复位状态
    Com_Delay_ms(100);              // 延迟100ms
    I2C2->CR1 &= ~I2C_CR1_SWRST;    // 置0退出复位状态
    Com_Delay_ms(100);              // 延迟100ms
    // 3.2 设置 I2C2 模块的输入时钟(APB1的时钟频率,36MHZ)       周期T_pclk1=1/36us
    I2C2->CR2 &= ~I2C_CR2_FREQ;     // 6个位置全部置0
    I2C2->CR2 |= 36;                // 将这6个位置设置为 100100
    // 3.3 设置 I2C2 模块的时钟分频系数                         标准模式： 速度 100kbit/s -> SCL频率:100kHz -> SCL周期：10us
    I2C2->CCR &= ~I2C_CCR_CCR;      // 12个位置全部置0         T_hight=5us T_low=5us 5=CCR*1/36 CCR=180
    I2C2->CCR |= 180;
    // 3.4 设置 I2C2 时钟的上升沿时间,设置为 1000ns=1us         T_pclk1=1/36us 标准模式最大上升沿时间:1us
    I2C2->TRISE = 37;                                      // 1/(1/36) = 36  36+1=37
    // 3.5 启动 I2C2 模块
    I2C2->CR1 |= I2C_CR1_PE;
}

void Dri_I2C2_Start(void)
{
    // 产生起始信号
    I2C2->CR1 |= I2C_CR1_START;

    // 等待起始信号发送完成
    while ((I2C2->SR1 & I2C_SR1_SB) == 0)
    {
    }
}

void Dri_I2C2_TransmitSlaveAddr(uint8_t addr)
{
    // 将从机地址写入数据寄存器
    I2C2->DR = addr;

    // 等待从机地址发送完成
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0)
    {
    }

    // 清除 ADDR 标志位,只需再读 SR2
    I2C2->SR2;
}

void Dri_I2C2_TransmitByte(uint8_t byte)
{
    // 将数据写入数据寄存器
    I2C2->DR = byte;

    // 等待数据发送完成
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0)
    {
    }
}

uint8_t Dri_I2C2_ReceiveACK(void)
{
    // 获取 ack 的状态
    uint8_t ack = (I2C2->SR1 & I2C_SR1_AF) ? 1 : 0;

    // 清除 AF 标志位
    I2C2->SR1 &= ~I2C_SR1_AF;

    return ack;
}

uint8_t Dri_I2C2_ReceiveByte(void)
{
    // 等待接收到一个字节
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0)
    {
    }

    return I2C2->DR;
}

void Dri_I2C2_TransmitACK(uint8_t ack)
{
    if (ack == 0)
    {
        I2C2->CR1 |= I2C_CR1_ACK;   // 回复ACK
    }else
    {
        I2C2->CR1 &= ~I2C_CR1_ACK;  // 回复NACK
    }
    
}

void Dri_I2C2_Stop(void)
{
    // 停止位置1
    I2C2->CR1 |= I2C_CR1_STOP;
}
