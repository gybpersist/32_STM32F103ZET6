#include "Dri_DMA.h"

void Dri_DMA_Init(void)
{
    // 1 DMA1 时钟配置
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2 DMA1_Channel4 配置
    // 2.1 设置 DMA1_Channel4 数据传输方向,0从外设读.1从存储器读 (CCR.DIR)
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;
    // 2.2 设置 DMA1_Channel4 存储器数据位宽 8 位 (CCR.MSIZE=00);
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    // 2.3 设置 DMA1_Channel4 外设数据位宽 8 位 (CCR.PSIZE=00)
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;
    // 2.4 设置 DMA1_Channel4 存储器地址增量模式开启 (CCR.MINC=1)
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;
    // 2.5 设置 DMA1_Channel4 外设地址增量模式关闭 (CCR.PINC=0)
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;
    // 2.6 设置开启传输完成 TC 中断
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    // 3 USART1 使能 DMA 发送功能
    // 3.1 USART1 将 DMA 接收到的数据用于发送
    USART1->CR3 |= USART_CR3_DMAT;
    
    // 4 NVIC 配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void Dri_DMA_Transfer(uint32_t mem_addr, uint32_t periph_addr, uint16_t len)
{
    // 1 先关闭 DMA1_Channel4 通道使能 (CCR.EN=0) ,为了设置 CNDTR 寄存器
    DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    Com_Delay_ms(100);      // 保证充分

    // 2 设置存储器地址
    DMA1_Channel4->CMAR = mem_addr;

    // 3 设置外设地址
    DMA1_Channel4->CPAR = periph_addr;

    // 4 设置传输长度 在 DMA 通道使能状态下，CNDTR 是只读的，无法进行修改
    DMA1_Channel4->CNDTR = len;

    // 5 开启 DMA1_Channel4 通道使能 (CCR.EN=1)
   DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

// 定义 DMA1_Channel4 TC 中断触发后调用的弱函数
__weak void Dri_DMA_TC_Callback(void){
}

/**
 * @brief 定义 DMA1_Channel4 中断服务函数
 * 
 */
void DMA1_Channel4_IRQHandler(void)
{
    // 1 清除中断标志位 TCIF
    DMA1->IFCR |= DMA_IFCR_CTCIF4;

    // 2 调用弱函数
    Dri_DMA_TC_Callback();
}
