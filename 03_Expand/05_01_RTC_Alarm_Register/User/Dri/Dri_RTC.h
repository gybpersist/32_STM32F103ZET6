#ifndef __DRI_RTC_H__
#define __DRI_RTC_H__

#include "Com_Util.h"

/**
 * @brief 初始化 RTC 时钟
 * 
 */
void Dri_RTC_Init(void);

/**
 * @brief 设置 RTC 闹钟 ,多少秒后产生闹钟事件
 * 
 * @param seconds 闹钟时间，单位秒
 */
void Dri_RTC_SetAlarm(uint32_t seconds);

#endif /* __DRI_RTC_H__ */
