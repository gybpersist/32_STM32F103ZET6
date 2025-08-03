#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_LowPower.h"
#include "Dri_RTC.h"

// 主函数
int main()
{
    // USART1 初始化
    Dri_USART1_Init();



    // LED 初始化
    Int_LED_Init();
    // 点亮 LED3
    Int_LED_On(LED3);
    
    // 打印信息
    // printf("STM32 LowPower Standby Example: \n");

 	// 对PWR时钟使能
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// 判断 WUF 标志位, 如果WUF为1，说明有待机模式唤醒，否则复位重新上电
	if (PWR->CSR & PWR_CSR_WUF)
	{
		printf("WakeUp From Standby...\n");
		// 清除 WUF 标志位
		PWR->CR |= PWR_CR_CWUF;
	}
	else
	{
		printf("Application Start \n");
	}

    printf("5s later standby...");
    // 延时 5s
    Com_Delay_ms(5000);
    // 正在睡眠
    printf("standbying...\n");
    Com_Delay_ms(1); // 延时 1ms 确保打印信息完整

     // 初始化 RTC
    Dri_RTC_Init();
    // 5s 闹钟
    Dri_RTC_SetAlarm(5);
    // 打印 5s 后唤醒
    printf("5s later wakeup...\n");
    // 进入低功耗的待机模式 (进入之后 ,后面的程序不会执行 . 唤醒后从开始重新执行)
    Enable_LowPower_StandbyMode();

    // 死循环
    while (1)
    {
    }
}
