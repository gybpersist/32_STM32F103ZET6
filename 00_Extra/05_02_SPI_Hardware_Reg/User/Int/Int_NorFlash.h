#ifndef __INT_NORFLASH_H__
#define __INT_NORFLASH_H__

#include "Com_Util.h"
#include "Dri_SPI1.h"

/* 
norflash 总容量：           32M-bit（4MB）。
norflash 块（Block）：      64 个块，每个块 64KB。
norflash 扇区（Sector）：   每个块 16 个扇区，每个扇区 4KB。总扇区数：1024 个。
norflash 页（Page）：       每个扇区 16 个页，每个页 256 字节。总页数：16384 个。 
*/

void Int_NorFlash_GetProductID(uint8_t *manu_id, uint16_t *product_id);

/**
 * @brief 向 NorFlash 的 指定块,指定扇区,指定页 写入数据
 * 
 * @param block_addr 块地址
 * @param sector_addr 扇区地址
 * @param page_addr 页地址
 * @param data 要写入的数据
 * @param data_len 数据长度
 */
void Int_NorFlash_WriteData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t data_len);

/**
 * @brief 从 NorFlash 的 指定块,指定扇区,指定页 读取数据
 * 
 * @param block_addr 块地址
 * @param sector_addr 扇区地址
 * @param page_addr 页地址
 * @param data 用于存储读取数据
 * @param data_len 要读取的数据长度
 */
void Int_NorFlash_ReadData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t data_len);

#endif /* __INT_NORFLASH_H__ */
