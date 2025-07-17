#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__

#define PAGE_SIZE 16 //每页有多少字节

#include "Dri_I2C.h"

// 定义宏 设备地址 1010000 + 读写标识
#define DEV_ADDR_W  0xA0
#define DEV_ADDR_R  0xA1

/**
 * @brief 向指定位置写入指定长度的数据
 *
 * @param addr          要写入的位置
 * @param write_datas   要写入的数据
 * @param write_len     要写入的长度
 */
void Int_EEPROM_WriteData(uint8_t addr, uint8_t *write_datas, uint16_t write_len);

/**
 * @brief 从指定位置读取指定长度的数据
 *
 * @param addr          要读取的位置
 * @param read_datas    读取到的数据保存到此处
 * @param read_len      要读取的长度
 */
void Int_EEPROM_ReadData(uint8_t addr, uint8_t *read_datas, uint16_t read_len);

#endif /* __INT_EEPROM_H__ */
