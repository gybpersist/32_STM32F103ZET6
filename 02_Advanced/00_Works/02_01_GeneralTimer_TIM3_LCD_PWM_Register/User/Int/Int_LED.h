#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "Com_Util.h"

// 宏定义 各个LED对应的引脚
#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

/**
 * @brief LED初始化
 * 
 */
void Int_LED_Init(void);

/**
 * @brief 点亮指定LED
 * 
 * @param led 指定的LED
 */
void Int_LED_On(uint16_t led);

/**
 * @brief 熄灭指定LED
 * 
 * @param led 指定的LED
 */
void Int_LED_Off(uint16_t led);

/**
 * @brief 改变指定LED的状态
 * 
 * @param led 指定的LED
 */
void Int_LED_Toggle(uint16_t led);

/**
 * @brief 点亮多个LED
 * 
 * @param leds 需要点亮的leds数组
 * @param leds_len LED数组长度
 */
void Int_LED_OnAll(uint16_t *leds,uint8_t leds_len);

/**
 * @brief 熄灭多个LED
 * 
 * @param leds 需要熄灭的leds数组
 * @param leds_len LED数组长度
 */
void Int_LED_OffAll(uint16_t *leds,uint8_t leds_len);

#endif /* __INT_LED_H__ */
