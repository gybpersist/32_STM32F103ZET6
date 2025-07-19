#include "Dri_TIM3.h"

void Dri_TIM3_Init(void)
{
    // 1 ʱ��ʹ��
    // 1.1 �� TIM3 ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // 1.2 �� GPIOB ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2 GPIO ����
    // 2.1 �� TIM3_CH3(PB0) ����Ϊ����������� (MODE:11,CNF:10)
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL |= GPIO_CRL_CNF0_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    // 3 TIM3 ʱ����Ԫ����
    // 3.1 ���� CNT ����ģʽ (Ĭ��:���ϼ���ģʽ)
    TIM3->CR1 &=~ TIM_CR1_CMS;          // �ر�����������ģʽ
    TIM3->CR1 &= ~TIM_CR1_DIR;          // ���ϼ���ģʽ
    // 3.2 ���� PSC Ԥ��Ƶֵ ,���� 10us һ����
    TIM3->PSC = 719;
    // 3.3 ���� ARR �Զ���װ��ֵ ,����ÿ 1ms (��100����) ����һ�θ����¼�
    TIM3->ARR = 99;

    // 4 TIM3 ����Ƚ�����
    // 4.1 ���� TIM3_CH3  Ϊ����Ƚ�ģʽ (Ĭ��:CCMR1.CC2S==00)
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    // 4.2 ���� TIM3_CH3  ����Ƚ�ģʽΪ PWMģʽ1 (CCMR1.OC2M=110)
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    // 4.3 ���� TIM3_CH3 �������Ϊ�ߵ�ƽ��Ч (Ĭ��:CCER.CC2P=0)
    TIM3->CCER &= ~TIM_CCER_CC3P;
    // 4.4 ���� TIM3_CH3 ���ʹ�� (CCER.CC2E=1)
    TIM3->CCER |= TIM_CCER_CC3E;
}

void Dri_TIM3_Start(void)
{
    //���� TIM3 ����ʹ��
    TIM3->CR1 |= TIM_CR1_CEN; 
}

void Dri_TIM3_Stop(void)
{
    //�ر� TIM3 ����ʹ��
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

// ���� TIM3_CH3 PWM ��ռ�ձ�
void Dri_TIM3_CH3_SetPWMDuty(uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }

    // �� CCR3 ��ֵ ,����ռ�ձ�
    TIM3->CCR3 = duty;
}
