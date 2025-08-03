#include "Dri_USART1.h"
#include "Dri_IWDG.h"
#include "Int_Key.h"

// 主函数
int main()
{
    // USART1 初始化
    Dri_USART1_Init();

    // 按键初始化
    Int_Key_Init();
    
    // 打印信息
    printf("STM32 IWDG Register Example: \n");

    // 初始化 IWDG
    Dri_IWDG_Init();
    
    // 死循环
    while (1)
    {
        // 刷新 IWDG 独立看门狗 (喂狗)
        Dri_IWDG_Refresh();     // 4s 未喂狗, 会复位
        
        // 打印喂狗信息
        printf("IWDG Refreshed!\n");

        // 延时 2s
        Com_Delay_ms(2000);
    }
}

void Int_Key_SW4Callback(void){
    printf("SW4 Key Pressed!\n");
    // 人为干扰
    Com_Delay_s(5);
}

