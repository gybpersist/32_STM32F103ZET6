#include "Int_Key.h"

void Int_Key_Init(void)
{
    // 1 ʱ��ʹ�� -----------------------------------------
    // 1.1 �� GPIOF ����ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 �� AFIO ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2 ���� GPIO ����ģʽ
    // 2.1 �� PF8 ����Ϊ����������ģʽ MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE8;
    GPIOF->CRH |= GPIO_CRH_CNF8_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF8_0;
    // 2.2 ���� PF8 Ϊ����ģʽ
    GPIOF->ODR |= GPIO_ODR_ODR8;

    // 3 ���� AFIO -----------------------------------------
    // EXTI8 ѡ�� PF8,�� AFIO_EXTICR3 �Ĵ����н�������
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;   //����������EXTI8��4��λȫ����Ϊ0
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF; //EXTI8��4��λ����Ϊ 0101

    // 4 ���� EXTI -----------------------------------------
    // 4.1 �����½��ش��� EXTI8
    EXTI->FTSR |= EXTI_FTSR_TR8;
    // 4.2 ���ÿ��� EXTI8 ���ж�����
    EXTI->IMR |= EXTI_IMR_MR8;

    // 5 ���� NVIC -----------------------------------------
    // 5.1 �������ȼ���4��bitȫ����ռ���ȼ�,�� SCB->AIRCR �Ĵ����еĶ�Ӧ�Ŀ���λ���ó�(3)011,��ʾ4λȫ����ռ���ȼ� (ȫ������)    
    NVIC_SetPriorityGrouping(3);  
    // 5.2 �� EXTI9_5 �ж�����ʹ��
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    // 5.3 ���� EXTI9_5 �����ȼ�Ϊ 10(�������)
    NVIC_SetPriority(EXTI9_5_IRQn,10);
}

/**
 * @brief ����������
 * 
 * @return __weak 
 */
__weak void Int_Key_SW3Callback(void){

}

/**
 * @brief EXTI9_5 ���ж������������
 * 
 */
void EXTI9_5_IRQHandler(){
    
    // �ж��Ƿ��� EXTI8 ���͵��ж����� (SW2�Ƿ���) �жϱ�־λ��:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // ����жϱ�־λ (��1�����)
        EXTI->PR |= EXTI_PR_PR8;

        // ͨ����ʱ����
        Com_Delay_ms(10);

        // ��ʱ�����ٴ��ж�PF8�Ƿ��ڵ͵�ƽ
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            // while ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
            // {  //���������ֲ���,û����������
            // }
            
            // ����������
            Int_Key_SW3Callback();
        }
    }
}

