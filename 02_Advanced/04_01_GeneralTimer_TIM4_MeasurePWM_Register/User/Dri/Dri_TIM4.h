#ifndef __DRI_TIM4_H__
#define __DRI_TIM4_H__

#include "Com_Util.h"

/**
 * @brief TIM4 ��ʼ�� 
 * 
 */
void Dri_TIM4_Init(void);

/**
 * @brief TIM4 ��������
 * 
 */
void Dri_TIM4_Start(void);

/**
 * @brief TIM4 ֹͣ����
 * 
 */
void Dri_TIM4_Stop(void);

/**
 * @brief �õ������� PWM �������� T
 * 
 * @return double ���� ����,��λ ms
 */
double Dri_TIM4_CH1_GetPWMPeriod(void);

/**
 * @brief �õ������� PWM ����Ƶ�� f
 * 
 * @return double ���� Ƶ��,��λ HZ
 */
double Dri_TIM4_CH1_GetPWMFreq(void);

#endif /* __DRI_TIM4_H__ */
