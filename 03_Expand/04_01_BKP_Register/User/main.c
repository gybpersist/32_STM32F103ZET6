#include "Dri_USART1.h"
#include "Dri_BKP.h"
#include "Int_Key.h"

// 主函数
int main()
{
    // USART1 初始化
    Dri_USART1_Init();

    // BKP 初始化
    Dri_BKP_Init();

    // 按键初始化
    Int_Key_Init();

    // 打印信息
    printf("STM32 BKP 备份寄存器 Example: \n");

    // 向 BKP 寄存器写入数据
    // BKP->DR1 = 12345;

    // 死循环
    while (1)
    {
    }
}

void Int_Key_SW4Callback(void){
    //  打印 BKP 寄存器的值
    printf("BKP->DR1 = %d \n", BKP->DR1);
}
