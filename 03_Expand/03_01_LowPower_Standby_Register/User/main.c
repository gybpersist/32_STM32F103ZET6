#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_LowPower.h"

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

    printf("5s standby...");
    // 延时 5s
    Com_Delay_ms(5000);
    // 正在睡眠
    printf("standbying...\n");
    Com_Delay_ms(1); // 延时 1ms 确保打印信息完整

    // 进入低功耗的待机模式 (进入之后 ,后面的程序不会执行 . 唤醒后从开始重新执行)
    Enable_LowPower_StandbyMode();

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

// 重写接收回调函数 (触发接收中断后会中断调用)
void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
    // 打印接收到的字节数据
    printf("rx_bytes: %s\n",rx_bytes);
}
