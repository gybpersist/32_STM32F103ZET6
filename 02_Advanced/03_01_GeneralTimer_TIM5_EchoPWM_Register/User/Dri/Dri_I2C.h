#ifndef __DRI_I2C_H__
#define __DRI_I2C_H__

#include "Com_Util.h"

// ����� ��������SCL, SCL����PB10
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR10)

// ����� SCL��ʱ�����������5us����ʵ�� 100kbit/s �ٶȣ���׼ģʽ��
#define SCL_DELAY Com_Delay_us(5)

// ����� ��������SDA, SDA���ӽ�PB11
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR11)

// ����� ��ȡSDA��״̬, �����0���0
#define SDA_READ (GPIOB->IDR & GPIO_IDR_IDR11)

/**
 * @brief I2C��ʼ��
 * 
 */
void Dri_I2C_Init(void);

/**
 * @brief ������ʼ�ź�
 * 
 */
void Dri_I2C_Start(void);

/**
 * @brief ����һ���ֽ�
 * 
 * @param byte Ҫ���͵��ֽ�
 */
void Dri_I2C_TransmitByte(uint8_t byte);

/**
 * @brief ����Ӧ���ź�
 * 
 * @return uint8_t ���ؽ��յ���Ӧ���źţ�0:ACK; 1:NACK
 */
uint8_t Dri_I2C_ReceiveACK(void);


/**
 * @brief ����һ���ֽ�
 * 
 * @return uint8_t ���յ����ֽ�
 */
uint8_t Dri_I2C_ReceiveByte(void);

/**
 * @brief �ظ�Ӧ���ź�
 * 
 * @param ack Ӧ���źţ�0:ACK; 1:NACK
 */
void Dri_I2C_TransmitACK(uint8_t ack);

/**
 * @brief ����ֹͣ�ź�
 * 
 */
void Dri_I2C_Stop(void);


#endif /* __DRI_I2C_H__ */
