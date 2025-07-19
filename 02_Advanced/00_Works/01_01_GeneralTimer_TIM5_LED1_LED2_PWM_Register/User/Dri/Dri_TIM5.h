#ifndef __DRI_TIM5_H__
#define __DRI_TIM5_H__

#include "Com_Util.h"

/**
 * @brief ��ʼ�� TIM5 ,�̶� 1ms ����һ�θ����¼�
 * 
 */
void Dri_TIM5_Init(void);

/**
 * @brief ���� TIM5 ����
 * 
 */
void Dri_TIM5_Start(void);

/**
 * @brief ֹͣ TIM5 ����
 * 
 */
void Dri_TIM5_Stop(void);

/**
 * @brief ���� TIM5_CH2 ����� PWM ������ռ�ձ�
 * 
 * @param duty ռ�ձ�ֵ ,��Χ 0 - 100
 */
void Dri_TIM5_CH2_SetPWMDuty(uint8_t duty);

/**
 * @brief ���� TIM5_CH1 ����� PWM ������ռ�ձ�
 * 
 * @param duty ռ�ձ�ֵ ,��Χ 0 - 100
 */
void Dri_TIM5_CH1_SetPWMDuty(uint8_t duty);


#endif /* __DRI_TIM5_H__ */
