#ifndef __DRI_SPI_H__
#define __DRI_SPI_H__

#include "Com_Util.h"

// 宏定义 SPI通信速度 1us为半个时钟周期 -> T=2us -> f=500KHZ (最大为18MHZ,应小于18MHZ)
#define SCL_DELAY Com_Delay_us(1)

// 宏定义 片选信号拉高拉低 (PC13)
#define SS_HIGH GPIOC->ODR |= GPIO_ODR_ODR13
#define SS_LOW GPIOC->ODR &= ~GPIO_ODR_ODR13

// 宏定义 时钟信号拉高拉低 (PA5)
#define SCK_HIGH GPIOA->ODR |= GPIO_ODR_ODR5
#define SCK_LOW GPIOA->ODR &= ~GPIO_ODR_ODR5

// 宏定义 主设备输出数据信号拉高拉低 (PA7)
#define MOSI_HIGH GPIOA->ODR |= GPIO_ODR_ODR7
#define MOSI_LOW GPIOA->ODR &= ~GPIO_ODR_ODR7

// 宏定义 主设备输入数据信号读取 (PA6)
#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

/**
 * @brief 初始化 SPI
 * 
 */
void Dri_SPI_Init(void);

/**
 * @brief 启动 SPI
 * 
 */
void Dri_SPI_Start(void);

/**
 * @brief 停止 SPI
 * 
 */
void Dri_SPI_Stop(void);

/**
 * @brief 交换一个字节
 * 
 * @param byte 要发送的字节
 * @return uint8_t 接收的字节
 */
uint8_t Dri_SPI_SwapByte(uint8_t byte);


#endif /* __DRI_SPI_H__ */
