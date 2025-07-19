#ifndef __DRI_TIM6_H__
#define __DRI_TIM6_H__

#include "Com_Util.h"

/**
 * @brief 初始化TIM6
 * 
 * @param up_ms 多长时间产生一次更新事件
 */
void Dri_TIM6_Init(uint16_t up_ms);

/**
 * @brief 启动TIM6
 * 
 */
void Dri_TIM6_Start(void);

/**
 * @brief 停止TIM6
 * 
 */
void Dri_TIM6_Stop(void);

#endif /* __DRI_TIM6_H__ */
