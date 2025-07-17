#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__

#define PAGE_SIZE 16 //ÿҳ�ж����ֽ�

#include "Dri_I2C.h"

// ����� �豸��ַ 1010000 + ��д��ʶ
#define DEV_ADDR_W  0xA0
#define DEV_ADDR_R  0xA1

/**
 * @brief ��ָ��λ��д��ָ�����ȵ�����
 *
 * @param addr          Ҫд���λ��
 * @param write_datas   Ҫд�������
 * @param write_len     Ҫд��ĳ���
 */
void Int_EEPROM_WriteData(uint8_t addr, uint8_t *write_datas, uint16_t write_len);

/**
 * @brief ��ָ��λ�ö�ȡָ�����ȵ�����
 *
 * @param addr          Ҫ��ȡ��λ��
 * @param read_datas    ��ȡ�������ݱ��浽�˴�
 * @param read_len      Ҫ��ȡ�ĳ���
 */
void Int_EEPROM_ReadData(uint8_t addr, uint8_t *read_datas, uint16_t read_len);

#endif /* __INT_EEPROM_H__ */
