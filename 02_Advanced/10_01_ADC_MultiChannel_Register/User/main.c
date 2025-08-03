#include "Dri_USART1.h"
#include "Dri_ADC1.h"


// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	
	// 打印信息
	printf("STM32 ADC1_IN10/IN11 Example: \n");

    // 定义一个数组来存储ADC转换结果
    uint16_t adc_value[2];
	
    // 初始化ADC1
    Dri_ADC1_Init();
    // 初始化ADC1 DMA
    Dri_ADC1_Init_DMA((uint32_t)adc_value);
    // 启动ADC1转换
    Dri_ADC1_Start();

	// 死循环
	while (1)
	{
		printf("\nADC1_IN10_Value: %d ,ADC1_IN12_Value: %d\n",adc_value[0],adc_value[1]);
		printf("ADC1_IN10_Vol: %.2lf V,ADC1_IN12_Vol: %.2lf V\n", adc_value[0] / 4095.0 * 3.3 ,adc_value[1] / 4095.0 * 3.3);
		Com_Delay_ms(1000);
	}
		
}
