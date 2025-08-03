#ifndef __DRI_TIM5_H__
#define __DRI_TIM5_H__

#include "Com_Util.h"

/**
 * @brief 初始化 TIM5 ,固定 1ms 产生一次更新事件
 * 
 */
void Dri_TIM5_Init(void);

/**
 * @brief 启动 TIM5 计数
 * 
 */
void Dri_TIM5_Start(void);

/**
 * @brief 停止 TIM5 计数
 * 
 */
void Dri_TIM5_Stop(void);

/**
 * @brief 设置 TIM5_CH2 输出的 PWM 方波的占空比
 * 
 * @param duty 占空比值 ,范围 0 - 100
 */
void Dri_TIM5_CH2_SetPWMDuty(uint8_t duty);

/**
 * @brief 设置 TIM5_CH1 输出的 PWM 方波的占空比
 * 
 * @param duty 占空比值 ,范围 0 - 100
 */
void Dri_TIM5_CH1_SetPWMDuty(uint8_t duty);


#endif /* __DRI_TIM5_H__ */
