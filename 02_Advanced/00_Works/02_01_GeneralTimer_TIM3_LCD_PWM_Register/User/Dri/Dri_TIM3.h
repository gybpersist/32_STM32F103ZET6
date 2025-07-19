#ifndef __DRI_TIM3_H__
#define __DRI_TIM3_H__


#include "Com_Util.h"

/**
 * @brief ��ʼ�� TIM3 ,�̶� 1ms ����һ�θ����¼�
 * 
 */
void Dri_TIM3_Init(void);

/**
 * @brief ���� TIM3 ����
 * 
 */
void Dri_TIM3_Start(void);

/**
 * @brief ֹͣ TIM3 ����
 * 
 */
void Dri_TIM3_Stop(void);

/**
 * @brief ���� TIM3_CH3 ����� PWM ������ռ�ձ�
 * 
 * @param duty ռ�ձ�ֵ ,��Χ 0 - 100
 */
void Dri_TIM3_CH3_SetPWMDuty(uint8_t duty);



#endif /* __DRI_TIM3_H__ */
