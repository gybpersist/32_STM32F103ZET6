#ifndef __DRI_ADC1_H__
#define __DRI_ADC1_H__

#include "Com_Util.h"

/**
 * @brief ADC1 初始化
 * 
 */
void Dri_ADC1_Init(void);

/**
 * @brief ADC1 启动转换
 * 
 */
void Dri_ADC1_Start(void);

/**
 * @brief ADC1 获取转换值
 * 
 * @return uint16_t 转换值
 */
uint16_t Dri_ADC1_GetValue(void);

/**
 * @brief ADC1 获取转换值电压
 * 
 * @return double 转换值电压
 */
double Dri_ADC1_GetValue_Vol(void);

/**
 * @brief ADC1 获取转换值温度
 * 
 * @return double 转换值温度
 */
double Dri_ADC1_GetValue_Temp(void);

#endif /* __DRI_ADC1_H__ */
