#ifndef __COM_UTIL_H__
#define __COM_UTIL_H__

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

/**
 * @brief 延时多少 us 微秒 
 * 
 * @param ms 
 */
void Com_Delay_us(uint16_t us);

/**
 * @brief 延时多少 ms 毫秒
 * 
 * @param ms 
 */
void Com_Delay_ms(uint16_t ms);

#endif /* __COM_UTIL_H__ */
