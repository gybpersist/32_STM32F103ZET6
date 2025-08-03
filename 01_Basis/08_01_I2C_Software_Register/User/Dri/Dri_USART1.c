#include "Dri_USART1.h"

static uint8_t s_rx_bytes[128]; //用来存放接收到的数据
static uint16_t s_rx_len = 0;   //用来计算接收到的数据的长度

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

    // // 4 USART1 开启中断使能 放在 Dri_USART1_Receive_IT_Start(); 函数中
    // USART1->CR1 |= USART_CR1_RXNEIE; //使能 RXNE 中断
    // USART1->CR1 |= USART_CR1_IDLEIE; //使能 IDLE 中断

    // 5 NVIC 设置
    NVIC_SetPriorityGrouping(3); 
    NVIC_SetPriority(USART1_IRQn,10); //给中断向量设置优先级
    NVIC_EnableIRQ(USART1_IRQn); //使能中断向量
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

void Dri_USART1_Receive_IT_Start(void)
{
    // 4 USART1 开启中断使能 
    USART1->CR1 |= USART_CR1_RXNEIE; //使能 RXNE 中断
    USART1->CR1 |= USART_CR1_IDLEIE; //使能 IDLE 中断
}

void Dri_USART1_Receive_IT_Stop(void)
{
    // 4 USART1 关闭中断使能 
    USART1->CR1 &= ~USART_CR1_RXNEIE; 
    USART1->CR1 &= ~USART_CR1_IDLEIE; 
}

/**
 * @brief 定义接收数据的回调函数
 * 
 * @param rx_bytes 接收到的数据保存到这里
 * @param rx_len 接收到的数据的长度
 * @return __weak 
 */
__weak void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
}

/**
 * @brief USART1 的中断服务程序
 * 
 */
void USART1_IRQHandler(void){

    // 判断是否是 RXNE 中断触发的,说明接收到一个字节
    if (USART1->SR &= USART_SR_RXNE)
    {
        s_rx_bytes[s_rx_len] = USART1->DR;
        s_rx_len++;
    }
    
    // 判断总线是否是空闲,说明全部接收完成
    if (USART1->SR & USART_SR_IDLE)
    {
        // 调用弱函数,将接收到数据传出
        Dri_USART1_ReceiveCallback(s_rx_bytes,s_rx_len);
        // 接收长度清零
        s_rx_len = 0;
        // 清除 IDLE 标志位,需要再读一次DR
        USART1->DR;
    }
    
}

// 重新定义 fputc() 函数
int fputc(int ch,FILE *stream){
    // 使用串口将 ch 发送出去
    Dri_USART1_TransmitByte(ch);
    // 返回 ch
    return ch;
}
