#include "Int_Key.h"

void Int_Key_Init(void)
{
    // 1 ʱ��ʹ�� 
    // 1.1 ʹ�� GPIOF
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 ʹ�� AFIO 
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2 GPIO ����
    // 2.1 PF8 PF9 PF10 PF11 ����������ģʽ MODE:00 CNF:10
    GPIOF->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    GPIOF->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
    GPIOF->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
    // 2.2 �� ODR �Ĵ�����ѡ�� ����(1)/����(0)
    GPIOF->ODR |= (GPIO_ODR_ODR8 | GPIO_ODR_ODR9);      // PF8 PF9 ѡ������ 
    GPIOF->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR11);   // PF10 PF11 ѡ������ 

    // 3 AFIO ����
    // 3.1 AFIO ѡ�� PF8 PF9 PF10 PF11
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI8 | AFIO_EXTICR3_EXTI9 | AFIO_EXTICR3_EXTI10 | AFIO_EXTICR3_EXTI11);     // ����0
    AFIO->EXTICR[2] |= (AFIO_EXTICR3_EXTI8_PF | AFIO_EXTICR3_EXTI9_PF | AFIO_EXTICR3_EXTI10_PF | AFIO_EXTICR3_EXTI11_PF);   // ������PF8 PF9 PF10 PF11
    
    // 4 EXTI ����
    // 4.1 ѡ�� ����/�½��� ����
    EXTI->FTSR |= (EXTI_FTSR_TR8 | EXTI_FTSR_TR9);      // PF8 PF9 �½��ش���
    EXTI->RTSR |= (EXTI_RTSR_TR10 | EXTI_RTSR_TR11);      // PF10 PF11 �Ͻ��ش���
    // 4.2 ���� EXTI �ж�
    EXTI->IMR |= (EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11);

    // 5 NVIC ����
    NVIC_SetPriorityGrouping(3);            // ȫ������ռ���ȼ�
    NVIC_SetPriority(EXTI9_5_IRQn,2);       
    NVIC_SetPriority(EXTI15_10_IRQn,2);
    NVIC_EnableIRQ(EXTI9_5_IRQn);           //ʹ�ܿ���
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**************** �����ص����� ****************************/
// ���� SW3 ���������� ,�����жϺ󽫻ᱻ����
__weak void Int_Key_IsSW3Callback(void){
}
// ���� SW4 ���������� ,�����жϺ󽫻ᱻ����
__weak void Int_Key_IsSW4Callback(void){
}
// ���� SW5 ���������� ,�����жϺ󽫻ᱻ����
__weak void Int_Key_IsSW5Callback(void){
}
// ���� SW6 ���������� ,�����жϺ󽫻ᱻ����
__weak void Int_Key_IsSW6Callback(void){
}
/**************** �����ص����� ****************************/

/**************** �жϷ������ ****************************/
// SW3(PF8) SW4(PF9) ���жϷ������
void EXTI9_5_IRQHandler(void)
{
    // �ж��Ƿ��� EXTI8(SW3) ������
    if (EXTI->PR & EXTI_PR_PR8)
    {
        // ��� �����־λ (д�롯1�����������)
        EXTI->PR |= EXTI_PR_PR8;

        // ����
        Com_Delay_ms(10);
        if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
        {
            Int_Key_IsSW3Callback();
        }
    }

     // �ж��Ƿ��� EXTI9(SW4) ������
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // ��� �����־λ (д�롯1�����������)
        EXTI->PR |= EXTI_PR_PR9;

        // ����
        Com_Delay_ms(10);
        if ((GPIOF->IDR & GPIO_IDR_IDR9) == 0)
        {
            Int_Key_IsSW4Callback();
        }
    }
}

// SW5(PF10) SW6(PF11) ���жϷ������
void EXTI15_10_IRQHandler(void)
{
    // �ж��Ƿ��� EXTI110(SW5) ������
    if (EXTI->PR & EXTI_PR_PR10)
    {
        // ��� �����־λ (д�롯1�����������)
        EXTI->PR |= EXTI_PR_PR10;

        // ����
        Com_Delay_ms(10);
        if (GPIOF->IDR & GPIO_IDR_IDR10)
        {
            Int_Key_IsSW5Callback();
        }
    }

     // �ж��Ƿ��� EXTI11(SW6) ������
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // ��� �����־λ (д�롯1�����������)
        EXTI->PR |= EXTI_PR_PR11;

        // ����
        Com_Delay_ms(10);
        if (GPIOF->IDR & GPIO_IDR_IDR11)
        {
            Int_Key_IsSW6Callback();
        }
    }
}
/**************** �жϷ������ ****************************/
