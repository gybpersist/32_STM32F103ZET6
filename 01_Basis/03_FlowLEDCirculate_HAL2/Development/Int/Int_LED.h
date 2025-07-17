#ifndef __INT_LED_H__
#define __INT_LED_H__

#include "stm32f1xx_hal.h"  

/**
 * @brief ����ָ��LED
 * 
 * @param led ָ����LED
 */
void Int_LED_On(uint16_t led);

/**
 * @brief Ϩ��ָ��LED
 * 
 * @param led ָ����LED
 */
void Int_LED_Off(uint16_t led);

/**
 * @brief �ı�ָ��LED��״̬
 * 
 * @param led ָ����LED
 */
void Int_LED_Toggle(uint16_t led);

/**
 * @brief �������LED
 * 
 * @param leds ��Ҫ������leds����
 * @param leds_len LED���鳤��
 */
void Int_LED_OnAll(uint16_t *leds,uint8_t leds_len);

/**
 * @brief Ϩ����LED
 * 
 * @param leds ��ҪϨ���leds����
 * @param leds_len LED���鳤��
 */
void Int_LED_OffAll(uint16_t *leds,uint8_t leds_len);

#endif /* __INT_LED_H__ */
