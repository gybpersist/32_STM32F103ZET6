#ifndef __DRI_SPI1_H__
#define __DRI_SPI1_H__

#include "Com_Util.h"

// 宏定义 片选信号拉高拉低 (PC13)
#define SS_HIGH GPIOC->ODR |= GPIO_ODR_ODR13
#define SS_LOW GPIOC->ODR &= ~GPIO_ODR_ODR13

/**
 * @brief SPI 初始化
 * 
 */
void Dri_SPI1_Init(void);

/**
 * @brief SPI 开始通信
 * 
 */
void Dri_SPI1_Start(void);

/**
 * @brief SPI 结束通信
 * 
 */
void Dri_SPI1_Stop(void);

/**
 * @brief SPI 交换一个字节
 * 
 * @param byte 要交换的字节
 * @return uint8_t 交换后的字节
 */
uint8_t Dri_SPI1_SwapByte(uint8_t byte);

#endif /* __DRI_SPI1_H__ */
