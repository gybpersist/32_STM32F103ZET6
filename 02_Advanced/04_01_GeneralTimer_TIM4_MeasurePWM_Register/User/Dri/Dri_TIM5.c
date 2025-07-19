#include "Dri_TIM5.h"

void Dri_TIM5_Init(void)
{
    // 1 ʱ��ʹ��
    // 1.1 �� TIM5 ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    // 1.2 �� GPIOA ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2 GPIO ����
    // 2.1 �� TIM5_CH2(PA1) ����Ϊ����������� (MODE:11,CNF:10)
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3 TIM5 ʱ����Ԫ����
    // 3.1 ���� CNT ����ģʽ (Ĭ��:���ϼ���ģʽ)
    TIM5->CR1 &=~ TIM_CR1_CMS;          // �ر�����������ģʽ
    TIM5->CR1 &= ~TIM_CR1_DIR;          // direction(����) ���ϼ���ģʽ
    // 3.2 ���� PSC Ԥ��Ƶֵ ,���� 10us һ����
    TIM5->PSC = 719;
    // 3.3 ���� ARR �Զ���װ��ֵ ,����ÿ 1ms (��100����) ����һ�θ����¼�
    TIM5->ARR = 99;

    // 4 TIM5 ����Ƚ����� Capture/Compare 
    // 4.1 ���� TIM5_CH2 Ϊ����Ƚ�ģʽ (Ĭ��:CCMR1.CC2S==00)
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;     // Selection
    // 4.2 ���� TIM5_CH2 ����Ƚ�ģʽΪ PWMģʽ1 (CCMR1.OC2M=110)
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;    // Output Compare Mode
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;
    // 4.3 ���� TIM5_CH2 �������Ϊ�ߵ�ƽ��Ч (Ĭ��:CCER.CC2P=0)
    TIM5->CCER &= ~TIM_CCER_CC2P;       // Polarity
    // 4.4 ���� TIM5_CH2 ���ʹ�� (CCER.CC2E=1)
    TIM5->CCER |= TIM_CCER_CC2E;
}

void Dri_TIM5_Start(void)
{
    //���� TIM5 ����ʹ��
    TIM5->CR1 |= TIM_CR1_CEN;   // Counter Enable
}

void Dri_TIM5_Stop(void)
{
    //�ر� TIM5 ����ʹ��
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void Dri_TIM5_CH2_SetPWMDuty(uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    // �� CCR2 ��ֵ ,����ռ�ձ�
    TIM5->CCR2 = duty;
}
