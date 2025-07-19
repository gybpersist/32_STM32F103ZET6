#ifndef __DRI_USART1_H__
#define __DRI_USART1_H__

#include "Com_Util.h"
#include <stdio.h>

/**
 * @brief ��ʼ��USART1
 * 
 */
void Dri_USART1_Init(void);

// /**
//  * @brief ��Ƭ������һ���ֽڵ�����
//  * 
//  * @param byte Ҫ���͵�һ���ֽ�����
//  */
// void Dri_USART1_TransmitByte(uint8_t byte);

/**
 * @brief ��Ƭ�����Ͷ���ֽڵ�����
 * 
 * @param bytes Ҫ���͵Ķ���ֽ�����
 * @param len �ֽ����ݳ���
 */
void Dri_USART1_TransmitBytes(uint8_t *bytes,uint16_t len);

/**
 * @brief ����һ���ֽ�����,������Ҫһֱ��ѯ
 * 
 * @return uint8_t ���ؽ��յ����ֽ�����
 */
uint8_t Dri_USART1_ReceiveByte(void);

/**
 * @brief ���ն���ֽ�,һֱ����⵽����״̬
 * 
 * @param bytes �����յ����ֽڱ��浽�õ�ַ
 * @param len �����յ����ֽڳ��ȱ��浽�õ�ַ
 */
void Dri_USART1_ReceiveBytes(uint8_t *bytes,uint16_t *len);

/**
 * @brief �������жϵ���ʽ(������) ��������
 * 
 */
void Dri_USART1_Receive_IT_Start(void);

/**
 * @brief �ر����жϵ���ʽ(������) ��������
 * 
 */
void Dri_USART1_Receive_IT_Stop(void);

#endif /* __DRI_USART1_H__ */
