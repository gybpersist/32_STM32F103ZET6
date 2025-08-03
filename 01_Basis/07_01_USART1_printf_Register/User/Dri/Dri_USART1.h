#ifndef __DRI_USART1_H__
#define __DRI_USART1_H__

#include "Com_Util.h"
#include <stdio.h>

/**
 * @brief 初始化USART1
 * 
 */
void Dri_USART1_Init(void);

// /**
//  * @brief 单片机发送一个字节的数据
//  * 
//  * @param byte 要发送的一个字节数据
//  */
// void Dri_USART1_TransmitByte(uint8_t byte);

/**
 * @brief 单片机发送多个字节的数据
 * 
 * @param bytes 要发送的多个字节数据
 * @param len 字节数据长度
 */
void Dri_USART1_TransmitBytes(uint8_t *bytes,uint16_t len);

/**
 * @brief 接收一个字节数据,接收需要一直轮询
 * 
 * @return uint8_t 返回接收到的字节数据
 */
uint8_t Dri_USART1_ReceiveByte(void);

/**
 * @brief 接收多个字节,一直到检测到空闲状态
 * 
 * @param bytes 将接收到的字节保存到该地址
 * @param len 将接收到的字节长度保存到该地址
 */
void Dri_USART1_ReceiveBytes(uint8_t *bytes,uint16_t *len);

/**
 * @brief 开启以中断的形式(非阻塞) 接收数据
 * 
 */
void Dri_USART1_Receive_IT_Start(void);

/**
 * @brief 关闭以中断的形式(非阻塞) 接收数据
 * 
 */
void Dri_USART1_Receive_IT_Stop(void);

#endif /* __DRI_USART1_H__ */
