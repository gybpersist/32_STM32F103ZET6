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
    // USART1 开启接收中断
    Dri_USART1_Receive_IT_Start();
    // LED 初始化
    Int_LED_Init();
    // 点亮 LED1
    Int_LED_On(LED1);
    
    // 打印信息
    printf("STM32 LowPower Sleep Example: \n");

    printf("5s sleep...\n");
    // 延时 5s
    Com_Delay_ms(5000);
    // 正在睡眠
    printf("sleeping...\n");
    // 进入低功耗的睡眠模式
    Enable_LowPower_SleepMode();
    // 打印信息
    printf("wakeup...\n");

    // 死循环
    while (1)
    {
        // 闪烁 LED3
        Int_LED_Toggle(LED3);
        // 延时 500ms
        Com_Delay_ms(500);
    }
}

// 重写接收回调函数 (触发接收中断后会中断调用)
void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
    // 打印接收到的字节数据
    printf("rx_bytes: %s\n",rx_bytes);
}
