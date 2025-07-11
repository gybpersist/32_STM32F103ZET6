#include "Dri_USART1.h"

void Dri_USART1_Init(void)
{
    // 1 时钟使能，对GPIOA、USART1 时钟使能 ---------------------------------------->
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

    // 2 设置 GPIOA 的引脚 -------------------------------------------------------->
    // 2.1 TX引脚(PA9)设置为复用推挽输出 MODE=11,CNF=10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 RX引脚(PA10)设置为浮空输入或者复用推挽输出 MODE=00,CNF=01  (默认值)
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3 设置 USART1 -------------------------------------------------------------->
    // 3.1 使能整个 USART1 外设
    USART1->CR1 |= USART_CR1_UE;
    // 3.2 使能 发送使能TXD
    USART1->CR1 |= USART_CR1_TE;
    // 3.3 使能 接收使能RXD
    USART1->CR1 |= USART_CR1_RE;
    // 3.4 设置数据帧 -> 数据位长度 8 位 (默认值)
    USART1->CR1 &= ~USART_CR1_M;
    // 3.5 设置数据帧 -> 无校验位 (默认值)
    USART1->CR1 &= ~USART_CR1_PCE; 
    // 3.6 设置数据帧 -> 1停止位 (默认值)          
    USART1->CR2 &= ~USART_CR2_STOP;                          //         f(72MHZ)
    // 3.7 波特率设置 -> 设置19200波特率,求出除数因子:39.0625   //波特率=---------------   
    USART1->BRR = ((39 << 4) + 0001);                        //        16 * 除数因子
}

 /**
 * @brief 单片机发送一个字节的数据
 * 
 * @param byte 要发送的一个字节数据
 */
static void Dri_USART1_TransmitByte(uint8_t byte)
{
    // 等待发送数据寄存器为空,标志位 TXE 是0就一直循环,直到为1
    while ((USART1->SR & USART_SR_TXE) == 0){
    }

    // 将要发送的字节写入发送数据寄存器
    USART1->DR = byte;
}

void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t len)
{
    // 循环:一位一位的发送
    for (uint16_t i = 0; i < len; i++)
    {
        Dri_USART1_TransmitByte(bytes[i]);
    }
}

uint8_t Dri_USART1_ReceiveByte(void)
{
    // 等待接收数据寄存器为非空,标志位 RXNE 是0就一直循环,直到为1
    while ((USART1->SR & USART_SR_RXNE) == 0){  
    }

    // 返回一个字节数据
    return USART1->DR;
}

void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *len)
{
    *len = 0; //初始化

    // 循环接收多个字节,接收完成退出循环
    while (1)
    {
        // 等待接收完成当前字节
        while ((USART1->SR & USART_SR_RXNE) == 0){
            // 如果在接收一个字节期间,检测到空闲状态,直接结束循环
            if (USART1->SR & USART_SR_IDLE) //USART_SR_IDLE [1:检测到空闲总线]
            {
                // 清除IDLE标志位,需要读DR
                USART1->DR;
                return;
            }
        }
        // 把从DR接收到的数据放到bytes中
        bytes[*len] = USART1->DR;
        (*len)++;
    }
}
