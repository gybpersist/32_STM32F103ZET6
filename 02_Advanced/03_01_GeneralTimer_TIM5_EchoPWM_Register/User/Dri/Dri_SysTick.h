#ifndef __DRI_SYSTICK_H__
#define __DRI_SYSTICK_H__

#include "Com_Util.h"

/**
 * @brief ��ʼ��SysTick,ʵ��ÿ 1ms ����һ���ж�
 */
void Dri_SysTick_Init(void);

/**
 * @brief ��ʱ����,��λ ms
 * 
 * @param ms ��ʱʱ��,��λ ms
 */
void Dri_SysTick_Delay_ms(uint16_t ms);


#endif /* __DRI_SYSTICK_H__ */
