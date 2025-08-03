#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "stm32f1xx_hal.h"  

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
