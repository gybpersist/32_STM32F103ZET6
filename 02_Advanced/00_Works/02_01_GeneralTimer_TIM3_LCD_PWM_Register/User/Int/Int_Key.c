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
    GPIOF->ODR |= GPIO_ODR_ODR8;    //���� PF8 Ϊ����ģʽ
    // 2.2 �� PF9 ����Ϊ����������ģʽ MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE9;
    GPIOF->CRH |= GPIO_CRH_CNF9_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF9_0;
    GPIOF->ODR |= GPIO_ODR_ODR9;    //���� PF9 Ϊ����ģʽ
    // 2.3 �� PF10 ����Ϊ����������ģʽ MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR10;    //���� PF10 Ϊ����ģʽ
    // 2.4 �� PF11 ����Ϊ����������ģʽ MODE=00,CNF=10
    GPIOF->CRH &= ~GPIO_CRH_MODE11;
    GPIOF->CRH |= GPIO_CRH_CNF11_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF11_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR11;    //���� PF11 Ϊ����ģʽ


    // 3 ���� AFIO -----------------------------------------
    // 3.1 EXTI8 ѡ�� PF8,�� AFIO_EXTICR3 �Ĵ����н�������
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;   //����������EXTI8��4��λȫ����Ϊ0
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF; //EXTI8��4��λ����Ϊ 0101 [0101��PF[x]����]
    // 3.2 EXTI9 ѡ�� PF9,�� AFIO_EXTICR3 �Ĵ����н�������
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI9; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PF; 
    // 3.3 EXTI10 ѡ�� PF10,�� AFIO_EXTICR3 �Ĵ����н�������
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;
    // 3.4 EXTI11 ѡ�� PF11,�� AFIO_EXTICR3 �Ĵ����н�������
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI11; 
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PF;

    // 4 ���� EXTI -----------------------------------------
    // 4.1 �����½��ش��� EXTI8 EXTI9
    EXTI->FTSR |= (EXTI_FTSR_TR8 | EXTI_FTSR_TR9);
    // 4.2 ���������ش��� EXTI10 EXTI11
    EXTI->RTSR |= (EXTI_RTSR_TR10 | EXTI_RTSR_TR11);
    // 4.3 ���ÿ��� EXTI8 EXTI9 EXTI10 EXTI11 ���ж�����
    EXTI->IMR |= (EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11);
 

    // 5 ���� NVIC -----------------------------------------
    // 5.1 �������ȼ���4��bitȫ����ռ���ȼ�,�� SCB->AIRCR �Ĵ����еĶ�Ӧ�Ŀ���λ���ó�(3)011,��ʾ4λȫ����ռ���ȼ� (ȫ������)    
    NVIC_SetPriorityGrouping(3);  
    // 5.2 �� EXTI9_5 EXTI15_10 �ж�����ʹ��
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    // 5.3 ���� EXTI9_5 EXTI15_10 �����ȼ�Ϊ 10 11(�������)
    NVIC_SetPriority(EXTI9_5_IRQn,2);
    NVIC_SetPriority(EXTI15_10_IRQn,2);
}

/**
 * @brief ���尴��SW3��������
 * 
 * @return __weak 
 */
__weak void Int_Key_SW3Callback(void){

}

/**
 * @brief ���尴��SW4��������
 * 
 * @return __weak 
 */
__weak void Int_Key_SW4Callback(void){

}

/**
 * @brief ���尴��SW5��������
 * 
 * @return __weak 
 */
__weak void Int_Key_SW5Callback(void){

}

/**
 * @brief ���尴��SW6��������
 * 
 * @return __weak 
 */
__weak void Int_Key_SW6Callback(void){

}

/**
 * @brief EXTI9_5 ���ж������������
 * 
 */
void EXTI9_5_IRQHandler(){
    
    // �ж��Ƿ��� EXTI8 ���͵��ж����� (SW3�Ƿ���) �жϱ�־λ��:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // ����жϱ�־λ (��1�����)
        EXTI->PR |= EXTI_PR_PR8;

        // ͨ����ʱ����
        Com_Delay_ms(10);

        // ��ʱ�����ٴ��ж�PF8�Ƿ��ڵ͵�ƽ
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            // ����������
            Int_Key_SW3Callback();
        }
    }
    // �ж��Ƿ��� EXTI9 ���͵��ж����� (SW4�Ƿ���) �жϱ�־λ��:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // ����жϱ�־λ (��1�����)
        EXTI->PR |= EXTI_PR_PR9;

        // ͨ����ʱ����
        Com_Delay_ms(10);

        // ��ʱ�����ٴ��ж�PF9�Ƿ��ڵ͵�ƽ
        if ((GPIOF->IDR & GPIO_IDR_IDR9) == 0)
        {
            // ����������
            Int_Key_SW4Callback();
        }
        
    }
    
}

/**
 * @brief EXTI15_10 ���ж������������
 * 
 */
void EXTI15_10_IRQHandler(){

    // �ж��Ƿ��� EXTI10 ���͵��ж����� (SW2�Ƿ���) �жϱ�־λ��:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR10)
    {
        // ����жϱ�־λ (��1�����)
        EXTI->PR |= EXTI_PR_PR10;

        // ͨ����ʱ����
        Com_Delay_ms(10);

        // ��ʱ�����ٴ��ж�PF10�Ƿ��ڸߵ�ƽ
        if (GPIOF->IDR & GPIO_IDR_IDR10)
        {
             // ����������
            Int_Key_SW5Callback();
        }     
    }
    // �ж��Ƿ��� EXTI11 ���͵��ж����� (SW2�Ƿ���) �жϱ�־λ��:EXTI_PR
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // ����жϱ�־λ (��1�����)
        EXTI->PR |= EXTI_PR_PR11;

        // ͨ����ʱ����
        Com_Delay_ms(10);

        // ��ʱ�����ٴ��ж�PF11�Ƿ��ڸߵ�ƽ
        if (GPIOF->IDR & GPIO_IDR_IDR11)
        {
            // ����������
            Int_Key_SW6Callback();
        }    
    }
}
