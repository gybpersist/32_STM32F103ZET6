#include "Dri_I2C.h"

/**
 * @brief I2C初始化
 *
 */
void Dri_I2C_Init(void)
{
    // 1. 对 GPIOB 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. SCL（PB10）设置为通用开漏输出, MODE=11；CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;

    // 3. SDA（PB11）设置为通用开漏输出，MODE=11；CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;

    // 4. 让SCL和SDA处于空闲状态
    SCL_HIGH;
    SDA_HIGH;
}

/**
 * @brief 发送起始信号
 *
 */
void Dri_I2C_Start(void)
{
    // 拉高 SCL
    SCL_HIGH;

    // SDA先拉高再拉低
    SDA_HIGH;
    SCL_DELAY;
    SDA_LOW;

    // 拉低 SCL 并延时
    SCL_LOW;
    SCL_DELAY;
}

/**
 * @brief 发送一个字节
 *
 * @param byte 要发送的字节
 */
void Dri_I2C_TransmitByte(uint8_t byte)
{
    // 循环逐bit发送，先发高位
    for (uint8_t i = 0; i < 8; i++)
    {
        // 判断如果当前byte最高位是1，拉高SDA
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        // 如果当前byte最高位是0，拉低SDA
        else
        {
            SDA_LOW;
        }

        // byte 左移
        byte <<= 1;

        // 拉高SCL，并延时，让接收方采样
        SCL_HIGH;
        SCL_DELAY;

        // 拉低SCL，并延时, 方便下一次可以直接修改SDA
        SCL_LOW;
        SCL_DELAY;
    }
}

/**
 * @brief 接收应答信号
 *
 * @return uint8_t 返回接收到的应答信号，0:ACK; 1:NACK
 */
uint8_t Dri_I2C_ReceiveACK(void)
{
    // 主设备释放SDA
    SDA_HIGH;

    // 拉高SCL，读取SDA状态, 并延时
    SCL_HIGH;
    uint8_t ack = SDA_READ ? 1 : 0;
    SCL_DELAY;

    // 拉低SCL，并延时, 方便下一次可以直接修改SDA
    SCL_LOW;
    SCL_DELAY;

    // 返回
    return ack;
}

/**
 * @brief 接收一个字节
 * 
 * @return uint8_t 接收到的字节
 */
uint8_t Dri_I2C_ReceiveByte(void)
{
    // 定义变量，用于接收数据
    uint8_t byte = 0x00;

    // 主设备释放SDA
    SDA_HIGH;

    // 循环逐bit接收，先接收高位
    for (uint8_t i = 0; i < 8; i++)
    {
        // byte先左移1位，将最低位空出来
        byte <<= 1;

        // 拉高SCL
        SCL_HIGH;

        // 读 SDA 的状态， 如果SDA是高电平，向byte最低位写入1; 如果SDA是低电平，不处理，byte当前最低位默认是0
        if (SDA_READ)
        {
            byte |= 0x01;
        }

        SCL_DELAY;

        // 拉低SCL并演示
        SCL_LOW;
        SCL_DELAY;
    }

    // 返回
    return byte;
}

/**
 * @brief 回复应答信号
 * 
 * @param ack 应答信号，0:ACK; 1:NACK
 */
void Dri_I2C_TransmitACK(uint8_t ack)
{
    if (ack == 0)
    {
        SDA_LOW;        // 发送ACK信号
    }
    else
    {
        SDA_HIGH;       // 发送NACK信号
    }

    // 拉高SCL并延时，让从设备读取状态
    SCL_HIGH;
    SCL_DELAY;

    // 拉低SCL并延时
    SCL_LOW;
    SCL_DELAY;
}

/**
 * @brief 发送停止信号(在SCL高电平期间，SDA产生上升沿)
 *
 */
void Dri_I2C_Stop(void)
{
    // 拉高SCL之前先拉低SDA
    SDA_LOW;

    // 拉高 SCL
    SCL_HIGH;
    SCL_DELAY;

    // 在SCL高电平期间，拉高SDA
    SDA_HIGH;
}
