#ifndef __DRI_TIM1_H__
#define __DRI_TIM1_H__

#include "Com_Util.h"
#define TIM1_CH1_PWM_DUTY 35    // TIM1_CH1 的 PWM 方波占空比设置 (1-100%)

/**
 * @brief TIM1 初始化
 * 
 */
void Dri_TIM1_Init(void);

/**
 * @brief 开启 TIM1 计数
 * 
 */
void Dri_TIM1_Start(void);

/**
 * @brief 关闭 TIM1 计数
 * 
 */
void Dri_TIM1_Stop(void);

#endif /* __DRI_TIM1_H__ */
