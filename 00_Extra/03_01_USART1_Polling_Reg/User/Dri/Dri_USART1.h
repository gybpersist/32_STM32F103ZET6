#ifndef __DRI_USART1_H__
#define __DRI_USART1_H__

#include "Com_Util.h"
#include <stdio.h>

/**
 * @brief USART1 ��ʼ��
 * 
 */
void Dri_USART1_Init(void);

/**
 * @brief ���Ͷ���ֽڵ�����
 * 
 * @param bytes Ҫ���͵Ķ���ֽ�����
 * @param bytes_len �����ֽڵĳ���
 */
void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t bytes_len);

/**
 * @brief ���ն���ֽڵ�����
 * 
 * @param bytes ���յ����ֽ����ݴ��λ��
 * @param bytes_len �����ֽڵĳ���
 */
void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *bytes_len);

#endif /* __DRI_USART1_H__ */
