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
 * @brief I2C 初始化
 * 
 */
void Dri_I2C_Init(void);

/**
 * @brief I2C 启动信号
 * 
 */
void Dri_I2C_Start(void);
/**
 * @brief I2C 停止信号
 * 
 */
void Dri_I2C_Stop(void);

/**
 * @brief I2C 发送一个字节数据
 * 
 * @param byte 发送的字节数据
 */
void Dri_I2C_TransmitByte(uint8_t byte);

/**
 * @brief 接收应答信号
 * 
 * @return uint8_t ACK 0 表示应答，1 表示非应答
 */
uint8_t Dri_I2C_ReceiveAck(void);

/**
 * @brief I2C 接收一个字节数据
 * 
 * @return uint8_t 接收的字节数据
 */
uint8_t Dri_I2C_ReceiveByte(void);

/**
 * @brief I2C 发送应答信号
 * 
 * @param ACK 0 表示应答，1 表示非应答
 */
void Dri_I2C_TransmitACK(uint8_t ACK);


#endif /* __DRI_I2C_H__ */
