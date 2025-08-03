#include "Dri_USART1.h"
#include "Dri_ADC1.h"


// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 ADC1_IN16 Temperature Example: \n");

	// ADC1 初始化
    Dri_ADC1_Init();
    // 启动ADC1转换
    Dri_ADC1_Start();

	// 死循环
	while (1)
	{
		printf("ADC1_IN10_Value: %d ,Vol: %.2lf V,Temp: %.2lf C\n",Dri_ADC1_GetValue(),Dri_ADC1_GetValue_Vol(),Dri_ADC1_GetValue_Temp());
		Com_Delay_ms(1000);
	}
		
}
