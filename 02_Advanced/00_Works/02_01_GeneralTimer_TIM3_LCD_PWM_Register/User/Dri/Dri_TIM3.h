#ifndef __DRI_TIM3_H__
#define __DRI_TIM3_H__


#include "Com_Util.h"

/**
 * @brief 初始化 TIM3 ,固定 1ms 产生一次更新事件
 * 
 */
void Dri_TIM3_Init(void);

/**
 * @brief 启动 TIM3 计数
 * 
 */
void Dri_TIM3_Start(void);

/**
 * @brief 停止 TIM3 计数
 * 
 */
void Dri_TIM3_Stop(void);

/**
 * @brief 设置 TIM3_CH3 输出的 PWM 方波的占空比
 * 
 * @param duty 占空比值 ,范围 0 - 100
 */
void Dri_TIM3_CH3_SetPWMDuty(uint8_t duty);



#endif /* __DRI_TIM3_H__ */
