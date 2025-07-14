#include "Dri_I2C.h"

/**
 * @brief I2C��ʼ��
 *
 */
void Dri_I2C_Init(void)
{
    // 1. �� GPIOB ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. SCL��PB10������Ϊͨ�ÿ�©���, MODE=11��CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;

    // 3. SDA��PB11������Ϊͨ�ÿ�©�����MODE=11��CNF=01
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;

    // 4. ��SCL��SDA���ڿ���״̬
    SCL_HIGH;
    SDA_HIGH;
}

/**
 * @brief ������ʼ�ź�
 *
 */
void Dri_I2C_Start(void)
{
    // ���� SCL
    SCL_HIGH;

    // SDA������������
    SDA_HIGH;
    SCL_DELAY;
    SDA_LOW;

    // ���� SCL ����ʱ
    SCL_LOW;
    SCL_DELAY;
}

/**
 * @brief ����һ���ֽ�
 *
 * @param byte Ҫ���͵��ֽ�
 */
void Dri_I2C_TransmitByte(uint8_t byte)
{
    // ѭ����bit���ͣ��ȷ���λ
    for (uint8_t i = 0; i < 8; i++)
    {
        // �ж������ǰbyte���λ��1������SDA
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        // �����ǰbyte���λ��0������SDA
        else
        {
            SDA_LOW;
        }

        // byte ����
        byte <<= 1;

        // ����SCL������ʱ���ý��շ�����
        SCL_HIGH;
        SCL_DELAY;

        // ����SCL������ʱ, ������һ�ο���ֱ���޸�SDA
        SCL_LOW;
        SCL_DELAY;
    }
}

/**
 * @brief ����Ӧ���ź�
 *
 * @return uint8_t ���ؽ��յ���Ӧ���źţ�0:ACK; 1:NACK
 */
uint8_t Dri_I2C_ReceiveACK(void)
{
    // ���豸�ͷ�SDA
    SDA_HIGH;

    // ����SCL����ȡSDA״̬, ����ʱ
    SCL_HIGH;
    uint8_t ack = SDA_READ ? 1 : 0;
    SCL_DELAY;

    // ����SCL������ʱ, ������һ�ο���ֱ���޸�SDA
    SCL_LOW;
    SCL_DELAY;

    // ����
    return ack;
}

/**
 * @brief ����һ���ֽ�
 * 
 * @return uint8_t ���յ����ֽ�
 */
uint8_t Dri_I2C_ReceiveByte(void)
{
    // ������������ڽ�������
    uint8_t byte = 0x00;

    // ���豸�ͷ�SDA
    SDA_HIGH;

    // ѭ����bit���գ��Ƚ��ո�λ
    for (uint8_t i = 0; i < 8; i++)
    {
        // byte������1λ�������λ�ճ���
        byte <<= 1;

        // ����SCL
        SCL_HIGH;

        // �� SDA ��״̬�� ���SDA�Ǹߵ�ƽ����byte���λд��1; ���SDA�ǵ͵�ƽ��������byte��ǰ���λĬ����0
        if (SDA_READ)
        {
            byte |= 0x01;
        }

        SCL_DELAY;

        // ����SCL����ʾ
        SCL_LOW;
        SCL_DELAY;
    }

    // ����
    return byte;
}

/**
 * @brief �ظ�Ӧ���ź�
 * 
 * @param ack Ӧ���źţ�0:ACK; 1:NACK
 */
void Dri_I2C_TransmitACK(uint8_t ack)
{
    if (ack == 0)
    {
        SDA_LOW;        // ����ACK�ź�
    }
    else
    {
        SDA_HIGH;       // ����NACK�ź�
    }

    // ����SCL����ʱ���ô��豸��ȡ״̬
    SCL_HIGH;
    SCL_DELAY;

    // ����SCL����ʱ
    SCL_LOW;
    SCL_DELAY;
}

/**
 * @brief ����ֹͣ�ź�(��SCL�ߵ�ƽ�ڼ䣬SDA����������)
 *
 */
void Dri_I2C_Stop(void)
{
    // ����SCL֮ǰ������SDA
    SDA_LOW;

    // ���� SCL
    SCL_HIGH;
    SCL_DELAY;

    // ��SCL�ߵ�ƽ�ڼ䣬����SDA
    SDA_HIGH;
}
