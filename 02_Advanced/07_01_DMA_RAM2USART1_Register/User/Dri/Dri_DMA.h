#ifndef __DRI_DMA_H__
#define __DRI_DMA_H__

#include "Com_Util.h"

/**
 * @brief DMA 初始化
 * 
 */
void Dri_DMA_Init(void);

/**
 * @brief 开启 DMA 传输数据 [存储器 -> DMA -> 外设]
 * 
 * @param mem_addr 存储器地址
 * @param periph_addr 外设地址
 * @param len 传输长度
 */
void Dri_DMA_Transfer(uint32_t mem_addr,uint32_t periph_addr,uint16_t len);

#endif /* __DRI_DMA_H__ */
