#include "Dri_USART1.h"
#include "Dri_RTC.h"

// 主函数
int main()
{
    // USART1 初始化
    Dri_USART1_Init();
    
    // 打印信息
    printf("STM32 RTC Calendar Register Example: \n");

     // 初始化 RTC
    Dri_RTC_Init();
   
    // 设置当前时间
    // DataTime_t datatime = {2025,7,30,11,34,0};
    // Dri_RTC_SetTime(datatime);

    // 死循环
    while (1)
    {
        // 打印当前时间
        DataTime_t datatime = Dri_RTC_GetTime();
        printf("Current Time: %d-%02d-%02d %02d:%02d:%02d\n", datatime.year, datatime.month, datatime.day, datatime.hour, datatime.minute, datatime.second);
        // 延时 1s
        Com_Delay_ms(997);
    }
}
