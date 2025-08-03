#ifndef __DRI_RTC_H__
#define __DRI_RTC_H__

#include "Com_Util.h"
#include <time.h>

// 定义结构体:保存时间戳转换的时间
typedef struct
{
   uint16_t year;   // 年 
   uint8_t month;   // 月
   uint8_t day;     // 日
   uint8_t hour;    // 时
   uint8_t minute;  // 分
   uint8_t second;  // 秒
} DataTime_t;

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

/**
 * @brief 设置 RTC 起始时间
 * 
 * @param datatime 时间结构体
 */
void Dri_RTC_SetTime(DataTime_t datatime);

/**
 * @brief 获取当前时间
 * 
 * @return DataTime_t 时间结构体
 */
DataTime_t Dri_RTC_GetTime(void);

#endif /* __DRI_RTC_H__ */
