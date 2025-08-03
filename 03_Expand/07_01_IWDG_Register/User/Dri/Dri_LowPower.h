#ifndef __DRI_LOWPOWER_H__
#define __DRI_LOWPOWER_H__

#include "Com_Util.h"

/**
 * @brief 开启低功耗的睡眠模式
 * 
 * @note  该函数开启低功耗的睡眠模式，在睡眠模式下，系统会进入低功耗状态.  
 * @note  唤醒条件 :任一中断(__WFI)/事件(__WFE)
 * @note  唤醒方式 :中断/事件触发后，系统会从睡眠模式唤醒，继续执行后续代码.
 * @note  对 1.8V 区域时钟的影响 :CPU时钟关闭,对其他区域时钟无影响.
 */
void Enable_LowPower_SleepMode(void);

/**
 * @brief 开启低功耗的停止模式 (使用 EXTI 唤醒)
 * 
 * @note  该函数开启低功耗的停止模式，在停止模式下，系统会进入低功耗状态.
 * @note  唤醒条件 :任一 EXTI中断(__WFI)/事件(__WFE)
 * @note  唤醒方式 :EXTI 中断/事件触发后，系统会从停止模式唤醒，继续执行后续代码.(但是需要重新配置时钟)
 * @note  对 1.8V 区域时钟的影响 :关闭 1.8V 区域的所有时钟
 * @note  可选择是否开启低功耗模式(电源控制寄存器PWR_CR)
 * @note  供电选择正常或/功耗
 */
void Enable_LowPower_StopMode(void);

/**
 * @brief 开启低功耗的待机模式 (使用 WKUP引脚 唤醒)
 * 
 * @note  该函数开启低功耗的待机模式，在待机模式下，系统会进入低功耗状态.
 * @note  唤醒条件 :WKUP引脚上升沿
 * @note  唤醒方式 :WKUP引脚上升沿触发后，系统会从待机模式唤醒，会重新开始执行.(因此需要重新使能PWR时钟)
 * @note  对 1.8V 区域时钟的影响 :关闭 1.8V 区域的所有时钟
 * @note  除了待机电路和后备区域，全部断电
 */
void Enable_LowPower_StandbyMode(void);


#endif /* __DRI_LOWPOWER_H__ */
