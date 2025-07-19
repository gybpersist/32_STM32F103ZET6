#include "Dri_TIM6.h"

void Dri_TIM6_Init(uint16_t up_ms)
{
    // 1 ʱ��ʹ��
    // �� TIM6 ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2 TIM6 ����
    // 2.1 ʱ����Ԫ����:����Ԥ��Ƶֵ ,ʵ��ÿ 100us ��һ���� 
    TIM6->PSC = 7200 - 1;       // f = 72M / (PSC + 1) = 1000000 / 7200 = 100us
    // 2.2 ʱ����Ԫ����:�����Զ���װ��ֵ
    TIM6->ARR = up_ms * 10- 1;      // ÿ up_ms �������һ�θ����¼�
    // 2.3 �򿪸����ж�ʹ��
    TIM6->DIER |= TIM_DIER_UIE;

    // 3 NVIC ����
    NVIC_SetPriorityGrouping(3);    // ������ռ���ȼ�Ϊ4λ
    NVIC_SetPriority(TIM6_IRQn,5);  // ���� TIM6 �ж����������ȼ�
    NVIC_EnableIRQ(TIM6_IRQn);      // ʹ�� TIM6 �ж�����

}

void Dri_TIM6_Start(void)
{
    // �� TIM6 �ļ���ʹ��
    TIM6->CR1 |= TIM_CR1_CEN;
}

void Dri_TIM6_Stop(void)
{
    // �ر� TIM6 �ļ���ʹ��
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

// ����������,���������жϻᱻ����
__weak void Dri_TIM6_UpdateCallback(void){
}

/**
 * @brief TIM6 ���жϷ������
 * 
 */
void TIM6_IRQHandler(void){

    // ��������¼��жϱ�־λ
    TIM6->SR &= ~TIM_SR_UIF;

    // ����������
    Dri_TIM6_UpdateCallback();
}
