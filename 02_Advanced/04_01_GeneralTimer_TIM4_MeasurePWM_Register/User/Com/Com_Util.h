#ifndef __COM_UTIL_H__
#define __COM_UTIL_H__

#include <stdint.h>
#include "stm32f10x.h"  

// �ӳٶ���΢��
void Com_Delay_us(uint16_t count);

// �ӳٶ��ٺ���
void Com_Delay_ms(uint16_t count);

void Com_Delay_s(uint16_t count);

#endif /* __COM_UTIL_H__ */
