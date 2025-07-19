#ifndef __COM_UTIL_H__
#define __COM_UTIL_H__

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

/**
 * @brief ��ʱ���� us ΢�� 
 * 
 * @param ms 
 */
void Com_Delay_us(uint16_t us);

/**
 * @brief ��ʱ���� ms ����
 * 
 * @param ms 
 */
void Com_Delay_ms(uint16_t ms);

#endif /* __COM_UTIL_H__ */
