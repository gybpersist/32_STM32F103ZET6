#include "Dri_DMA.h"

void Dri_DMA_Init(void)
{
    // 1 DMA1 时钟配置
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2 DMA1_Channel5 配置
    // 2.1 设置 DMA1_Channel5 数据传输方向,0从外设读.1从存储器读 (CCR.DIR)
    DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;
    // 2.2 设置 DMA1_Channel5 存储器数据位宽 8 位 (CCR.MSIZE=00);
    DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;
    // 2.3 设置 DMA1_Channel5 外设数据位宽 8 位 (CCR.PSIZE=00)
    DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE;
    // 2.4 设置 DMA1_Channel5 存储器地址增量模式开启 (CCR.MINC=1)
    DMA1_Channel5->CCR |= DMA_CCR5_MINC;
    // 2.5 设置 DMA1_Channel5 外设地址增量模式关闭 (CCR.PINC=0)
    DMA1_Channel5->CCR &= ~DMA_CCR5_PINC;
    // 2.6 设置开启传输完成 TC 中断
    DMA1_Channel5->CCR |= DMA_CCR5_TCIE;
    // 2.7 设置开启循环模式（CCR.CIRC=1）
    DMA1_Channel5->CCR |= DMA_CCR5_CIRC;


    // 3 USART1 使能 DMA
    // 3.1 USART1 串口接收到的数据通过 DMA 再进行输出
    USART1->CR3 |= USART_CR3_DMAR;
    
    // 4 NVIC 配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel5_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

void Dri_DMA_Transfer(uint32_t mem_addr, uint32_t periph_addr, uint16_t len)
{
    // 1 先关闭 DMA1_Channel5 通道使能 (CCR.EN=0) ,为了设置 CNDTR 寄存器
    DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
    Com_Delay_ms(100);      // 保证充分

    // 2 设置存储器地址
    DMA1_Channel5->CMAR = mem_addr;

    // 3 设置外设地址
    DMA1_Channel5->CPAR = periph_addr;

    // 4 设置传输长度
    DMA1_Channel5->CNDTR = len;

    // 5 开启 DMA1_Channel5 通道使能 (CCR.EN=1)
   DMA1_Channel5->CCR |= DMA_CCR5_EN;
}

// 定义 DMA1_Channel5 TC 中断触发后调用的弱函数
__weak void Dri_DMA_TC_Callback(void){
}

/**
 * @brief 定义 DMA1_Channel5 中断服务函数
 * 
 */
void DMA1_Channel5_IRQHandler(void)
{
    // 1 清除中断标志位 TCIF
    DMA1->IFCR |= DMA_IFCR_CTCIF5;

    // 2 调用弱函数
    Dri_DMA_TC_Callback();
}
