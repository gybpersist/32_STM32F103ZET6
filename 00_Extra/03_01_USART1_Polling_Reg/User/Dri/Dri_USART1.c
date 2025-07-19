#include "Dri_USART1.h"

void Dri_USART1_Init(void)
{
    // 1 ʱ��ʹ�� 
    // 1.1 GPIOA ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 USART1 ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2 GPIO ����
    // 2.1 ���� TXD(PA9)  ����ģʽ:����������� MODE:11 CNF:10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 ���� RXD(PA10) ����ģʽ:��������ģʽ MODE:00 CNF:01
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3 USART1 ����
    // 3.1 USART1 ������λ�� (8λ)
    USART1->CR1 &= ~USART_CR1_M;        // 0:8λ 1:9λ
    // 3.2 USART1 ��У��λ�� (��)
    USART1->CR1 &= ~USART_CR1_PCE;      // 0:��  1:��У��
    // 3.3 USART1 ��ֹͣλ�� (1λ)
    USART1->CR2 &= ~USART_CR2_STOP;     // 00:1λ 01:0.5λ 10:2λ 11:1.5λ
    // 3.4 USART1 �Ĳ��������� (19200) DIV=39.0625
    USART1->BRR = ((39 << 4) | 0001);
    // 3.5 TXD ����ʹ��
    USART1->CR1 |= USART_CR1_TE;
    // 3.6 RXD ����ʹ��
    USART1->CR1 |= USART_CR1_RE;
    // 3.7 USART1 ʹ��
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * @brief ����һ���ֽ�(8bit)������
 * 
 * @param byte Ҫ���͵�һ���ֽ�(8bit)����
 */
static void Dri_USART1_TransmitByte(uint8_t byte)
{
    // �ȴ�������� (�������ݼĴ���Ϊ��) --> �ȴ���Ϊ1(Ϊ��)
    while ((USART1->SR & USART_SR_TXE) == 0)
    {}
    
    // �ŵ����ݼĴ�����
    USART1->DR = byte;
}

// /**
//  * @brief ���յ�һ���ֽ�(8bit)������
//  * 
//  * @return uint8_t ���յ����ֽ�(8bit)����
//  */
// static uint8_t Dri_USART1_ReceiveByte(void)
// {
//     // �ȴ���ʼ���� (�������ݼĴ����ǿ�) --> �ȴ���Ϊ1(�ǿ�)
//     while ((USART1->SR & USART_SR_RXNE) == 0)
//     {}
    
//     return USART1->DR;
// }

void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t bytes_len)
{
    // ѭ�� ���ͺͽ��ն��ǵ�λ
    for (uint16_t i = 0; i < bytes_len; i++)
    {
        Dri_USART1_TransmitByte(bytes[i]);
    }
}

void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *bytes_len)
{
     *bytes_len = 0; //��ʼ��

    // ѭ�����ն���ֽ�,��������˳�ѭ��
    while (1)
    {
        // �ȴ�������ɵ�ǰ�ֽ�
        while ((USART1->SR & USART_SR_RXNE) == 0){
            // ����ڽ���һ���ֽ��ڼ�,��⵽����״̬,ֱ�ӽ���ѭ��
            if (USART1->SR & USART_SR_IDLE) //USART_SR_IDLE [1:��⵽��������]
            {
                // ���IDLE��־λ,��Ҫ��DR
                USART1->DR;
                return;
            }
        }
        // �Ѵ�DR���յ������ݷŵ�bytes��
        bytes[*bytes_len] = USART1->DR;
        (*bytes_len)++;
    }
}

// �ض��� fputc
int fputc(int ch,FILE *stream){
    // ʹ�ô��ڽ� ch ���ͳ�ȥ
    Dri_USART1_TransmitByte(ch);
    // ���� ch
    return ch;
}
