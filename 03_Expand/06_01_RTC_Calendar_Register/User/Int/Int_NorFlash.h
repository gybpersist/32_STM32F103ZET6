#ifndef __INT_NORFLASH_H__
#define __INT_NORFLASH_H__

#include "Com_Util.h"

/**
 * @brief 获取 NorFlash 厂商编号 和 产品 ID
 * 
 * @param manu_id 厂商编号
 * @param product_id 产品 ID
 */
void Int_NorFlash_GetProductID(uint8_t *manu_id,uint16_t *product_id);

/**
 * @brief 向 NorFlash 写入数据
 * 
 * @param block_addr 块地址
 * @param sector_addr 扇区地址
 * @param page_addr 页地址
 * @param write_datas 写入的数据
 * @param len 写入的数据的数据长度
 */
void Int_NorFlash_WritePageData(uint8_t block_addr,uint8_t sector_addr,uint8_t page_addr,uint8_t *write_datas,uint16_t len);

/**
 * @brief 从 NorFlash 读取数据
 * 
 * @param block_addr 块地址
 * @param sector_addr 扇区地址
 * @param page_addr 页地址
 * @param byte_addr 字节地址
 * @param read_datas 存放读取的数据
 * @param len 需要读取的数据长度
 */
void Int_NorFlash_ReadPageData(uint8_t block_addr,uint8_t sector_addr,uint8_t page_addr,uint8_t byte_addr,uint8_t *read_datas,uint16_t len);

#endif /* __INT_NORFLASH_H__ */
