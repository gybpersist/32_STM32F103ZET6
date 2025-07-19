#include "Dri_USART1.h"

void Dri_USART1_Init(void)
{
    // 1 时钟使能 
    // 1.1 GPIOA 使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 USART1 使能
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2 GPIO 配置
    // 2.1 发送 TXD(PA9)  工作模式:复用推挽输出 MODE:11 CNF:10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 接收 RXD(PA10) 工作模式:浮空输入模式 MODE:00 CNF:01
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3 USART1 配置
    // 3.1 USART1 的数据位数 (8位)
    USART1->CR1 &= ~USART_CR1_M;        // 0:8位 1:9位
    // 3.2 USART1 的校验位数 (无)
    USART1->CR1 &= ~USART_CR1_PCE;      // 0:无  1:有校验
    // 3.3 USART1 的停止位数 (1位)
    USART1->CR2 &= ~USART_CR2_STOP;     // 00:1位 01:0.5位 10:2位 11:1.5位
    // 3.4 USART1 的波特率设置 (19200) DIV=39.0625
    USART1->BRR = ((39 << 4) | 0001);
    // 3.5 TXD 发送使能
    USART1->CR1 |= USART_CR1_TE;
    // 3.6 RXD 接收使能
    USART1->CR1 |= USART_CR1_RE;
    // 3.7 USART1 使能
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * @brief 发送一个字节(8bit)的数据
 * 
 * @param byte 要发送的一个字节(8bit)数据
 */
static void Dri_USART1_TransmitByte(uint8_t byte)
{
    // 等待发送完成 (发送数据寄存器为空) --> 等待变为1(为空)
    while ((USART1->SR & USART_SR_TXE) == 0)
    {}
    
    // 放到数据寄存器上
    USART1->DR = byte;
}

// /**
//  * @brief 接收到一个字节(8bit)的数据
//  * 
//  * @return uint8_t 接收到的字节(8bit)数据
//  */
// static uint8_t Dri_USART1_ReceiveByte(void)
// {
//     // 等待开始接收 (接收数据寄存器非空) --> 等待变为1(非空)
//     while ((USART1->SR & USART_SR_RXNE) == 0)
//     {}
    
//     return USART1->DR;
// }

void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t bytes_len)
{
    // 循环 发送和接收都是低位
    for (uint16_t i = 0; i < bytes_len; i++)
    {
        Dri_USART1_TransmitByte(bytes[i]);
    }
}

void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *bytes_len)
{
     *bytes_len = 0; //初始化

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
        bytes[*bytes_len] = USART1->DR;
        (*bytes_len)++;
    }
}

// 重定义 fputc
int fputc(int ch,FILE *stream){
    // 使用串口将 ch 发送出去
    Dri_USART1_TransmitByte(ch);
    // 返回 ch
    return ch;
}
