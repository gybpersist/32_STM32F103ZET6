#ifndef __DRI_SYSTICK_H__
#define __DRI_SYSTICK_H__

#include "Com_Util.h"

/**
 * @brief 初始化SysTick,实现每 1ms 产生一次中断
 */
void Dri_SysTick_Init(void);

/**
 * @brief 延时函数,单位 ms
 * 
 * @param ms 延时时间,单位 ms
 */
void Dri_SysTick_Delay_ms(uint16_t ms);


#endif /* __DRI_SYSTICK_H__ */
