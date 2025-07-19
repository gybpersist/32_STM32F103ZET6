#include "Dri_TIM4.h"
#include "Dri_USART1.h"

void Dri_TIM4_Init(void)
{
    // 1 ʱ��ʹ��
    // 1.1 GPIOB ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 TIM4 ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // 2 GPIO ����
    // 2.1 TIM4_CH1(PB6) Ϊ��������ģʽ (MODE:00,CNF:01 Ĭ��)
    // GPIOB->CRL &= ~GPIO_CRL_MODE6;
    // GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    // GPIOB->CRL |= GPIO_CRL_CNF6_0;
    
    // 3 TIM4 ʱ����Ԫ����
    // 3.1 ���� CNT �ļ���ģʽ:���ϼ��� (Ĭ��)
    // TIM4->CR1 &= ~TIM_CR1_DIR;
    // 3.2 ���� PSC Ԥ��Ƶ��ֵ ,ʵ��ÿ 1us ����һ�� [�ɲ���Ƶ��: f < 1MHZ ��PWM����]
    TIM4->PSC = 71;
    // 3.3 ���� ARR �Զ���װ��ֵ ,����Ϊ���ֵ [�ɲ���Ƶ��: f > 16HZ ��PWM����]
    TIM4->ARR = 0xFFFF;

    // 4 TIM4 ���벶������
    // 4.1 ���� TIM4_CH1 Ϊ���벶��ģʽ,������ IC1ӳ����TI1�� (CCMR1.CC1S=01) 
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    // 4.2 ���� TI1 ѡ�� TIM4_CH1 ���� (CR2.TI1S=0 Ĭ��)
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 4.3 ���ò����������˲� (CCMR1.IC1F=0000 Ĭ��)
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 4.4 ����Ϊ�����ش��������¼� (CCER.CC1P=0 Ĭ��)
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 4.5 ���ò����� IC1 Ԥ��Ƶ (CCMR1.IC1PSC=00 Ĭ��)
    TIM4->CCMR1 &=~ TIM_CCMR1_IC1PSC;
    // 4.6 ʹ�� TIM4_CH1 ���벶�� (CCER.CC1E=1)
    TIM4->CCER |= TIM_CCER_CC1E;

    // 5. TIM4 �ж����� --------------------------
    // ���� TIM5_CH1 �����벶���ж�ʹ�ܣ�DIER.CC1IE=1��
    TIM4->DIER |= TIM_DIER_CC1IE;
    
    // 6 NVIC ����
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn,6);
    NVIC_EnableIRQ(TIM4_IRQn);

}

void Dri_TIM4_Start(void)
{
    // ����������ʹ��
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Dri_TIM4_Stop(void)
{
    // �رռ�����ʹ��
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

double Dri_TIM4_CH1_GetPWMPeriod(void)
{
    // TIM4->CCR1 ��ֵΪ us ,����Ϊ ms (��Ҫ / 1000.0)
    return ((TIM4->CCR1 + 1) / 1000.0);
}

double Dri_TIM4_CH1_GetPWMFreq(void)
{

    // TIM4->CCR1 ��ֵΪ us ,����Ϊ s (��Ҫ / 1000000.0)
    return (1000.0 / Dri_TIM4_CH1_GetPWMPeriod());
}

// ���� TIM4 ���жϷ������
void TIM4_IRQHandler(void){
    // ��0 ������ CNT ��ֵ
    TIM4->CNT = 0;

    // ����жϱ�־λ (���� ��TIMx_CCR1�塯0��)
    TIM4->SR &= ~TIM_SR_CC1IF;
}
