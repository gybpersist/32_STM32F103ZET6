#include "Dri_I2C.h"

void Dri_I2C_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIOB 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2 GPIOB 配置
    // 2.1 SCL(PB10) 引脚设置为通用开漏输出 MODE=11；CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;
    // 2.2 SDA(PB11) 引脚设置为通用开漏输出 MODE=11；CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;

    // 3 初始化SCL和SDA为高电平 让SCL和SDA处于空闲状态
    SDA_HIGH;
    SCL_HIGH;
    SCL_DELAY;
}

void Dri_I2C_Start(void)
{
    // 在 SCL 为高电平期间,拉高 SDA 
    // 拉高 SCL
    SCL_HIGH;
    SCL_DELAY;  //确保 SCL 稳定
    // SDA先拉高再拉低,产生一个下降沿
    SDA_HIGH;
    SCL_DELAY;  //确保 SDA 稳定
    SDA_LOW;

    // 拉低 SCL ,为下次数据做准备
    SCL_LOW;
    SCL_DELAY;
}

void Dri_I2C_Stop(void)
{
    // 确保SDA在SCL低电平时为低电平
    SDA_LOW;
    
    // 拉高SCL
    SCL_HIGH;
    SCL_DELAY;  //确保 SCL SDA 稳定
    
    // SDA从低电平切换到高电平，产生停止信号
    SDA_HIGH;
    SCL_DELAY;
}

void Dri_I2C_TransmitByte(uint8_t byte)
{
    // 先准备好 SDA 的数据,在拉高 SCL
    // 发送8位数据 发高位
    for (uint8_t i = 0; i < 8; i++)
    {   
        // 设置SDA为当前位的值
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        // 左移一位，准备发送下一位
        byte <<= 1;
        
        // 拉高SCL，让从设备读取数据
        SCL_HIGH;
        SCL_DELAY;

        // 拉低SCL，为下一次数据做准备
        SCL_LOW;
        SCL_DELAY;
    }
}

uint8_t Dri_I2C_ReceiveAck(void)
{
    uint8_t ACK;

    // 主设备需要先释放 SDA 
    SDA_HIGH;
    // 然后再拉高 SCL 读取 SDA 的状态
    SCL_HIGH;
    SCL_DELAY;

    if (SDA_READ)
    {
        ACK = 1;
    }
    else
    {
        ACK = 0;
    }

    // 拉低 SCL 为下一次数据做准备
    SCL_LOW;
    SCL_DELAY;
    
    return ACK;
}

uint8_t Dri_I2C_ReceiveByte(void)
{
    uint8_t byte = 0;   // 用来存放接收到的数据

    // 主设备需要先释放 SDA 
    SDA_HIGH;

    // 循环接收 从高位接收
    for (uint8_t i = 0; i < 8; i++)
    {
        // 先拉高 SCL 读取 SDA 的状态
        SCL_HIGH;
        SCL_DELAY;

        byte <<= 1;     // 空出最低位 ,
        // 读取 SDA 的状态
        if (SDA_READ)
        {
            byte |= 0x01;
        }
    
        // 拉低 SCL，为下一次数据做准备
        SCL_LOW;
        SCL_DELAY;
    }

    return byte;
}

void Dri_I2C_TransmitACK(uint8_t ACK)
{
    if (ACK)
    {
        SDA_HIGH;
    }
    else
    {
        SDA_LOW;
    }
    // 拉高 SCL 让从设备读取数据
    SCL_HIGH;
    SCL_DELAY;
    // 拉低 SCL 为下一次数据做准备
    SCL_LOW;
    SCL_DELAY;
}
