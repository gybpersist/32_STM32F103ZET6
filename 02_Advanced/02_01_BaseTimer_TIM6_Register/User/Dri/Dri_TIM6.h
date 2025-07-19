#ifndef __DRI_TIM6_H__
#define __DRI_TIM6_H__

#include "Com_Util.h"

/**
 * @brief ��ʼ��TIM6
 * 
 * @param up_ms �೤ʱ�����һ�θ����¼�
 */
void Dri_TIM6_Init(uint16_t up_ms);

/**
 * @brief ����TIM6
 * 
 */
void Dri_TIM6_Start(void);

/**
 * @brief ֹͣTIM6
 * 
 */
void Dri_TIM6_Stop(void);

#endif /* __DRI_TIM6_H__ */
