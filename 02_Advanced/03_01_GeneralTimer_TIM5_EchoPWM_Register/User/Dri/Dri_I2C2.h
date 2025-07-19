#ifndef __DRI_I2C2_H__
#define __DRI_I2C2_H__

#include "Com_Util.h"

/**
 * @brief I2C��ʼ��
 * 
 */
void Dri_I2C2_Init(void);

/**
 * @brief ������ʼ�ź�
 * 
 */
void Dri_I2C2_Start(void);

/**
 * @brief ���ʹӻ���ַ
 * 
 * @param addr �ӻ���ַ+��д��ʶ
 */
void Dri_I2C2_TransmitSlaveAddr(uint8_t addr);

/**
 * @brief ����һ���ֽ�
 * 
 * @param byte Ҫ���͵��ֽ�
 */
void Dri_I2C2_TransmitByte(uint8_t byte);

/**
 * @brief ����Ӧ���ź�
 * 
 * @return uint8_t ���ؽ��յ���Ӧ���źţ�0:ACK; 1:NACK
 */
uint8_t Dri_I2C2_ReceiveACK(void);


/**
 * @brief ����һ���ֽ�
 * 
 * @return uint8_t ���յ����ֽ�
 */
uint8_t Dri_I2C2_ReceiveByte(void);

/**
 * @brief �ظ�Ӧ���ź�
 * 
 * @param ack Ӧ���źţ�0:ACK; 1:NACK
 */
void Dri_I2C2_TransmitACK(uint8_t ack);

/**
 * @brief ����ֹͣ�ź�
 * 
 */
void Dri_I2C2_Stop(void);


#endif /* __DRI_I2C2_H__ */
