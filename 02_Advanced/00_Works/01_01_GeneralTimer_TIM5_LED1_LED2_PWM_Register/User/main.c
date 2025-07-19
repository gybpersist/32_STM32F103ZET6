#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_TIM6.h"
#include "Dri_TIM5.h"

uint8_t g_pwm_duty = 0; //记录 TIM5_CH2 输出 PWM 的占空比
uint8_t g_pwm_duty_dir = 0; //定义占空比递增/递减标志

// 主函数
int main()
{
	// USART1 初始化
	Dri_USART1_Init();
	// 打印信息
	printf("STM32 GeneralYimer TIM5 Example: \n");

	// 初始化LED
	Int_LED_Init();

	// 初始化 TIM6 ,每隔 100ms 产生一个更新事件
	Dri_TIM6_Init(100);

	// 开启 计时 ,每隔 10ms 产生一个更新中断
	Dri_TIM6_Start();

	// 初始化 TIM5 ,默认周期T=1ms
	Dri_TIM5_Init();
	Dri_TIM5_Start();
	Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty); //设置初始占空比
	Dri_TIM5_CH1_SetPWMDuty(g_pwm_duty); 
	// 死循环
	while (1)
	{
	}
		
}

// 重定义弱函数,产生更新中断会被调用
void Dri_TIM6_UpdateCallback(void)
{
	if (g_pwm_duty_dir == 0)
	{
		g_pwm_duty += 10; //占空比递增
	}
	if (g_pwm_duty_dir == 1)
	{
		g_pwm_duty -= 10; //占空比递减
	}
	printf("g_pwm_duty:%d",g_pwm_duty);

	Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty); //设置初始占空比
	Dri_TIM5_CH1_SetPWMDuty(g_pwm_duty); 

	// 到达临界点,改变变化方向
	if (g_pwm_duty == 0 || g_pwm_duty == 100 )
	{
		g_pwm_duty_dir = !g_pwm_duty_dir;
	}
	

}
