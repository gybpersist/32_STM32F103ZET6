#include "Dri_ADC1.h"
#include "Dri_USART1.h"
void Dri_ADC1_Init(void)
{
    // 1 时钟使能
    // 1.1 GPIOC 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 1.2 ADC1 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 1.3 ADCLK 预分频设置 ,进行 6 分频 为 12MHZ (ADCLK<=14MHZ) ((RCC_CFGR.ADCPRE=10))
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
    // 1.4 使能 DMA 时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2 GPIO 配置
    // 2.1 设置 ADC1_IN10（PC0）为模拟输入（MODE=00,CNF=00）
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;
    // 2.2 设置 ADC1_IN12 (PC2) 为模拟输入（MODE=00,CNF=00）
    GPIOC->CRL &= ~GPIO_CRL_MODE2;
    GPIOC->CRL &= ~GPIO_CRL_CNF2;

    // 3 ADC1 配置
    // 3.1 设置 ADC1_IN10 ADC1_IN12 采样时间为 71.5 个ADC周期 (SMPR1.SMP10=110 SMPR1.SMP12=110)
    ADC1->SMPR1 |= (ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP12_2);   //SMPR1:Sample Time Register 1 -> 采样时间寄存器 1
    ADC1->SMPR1 |= (ADC_SMPR1_SMP10_1 | ADC_SMPR1_SMP12_1);
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_0 | ADC_SMPR1_SMP12_0);
    // 3.2 设置规则通道组的长度（即要转换的通道数量）为 2 (SQR1.L=0001)
    ADC1->SQR1 &= ~ADC_SQR1_L;          //SQR1:Regular Sequence Register 1 -> 规则序列寄存器 1
    ADC1->SQR1 |= ADC_SQR1_L_0;
    // 3.3 设置 ADC1_IN10 为通道 1 位置 (SQR3.SQ1=01010)
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= (10 << 0);
    // 3.4 设置 ADC1_IN12 为通道 2 位置 (SQR3.SQ2=01100)
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= (12 << 5);
    // 3.5 设置开启扫描模式 (CR1.SCAN=1)
    ADC1->CR1 |= ADC_CR1_SCAN;     //SCAN:Scan Mode -> 扫描模式
    // 3.6 设置开启连续模式 (CR2.CONT=1)
    ADC1->CR2 |= ADC_CR2_CONT;      //CONT:Continuous Conversion Mode -> 连续转换模式
    // 3.7 设置数据寄存器的对齐方式为:右对齐 (CR2.ALIGN=0)
    ADC1->CR2 &= ~ADC_CR2_ALIGN;    //ALIGN:Data Alignment -> 数据对齐
    // 3.8 设置 ADC1 开启 DMA 传输模式
    ADC1->CR2 |= ADC_CR2_DMA;

    // 4 DMA 配置
    // 4.1 设置 DMA1_Channel1 数据传输方向,0从外设读.1从存储器读 (CCR.DIR)
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;
    // 4.2 设置 DMA1_Channel1 存储器数据位宽 16 位 (CCR.MSIZE=01)
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
    // 4.3 设置 DMA1_Channel1 外设数据位宽 16 位 (CCR.PSIZE=01)
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    // 4.4 设置 DMA1_Channel1 开启存储器地址自动增加 (CCR.MINC=1)
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;
    // 4.5 设置 DMA1_Channel1 关闭外设地址自动增加 (CCR.PINC=0)
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    // 4.6 设置 DMA1_Channel1 开启循环模式 (CCR.CIRC=1)
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
}

void Dri_ADC1_Start(void)
{
    // 1 唤醒ADC1:当ADON为0时值1
    ADC1->CR2 |= ADC_CR2_ADON;     //ADON:ADC Enable -> ADC使能
    
    // 2 ADC1唤醒之后需要稳定时间, 启动校准 ,当稳定时间过去,校准结束自动置0
    ADC1->CR2 |= ADC_CR2_CAL;      //CAL:Calibration -> 校准
    // 等待校准结束
    while(ADC1->CR2 & ADC_CR2_CAL)
    {
        // 等待校准结束
    }

    // 3 启动ADC1转换:当ADON为1时置1
    ADC1->CR2 |= ADC_CR2_ADON;
    // 等待转换结束
    // while(!(ADC1->SR & ADC_SR_EOC))
    // {
    //     // 等待转换结束
    // }
    // printf("ADC1 转换完成\n");
}

void Dri_ADC1_Init_DMA(uint32_t mem_addr)
{
    // 1 设置 DMA1_Channel1 存储器地址
    DMA1_Channel1->CMAR = mem_addr;

    // 2 设置 DMA1_Channel1 外设地址
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;

    // 3 设置 DMA1_Channel1 传输长度
    DMA1_Channel1->CNDTR = 2;

    // 4 使能 DMA1_Channel1
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

uint16_t Dri_ADC1_GetValue(void)
{
    return ADC1->DR;
}

double Dri_ADC1_GetValue_Vol(void)
{
    return ( Dri_ADC1_GetValue() / 4095.0 * 3.3 );
}
