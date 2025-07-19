#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_TIM6.h"
#include "Dri_TIM5.h"
#include "Dri_TIM3.h"
#include "Int_Key.h"

uint8_t g_pwm_duty = 0; //��¼ TIM5_CH2 ��� PWM ��ռ�ձ�
uint8_t g_pwm_duty_dir = 0; //����ռ�ձȵ���/�ݼ���־

// ������
int main()
{
	// ������ʼ��
	Int_Key_Init();

	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 GeneralYimer TIM3 LCD Example: \n");

	// ��ʼ��LED
	Int_LED_Init();

	// ��ʼ�� TIM6 ,ÿ�� 100ms ����һ�������¼�
	Dri_TIM6_Init(100);
	Dri_TIM5_Init();
	Dri_TIM5_Start();


	// ���� ��ʱ ,ÿ�� 10ms ����һ�������ж�
	Dri_TIM6_Start();

	// ��ʼ�� TIM5 ,Ĭ������T=1ms
	Dri_TIM3_Init();
	Dri_TIM3_Start();
	Dri_TIM3_CH3_SetPWMDuty(g_pwm_duty); //���ó�ʼռ�ձ�

	// ��ѭ��
	while (1)
	{
	}
		
}

/**
 * @brief �ض��尴��SW3��������
 * 
 */
void Int_Key_SW3Callback(void){
	if (g_pwm_duty < 100)
	{
		g_pwm_duty += 10;

		printf("g_pwm_duty:%d",g_pwm_duty);

		Dri_TIM3_CH3_SetPWMDuty(g_pwm_duty); 
		Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty);
	}
	
}

/**
 * @brief �ض��尴��SW4��������
 *  
 */
void Int_Key_SW4Callback(void){
	if (g_pwm_duty > 0)
	{
		g_pwm_duty -= 10;
		printf("g_pwm_duty:%d",g_pwm_duty);

		Dri_TIM3_CH3_SetPWMDuty(g_pwm_duty); 
		Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty);
	}
}

 // �ض���������,���������жϻᱻ����
// void Dri_TIM6_UpdateCallback(void)
// {
// 	if (g_pwm_duty > 100)
//     {
//         g_pwm_duty = 100;
//     }
// 	if (g_pwm_duty < 0)
//     {
//         g_pwm_duty = 0;
//     }
//	
// 	printf("g_pwm_duty:%d",g_pwm_duty);

// 	Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty); //���ϸ��� TIM5 ����� PWM ����ռ�ձ�

// 	// �����ٽ��,�ı�仯����
// 	if (g_pwm_duty == 0 || g_pwm_duty == 100 )
// 	{
// 	   g_pwm_duty_dir = !g_pwm_duty_dir;
// 	}
// }
