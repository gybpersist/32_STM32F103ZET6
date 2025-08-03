#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_LowPower.h"
#include "Int_Key.h"

// 主函数
int main()
{
    // 按键初始化
    Int_Key_Init();

    // USART1 初始化
    Dri_USART1_Init();

    // LED 初始化
    Int_LED_Init();
    // 点亮 LED1
    Int_LED_On(LED1);
    
    // 打印信息
    printf("STM32 LowPower Stop Example: \n");

    printf("5s stop...\n");
    // 延时 5s
    Com_Delay_ms(5000);
    // 正在睡眠
    printf("stoping...\n");
    Com_Delay_ms(1); // 延时 1ms 确保打印信息完整

    // 进入低功耗的停止模式 (进入之后时钟配置会失效)
    Enable_LowPower_StopMode();

    // 重新配置时钟
    SystemInit();

    // 打印信息
    printf("wakeup...\n");
    printf("闪烁 LED3...\n");

    // 死循环
    while (1)
    {
        // 闪烁 LED3
        Int_LED_Toggle(LED3);
        // 延时 500ms
        Com_Delay_ms(500);
    }
}

