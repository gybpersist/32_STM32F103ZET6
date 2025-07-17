#include "Dri_USART1.h"

static uint8_t s_rx_bytes[128]; //������Ž��յ�������
static uint16_t s_rx_len = 0;   //����������յ������ݵĳ���

void Dri_USART1_Init(void)
{
    // 1 ʱ��ʹ�ܣ���GPIOA��USART1 ʱ��ʹ�� ---------------------------------------->
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

    // 2 ���� GPIOA ������ -------------------------------------------------------->
    // 2.1 TX����(PA9)����Ϊ����������� MODE=11,CNF=10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // 2.2 RX����(PA10)����Ϊ����������߸���������� MODE=00,CNF=01  (Ĭ��ֵ)
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3 ���� USART1 -------------------------------------------------------------->
    // 3.1 ʹ������ USART1 ����
    USART1->CR1 |= USART_CR1_UE;
    // 3.2 ʹ�� ����ʹ��TXD
    USART1->CR1 |= USART_CR1_TE;
    // 3.3 ʹ�� ����ʹ��RXD
    USART1->CR1 |= USART_CR1_RE;
    // 3.4 ��������֡ -> ����λ���� 8 λ (Ĭ��ֵ)
    USART1->CR1 &= ~USART_CR1_M;
    // 3.5 ��������֡ -> ��У��λ (Ĭ��ֵ)
    USART1->CR1 &= ~USART_CR1_PCE; 
    // 3.6 ��������֡ -> 1ֹͣλ (Ĭ��ֵ)          
    USART1->CR2 &= ~USART_CR2_STOP;                          //         f(72MHZ)
    // 3.7 ���������� -> ����19200������,�����������:39.0625   //������=---------------   
    USART1->BRR = ((39 << 4) + 0001);                        //        16 * ��������

    // // 4 USART1 �����ж�ʹ�� ���� Dri_USART1_Receive_IT_Start(); ������
    // USART1->CR1 |= USART_CR1_RXNEIE; //ʹ�� RXNE �ж�
    // USART1->CR1 |= USART_CR1_IDLEIE; //ʹ�� IDLE �ж�

    // 5 NVIC ����
    NVIC_SetPriorityGrouping(3); 
    NVIC_SetPriority(USART1_IRQn,10); //���ж������������ȼ�
    NVIC_EnableIRQ(USART1_IRQn); //ʹ���ж�����
}

 /**
 * @brief ��Ƭ������һ���ֽڵ�����
 * 
 * @param byte Ҫ���͵�һ���ֽ�����
 */
static void Dri_USART1_TransmitByte(uint8_t byte)
{
    // �ȴ��������ݼĴ���Ϊ��,��־λ TXE ��0��һֱѭ��,ֱ��Ϊ1
    while ((USART1->SR & USART_SR_TXE) == 0){
    }

    // ��Ҫ���͵��ֽ�д�뷢�����ݼĴ���
    USART1->DR = byte;
}

void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t len)
{
    // ѭ��:һλһλ�ķ���
    for (uint16_t i = 0; i < len; i++)
    {
        Dri_USART1_TransmitByte(bytes[i]);
    }
}

uint8_t Dri_USART1_ReceiveByte(void)
{
    // �ȴ��������ݼĴ���Ϊ�ǿ�,��־λ RXNE ��0��һֱѭ��,ֱ��Ϊ1
    while ((USART1->SR & USART_SR_RXNE) == 0){  
    }

    // ����һ���ֽ�����
    return USART1->DR;
}

void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *len)
{
    *len = 0; //��ʼ��

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
        bytes[*len] = USART1->DR;
        (*len)++;
    }
}

void Dri_USART1_Receive_IT_Start(void)
{
    // 4 USART1 �����ж�ʹ�� 
    USART1->CR1 |= USART_CR1_RXNEIE; //ʹ�� RXNE �ж�
    USART1->CR1 |= USART_CR1_IDLEIE; //ʹ�� IDLE �ж�
}

void Dri_USART1_Receive_IT_Stop(void)
{
    // 4 USART1 �ر��ж�ʹ�� 
    USART1->CR1 &= ~USART_CR1_RXNEIE; 
    USART1->CR1 &= ~USART_CR1_IDLEIE; 
}

/**
 * @brief ����������ݵĻص�����
 * 
 * @param rx_bytes ���յ������ݱ��浽����
 * @param rx_len ���յ������ݵĳ���
 * @return __weak 
 */
__weak void Dri_USART1_ReceiveCallback(uint8_t *rx_bytes,uint16_t rx_len)
{
}

/**
 * @brief USART1 ���жϷ������
 * 
 */
void USART1_IRQHandler(void){

    // �ж��Ƿ��� RXNE �жϴ�����,˵�����յ�һ���ֽ�
    if (USART1->SR &= USART_SR_RXNE)
    {
        s_rx_bytes[s_rx_len] = USART1->DR;
        s_rx_len++;
    }
    
    // �ж������Ƿ��ǿ���,˵��ȫ���������
    if (USART1->SR & USART_SR_IDLE)
    {
        // ����������,�����յ����ݴ���
        Dri_USART1_ReceiveCallback(s_rx_bytes,s_rx_len);
        // ���ճ�������
        s_rx_len = 0;
        // ��� IDLE ��־λ,��Ҫ�ٶ�һ��DR
        USART1->DR;
    }
    
}

// ���¶��� fputc() ����
int fputc(int ch,FILE *stream){
    // ʹ�ô��ڽ� ch ���ͳ�ȥ
    Dri_USART1_TransmitByte(ch);
    // ���� ch
    return ch;
}
