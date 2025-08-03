#ifndef __DRI_I2C_H__
#define __DRI_I2C_H__

#include "Com_Util.h"

// 定义宏 拉高拉低SCL, SCL连接PB10
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR10)

// 定义宏 SCL延时（半个周期是5us，以实现 100kbit/s 速度，标准模式）
#define SCL_DELAY Com_Delay_us(5)

// 定义宏 拉高拉低SDA, SDA连接接PB11
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR11)

// 定义宏 读取SDA的状态, 结果是0或非0
#define SDA_READ (GPIOB->IDR & GPIO_IDR_IDR11)

/**
 * @brief I2C初始化
 * 
 */
void Dri_I2C_Init(void);

/**
 * @brief 发送起始信号
 * 
 */
void Dri_I2C_Start(void);

/**
 * @brief 发送一个字节
 * 
 * @param byte 要发送的字节
 */
void Dri_I2C_TransmitByte(uint8_t byte);

/**
 * @brief 接收应答信号
 * 
 * @return uint8_t 返回接收到的应答信号，0:ACK; 1:NACK
 */
uint8_t Dri_I2C_ReceiveACK(void);


/**
 * @brief 接收一个字节
 * 
 * @return uint8_t 接收到的字节
 */
uint8_t Dri_I2C_ReceiveByte(void);

/**
 * @brief 回复应答信号
 * 
 * @param ack 应答信号，0:ACK; 1:NACK
 */
void Dri_I2C_TransmitACK(uint8_t ack);

/**
 * @brief 发送停止信号
 * 
 */
void Dri_I2C_Stop(void);


#endif /* __DRI_I2C_H__ */
