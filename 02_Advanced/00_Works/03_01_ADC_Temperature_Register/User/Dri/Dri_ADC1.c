#include "Dri_ADC1.h"

void Dri_ADC1_Init(void)
{
    // 1 时钟使能
    // // 1.1 GPIOC 时钟使能
    // RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 1.2 ADC1 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 1.3 ADCLK 预分频设置 ,进行 6 分频 为 12MHZ (ADCLK<=14MHZ) ((RCC_CFGR.ADCPRE=10))
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 2 GPIO 配置
    // 2.1 设置 ADC1_IN10（PC0）为模拟输入（MODE=00,CNF=00）
    // GPIOC->CRL &= ~GPIO_CRL_MODE0;
    // GPIOC->CRL &= ~GPIO_CRL_CNF0_0;

    // 3 ADC1 配置
    // 3.1 设置采样时间为 239.5 个ADC周期 (SMPR1.SMP16=111)
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_2;   //SMPR1:Sample Time Register 1 -> 采样时间寄存器 1
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_1;
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_0;
    // 3.2 设置规则通道组的长度为 1 (SQR1.L=0000)
    ADC1->SQR1 &= ~ADC_SQR1_L;          //SQR1:Regular Sequence Register 1 -> 规则序列寄存器 1
    // 3.3 设置 ADC1_IN16 为通道 1 位置 (SQR1.SQ1=10000)
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= (16 << 0);
    // 3.4 设置关闭扫描模式 (CR1.SCAN=0)
    ADC1->CR1 &= ~ADC_CR1_SCAN;     //SCAN:Scan Mode -> 扫描模式
    // 3.5 设置开启连续模式 (CR2.CONT=1)
    ADC1->CR2 |= ADC_CR2_CONT;      //CONT:Continuous Conversion Mode -> 连续转换模式
    // 3.6 设置数据寄存器的对齐方式为:右对齐 (CR2.ALIGN=0)
    ADC1->CR2 &= ~ADC_CR2_ALIGN;    //ALIGN:Data Alignment -> 数据对齐
    // 3.7 设置 ADC 温度传感器使能
    ADC1->CR2 |= ADC_CR2_TSVREFE;
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
    while(!(ADC1->SR & ADC_SR_EOC))
    {
        // 等待转换结束
    }
}

uint16_t Dri_ADC1_GetValue(void)
{
    return ADC1->DR;
}

double Dri_ADC1_GetValue_Vol(void)
{
    return ( Dri_ADC1_GetValue() / 4095.0 * 3.3 );
}

double Dri_ADC1_GetValue_Temp(void)
{
    double V25 = 1.43;
    double Avg_Slope = 4.3 / 1000;
    return (V25 - Dri_ADC1_GetValue_Vol()) / Avg_Slope + 25;
}
