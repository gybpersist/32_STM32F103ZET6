#ifndef __DRI_SPI1_H__
#define __DRI_SPI1_H__

#include "Com_Util.h"

// 宏定义 拉高拉低片选信号
#define SS_HIGH GPIOC->ODR |= GPIO_ODR_ODR13
#define SS_LOW GPIOC->ODR &= ~GPIO_ODR_ODR13

/**
 * @brief 初始化 SPI1
 * 
 */
void Dri_SPI1_Init(void);

/**
 * @brief 启动 SPI1
 * 
 */
void Dri_SPI1_Start(void);

/**
 * @brief 停止 SPI1

 * 
 */
void Dri_SPI1_Stop(void);

/**
 * @brief 交换一个字节
 * 
 * @param byte 要发送的字节
 * @return uint8_t 接收的字节
 */
uint8_t Dri_SPI1_SwapByte(uint8_t byte);


#endif /* __DRI_SPI1_H__ */
