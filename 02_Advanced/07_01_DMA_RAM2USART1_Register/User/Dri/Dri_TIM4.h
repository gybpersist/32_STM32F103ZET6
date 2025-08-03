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
 * @brief TIM4_CH1 得到测量的 PWM 方波周期 T
 *
 * @return double 返回 周期,单位 ms
 */
double Dri_TIM4_CH1_GetPWMPeriod(void);

/**
 * @brief TIM4_CH1 得到测量的 PWM 方波频率 f
 *
 * @return double 返回 频率,单位 HZ
 */
double Dri_TIM4_CH1_GetPWMFreq(void);

/**
 * @brief TIM4_CH2 得到测量的 PWM 方波占空比
 *
 * @return double 返回 占空比,范围 0-1 的小数
 */
double Dri_TIM4_CH2_GetPWMDuty(void);

#endif /* __DRI_TIM4_H__ */
