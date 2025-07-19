#include "Dri_USART1.h"
#include "Int_LED.h"
#include "Dri_TIM6.h"
#include "Dri_TIM5.h"

uint8_t g_pwm_duty = 0; //��¼ TIM5_CH2 ��� PWM ��ռ�ձ�
uint8_t g_pwm_duty_dir = 0; //����ռ�ձȵ���/�ݼ���־

// ������
int main()
{
	// USART1 ��ʼ��
	Dri_USART1_Init();
	// ��ӡ��Ϣ
	printf("STM32 GeneralYimer TIM5 Example: \n");

	// ��ʼ��LED
	Int_LED_Init();

	// ��ʼ�� TIM6 ,ÿ�� 100ms ����һ�������¼�
	Dri_TIM6_Init(100);

	// ���� ��ʱ ,ÿ�� 10ms ����һ�������ж�
	Dri_TIM6_Start();

	// ��ʼ�� TIM5 ,Ĭ������T=1ms
	Dri_TIM5_Init();
	Dri_TIM5_Start();
	Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty); //���ó�ʼռ�ձ�
	Dri_TIM5_CH1_SetPWMDuty(g_pwm_duty); 
	// ��ѭ��
	while (1)
	{
	}
		
}

// �ض���������,���������жϻᱻ����
void Dri_TIM6_UpdateCallback(void)
{
	if (g_pwm_duty_dir == 0)
	{
		g_pwm_duty += 10; //ռ�ձȵ���
	}
	if (g_pwm_duty_dir == 1)
	{
		g_pwm_duty -= 10; //ռ�ձȵݼ�
	}
	printf("g_pwm_duty:%d",g_pwm_duty);

	Dri_TIM5_CH2_SetPWMDuty(g_pwm_duty); //���ó�ʼռ�ձ�
	Dri_TIM5_CH1_SetPWMDuty(g_pwm_duty); 

	// �����ٽ��,�ı�仯����
	if (g_pwm_duty == 0 || g_pwm_duty == 100 )
	{
		g_pwm_duty_dir = !g_pwm_duty_dir;
	}
	

}
