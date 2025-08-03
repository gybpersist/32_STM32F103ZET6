#ifndef __DRI_TIM4_H__
#define __DRI_TIM4_H__

#include "Com_Util.h"

/**
 * @brief TIM4 初始化 
 * 
 */
void Dri_TIM4_Init(void);

/**
 * @brief TIM4 启动计数
 * 
 */
void Dri_TIM4_Start(void);

/**
 * @brief TIM4 停止计数
 * 
 */
void Dri_TIM4_Stop(void);

/**
 * @brief 得到测量的 PWM 方波周期 T
 * 
 * @return double 返回 周期,单位 ms
 */
double Dri_TIM4_CH1_GetPWMPeriod(void);

/**
 * @brief 得到测量的 PWM 方波频率 f
 * 
 * @return double 返回 频率,单位 HZ
 */
double Dri_TIM4_CH1_GetPWMFreq(void);

#endif /* __DRI_TIM4_H__ */
