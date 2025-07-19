#include "Dri_I2C2.h"

void Dri_I2C2_Init(void)
{
    // 1 ʱ��ʹ�� -----------------------------------------
    // 1.1 �� GPIOB ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 �� I2C2 ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2 GPIO �������� ------------------------------------
    // 2.1 PB10(SCL) ���ÿ�©��� MODE:11  ,CNF:11
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH |= GPIO_CRH_CNF10;
    // 2.2 PB11(SDA) ���ÿ�©��� MODE:11  ,CNF:11
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH |= GPIO_CRH_CNF11;

    // 3 I2C2 ���� ----------------------------------------
    // 3.1 I2C2 ��λ(����ʡ��)
    I2C2->CR1 |= I2C_CR1_SWRST;     // ��1���븴λ״̬
    Com_Delay_ms(100);              // �ӳ�100ms
    I2C2->CR1 &= ~I2C_CR1_SWRST;    // ��0�˳���λ״̬
    Com_Delay_ms(100);              // �ӳ�100ms
    // 3.2 ���� I2C2 ģ�������ʱ��(APB1��ʱ��Ƶ��,36MHZ)       ����T_pclk1=1/36us
    I2C2->CR2 &= ~I2C_CR2_FREQ;     // 6��λ��ȫ����0
    I2C2->CR2 |= 36;                // ����6��λ������Ϊ 100100
    // 3.3 ���� I2C2 ģ���ʱ�ӷ�Ƶϵ��                         ��׼ģʽ�� �ٶ� 100kbit/s -> SCLƵ��:100kHz -> SCL���ڣ�10us
    I2C2->CCR &= ~I2C_CCR_CCR;      // 12��λ��ȫ����0         T_hight=5us T_low=5us 5=CCR*1/36 CCR=180
    I2C2->CCR |= 180;
    // 3.4 ���� I2C2 ʱ�ӵ�������ʱ��,����Ϊ 1000ns=1us         T_pclk1=1/36us ��׼ģʽ���������ʱ��:1us
    I2C2->TRISE = 37;                                      // 1/(1/36) = 36  36+1=37
    // 3.5 ���� I2C2 ģ��
    I2C2->CR1 |= I2C_CR1_PE;
}

void Dri_I2C2_Start(void)
{
    // ������ʼ�ź�
    I2C2->CR1 |= I2C_CR1_START;

    // �ȴ���ʼ�źŷ������
    while ((I2C2->SR1 & I2C_SR1_SB) == 0)
    {
    }
}

void Dri_I2C2_TransmitSlaveAddr(uint8_t addr)
{
    // ���ӻ���ַд�����ݼĴ���
    I2C2->DR = addr;

    // �ȴ��ӻ���ַ�������
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0)
    {
    }

    // ��� ADDR ��־λ,ֻ���ٶ� SR2
    I2C2->SR2;
}

void Dri_I2C2_TransmitByte(uint8_t byte)
{
    // ������д�����ݼĴ���
    I2C2->DR = byte;

    // �ȴ����ݷ������
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0)
    {
    }
}

uint8_t Dri_I2C2_ReceiveACK(void)
{
    // ��ȡ ack ��״̬
    uint8_t ack = (I2C2->SR1 & I2C_SR1_AF) ? 1 : 0;

    // ��� AF ��־λ
    I2C2->SR1 &= ~I2C_SR1_AF;

    return ack;
}

uint8_t Dri_I2C2_ReceiveByte(void)
{
    // �ȴ����յ�һ���ֽ�
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0)
    {
    }

    return I2C2->DR;
}

void Dri_I2C2_TransmitACK(uint8_t ack)
{
    if (ack == 0)
    {
        I2C2->CR1 |= I2C_CR1_ACK;   // �ظ�ACK
    }else
    {
        I2C2->CR1 &= ~I2C_CR1_ACK;  // �ظ�NACK
    }
    
}

void Dri_I2C2_Stop(void)
{
    // ֹͣλ��1
    I2C2->CR1 |= I2C_CR1_STOP;
}
