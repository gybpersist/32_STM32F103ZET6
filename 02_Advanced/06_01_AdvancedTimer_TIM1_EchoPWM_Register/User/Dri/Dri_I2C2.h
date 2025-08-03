#ifndef __DRI_I2C2_H__
#define __DRI_I2C2_H__

#include "Com_Util.h"

/**
 * @brief I2C初始化
 * 
 */
void Dri_I2C2_Init(void);

/**
 * @brief 发送起始信号
 * 
 */
void Dri_I2C2_Start(void);

/**
 * @brief 发送从机地址
 * 
 * @param addr 从机地址+读写标识
 */
void Dri_I2C2_TransmitSlaveAddr(uint8_t addr);

/**
 * @brief 发送一个字节
 * 
 * @param byte 要发送的字节
 */
void Dri_I2C2_TransmitByte(uint8_t byte);

/**
 * @brief 接收应答信号
 * 
 * @return uint8_t 返回接收到的应答信号，0:ACK; 1:NACK
 */
uint8_t Dri_I2C2_ReceiveACK(void);


/**
 * @brief 接收一个字节
 * 
 * @return uint8_t 接收到的字节
 */
uint8_t Dri_I2C2_ReceiveByte(void);

/**
 * @brief 回复应答信号
 * 
 * @param ack 应答信号，0:ACK; 1:NACK
 */
void Dri_I2C2_TransmitACK(uint8_t ack);

/**
 * @brief 发送停止信号
 * 
 */
void Dri_I2C2_Stop(void);


#endif /* __DRI_I2C2_H__ */
