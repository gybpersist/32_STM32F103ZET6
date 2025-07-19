#ifndef __DRI_USART1_H__
#define __DRI_USART1_H__

#include "Com_Util.h"
#include <stdio.h>

/**
 * @brief USART1 初始化
 * 
 */
void Dri_USART1_Init(void);

/**
 * @brief 发送多个字节的数据
 * 
 * @param bytes 要发送的多个字节数据
 * @param bytes_len 发送字节的长度
 */
void Dri_USART1_TransmitBytes(uint8_t *bytes, uint16_t bytes_len);

/**
 * @brief 接收多个字节的数据
 * 
 * @param bytes 接收到的字节数据存放位置
 * @param bytes_len 接收字节的长度
 */
void Dri_USART1_ReceiveBytes(uint8_t *bytes, uint16_t *bytes_len);

#endif /* __DRI_USART1_H__ */
