#ifndef __DRI_SPI_H__
#define __DRI_SPI_H__

#include "Com_Util.h"

// 宏定义 片选信号拉高拉低 (PC13)
#define SS_HIGH GPIOC->ODR |= GPIO_ODR_ODR13
#define SS_LOW GPIOC->ODR &= ~GPIO_ODR_ODR13

// 宏定义 时钟信号拉高拉低 (PA5) 及延时
#define SCLK_HIGH GPIOA->ODR |= GPIO_ODR_ODR5
#define SCLK_LOW GPIOA->ODR &= ~GPIO_ODR_ODR5
#define SCLK_DELAY Com_Delay_us(1)        // 半个周期是1us，通信速度 500kbit/s 

// 宏定义 MOSI数据信号拉高拉低 (PA7)
#define MOSI_HIGH GPIOA->ODR |= GPIO_ODR_ODR7   
#define MOSI_LOW GPIOA->ODR &= ~GPIO_ODR_ODR7

// 宏定义 读取MISO数据信号 (PA6)
#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

/**
 * @brief SPI 初始化
 * 
 */
void Dri_SPI_Init(void);

/**
 * @brief SPI 开始通信
 * 
 */
void Dri_SPI_Start(void);

/**
 * @brief SPI 结束通信
 * 
 */
void Dri_SPI_Stop(void);

/**
 * @brief SPI 交换一个字节
 * 
 * @param byte 要交换的字节
 * @return uint8_t 交换后的字节
 */
uint8_t Dri_SPI_SwapByte(uint8_t byte);

#endif /* __DRI_SPI_H__ */
