#ifndef __DRI_IWDG_H__
#define __DRI_IWDG_H__

#include "Com_Util.h"

/**
 * @brief 初始化 IWDG 独立看门狗
 * 
 */
void Dri_IWDG_Init(void);

/**
 * @brief 刷新 IWDG 独立看门狗 (喂狗)
 * 
 */
void Dri_IWDG_Refresh(void);

#endif /* __DRI_IWDG_H__ */
